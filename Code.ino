#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Servo.h>

// ---------------- Pin definitions ----------------
#define POT_PIN       A0
#define MQ6_PIN       A1
#define ONE_WIRE_BUS  2
#define SERVO_PIN     9   // suggested
#define FAN_PIN       8   // suggested (drives transistor/relay, NOT the fan directly)
#define BUTTON_PIN    7   // suggested

// ---------------- Objects ----------------
LiquidCrystal_I2C lcd(0x27, 16, 2); // change to 0x3F if the LCD doesn't init
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature tempSensor(&oneWire);
Servo cylinderServo;

// ---------------- Gas detection state ----------------
enum DangerLevel { LOW_DANGER, MODERATE_DANGER, INTERMEDIATE_DANGER, GAS_LEAK };
DangerLevel currentDanger = LOW_DANGER;
bool fanLatched = false;

// ---------------- Servo sweep state ----------------
int servoAngle = 0;
int servoDirection = 1;               // 1 = increasing, -1 = decreasing
unsigned long lastServoStep = 0;
const unsigned long SERVO_STEP_INTERVAL = 333; // 1 degree every 0.333s

// ---------------- Temperature change detection ----------------
float lastTemp = -1000;               // sentinel so first reading always registers
unsigned long lastTempCheck = 0;
const unsigned long TEMP_CHECK_INTERVAL = 1000; // DS18B20 needs ~750ms to convert

// ---------------- Gas sensor polling ----------------
unsigned long lastGasCheck = 0;
const unsigned long GAS_CHECK_INTERVAL = 200;

int cylinderPercent = 0;

void setup() {
  Serial.begin(9600);

  pinMode(FAN_PIN, OUTPUT);
  digitalWrite(FAN_PIN, LOW);
  pinMode(BUTTON_PIN, INPUT_PULLUP); // button: other leg to GND

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("System Init...");

  tempSensor.begin();
  tempSensor.requestTemperatures();
  lastTemp = tempSensor.getTempCByIndex(0);

  cylinderServo.attach(SERVO_PIN);
  cylinderServo.write(servoAngle);

  delay(1000);
  lcd.clear();
}

void loop() {
  unsigned long now = millis();

  updateServoSweep(now);

  if (now - lastGasCheck >= GAS_CHECK_INTERVAL) {
    lastGasCheck = now;
    checkGasLevel();
  }

  if (now - lastTempCheck >= TEMP_CHECK_INTERVAL) {
    lastTempCheck = now;
    checkTemperatureChange();
  }

  handleFanAndButton();
  updateDisplay();
}

// Non-blocking servo sweep: 1 degree per 0.333s, 0->180->0 repeating
void updateServoSweep(unsigned long now) {
  if (now - lastServoStep >= SERVO_STEP_INTERVAL) {
    lastServoStep = now;
    servoAngle += servoDirection;
    if (servoAngle >= 180) {
      servoAngle = 180;
      servoDirection = -1;
    } else if (servoAngle <= 0) {
      servoAngle = 0;
      servoDirection = 1;
    }
    cylinderServo.write(servoAngle);
  }
}

void checkGasLevel() {
  int potRaw = analogRead(POT_PIN);
  int gasRaw = analogRead(MQ6_PIN);

  if (potRaw < 10) potRaw = 10; // avoid divide-by-near-zero

  float gasPercent = ((float)gasRaw / (float)potRaw) * 100.0;

  if (gasPercent >= 100.0) {
    currentDanger = GAS_LEAK;
    fanLatched = true; // stays latched on until button press
  } else if (gasPercent >= 50.0) {
    currentDanger = INTERMEDIATE_DANGER;
  } else if (gasPercent <= 20.0) {
    currentDanger = LOW_DANGER;
  } else {
    currentDanger = MODERATE_DANGER; // 21-49%, filled-in state, see note at top
  }
}

// Recompute cylinder % (based on current servo position) only when temp changes
void checkTemperatureChange() {
  tempSensor.requestTemperatures();
  float currentTemp = tempSensor.getTempCByIndex(0);

  if (currentTemp != lastTemp) {
    cylinderPercent = map(servoAngle, 0, 180, 0, 100);
    lastTemp = currentTemp;
  }
}

void handleFanAndButton() {
  if (fanLatched) {
    digitalWrite(FAN_PIN, HIGH);
    if (digitalRead(BUTTON_PIN) == LOW) { // pressed (active LOW w/ pullup)
      delay(50); // debounce
      if (digitalRead(BUTTON_PIN) == LOW) {
        fanLatched = false;
        digitalWrite(FAN_PIN, LOW);
        currentDanger = LOW_DANGER; // reset system state
      }
    }
  } else {
    digitalWrite(FAN_PIN, LOW);
  }
}

void updateDisplay() {
  lcd.setCursor(0, 0);
  switch (currentDanger) {
    case LOW_DANGER:
      lcd.print("Low Danger      ");
      break;
    case MODERATE_DANGER:
      lcd.print("Moderate Danger ");
      break;
    case INTERMEDIATE_DANGER:
      lcd.print("Intermediate Dgr");
      break;
    case GAS_LEAK:
      lcd.print("GAS LEAK ALERT! ");
      break;
  }

  lcd.setCursor(0, 1);
  lcd.print("Cylinder: ");
  lcd.print(cylinderPercent);
  lcd.print("%   ");
}
