# GasVision
**GasVision** is an Arduino-based safety and telemetry system designed to monitor LPG (Liquefied Petroleum Gas) cylinders. It serves two critical functions:
1. **Level Tracking via Thermal Differential:** It utilizes precise temperature sensing (`DS18B20`) to detect the boundary line between the liquid phase and gas phase inside the cylinder, providing an estimate of remaining fuel.
2. **Autonomous Leakage Mitigation:** It continuously samples air quality using an `MQ-6` gas sensor. If an LPG leakage is detected, it instantly triggers an exhaust fan via a control circuit to safely vent the gas and actuates a safety servo motor.

---

## 🛠️ System Features
* **Real-time Telemetry:** Displays current temperature variations and ambient gas levels on an I2C LCD screen.
* **Manual Calibration:** Includes a potentiometer to manually fine-tune sensor thresholds or interface views.
* **Active Safety Ventilation:** Drives a relay/transistor circuit to activate an emergency cooling/exhaust fan during critical thresholds.
* **User Interactivity:** Features a physical toggle button configured via internal pullups for mode switching or alarm resets.

---

## 🔌 Circuit Wiring Reference

| Component | Component Pin | Arduino Pin | Description / Notes | Power / Ground Connections |
| :--- | :--- | :--- | :--- | :--- |
| **Potentiometer** | Wiper (Middle Pin) | **A0** | Analog input adjustment / threshold calibration | VCC to 5V, GND to GND |
| **MQ-6 LPG Sensor** | AOUT | **A1** | Gas concentration analog signal | VCC to 5V, GND to GND |
| **I2C LCD Display** | SDA <br> SCL | **A4** <br> **A5** | Data Line <br> Clock Line | VCC to 5V, GND to GND |
| **DS18B20 Temp Sensor** | Data | **D2** | One-Wire thermal data stream | **Requires 4.7kΩ pull-up resistor** to 5V |
| **Servo Motor** | Signal (Yellow/Orange) | **D9** | PWM control for mechanical safety shutoff | VCC to 5V, GND to GND |
| **Cooling Fan** | Control (Base/Gate) | **D8** | Digital output to trigger relay or transistor | External power source required for motor |
| **Push Button** | Pin 1 | **D7** | Input trigger configured as `INPUT_PULLUP` | Pin 2 connects directly to GND |

---

## 🚀 Getting Started & Installation

### Prerequisites
To deploy this code to your Arduino hardware, ensure you have downloaded and configured the [Arduino IDE](https://arduino.cc).

### Required Libraries
Before compiling, install the following libraries via the Arduino Library Manager (**Sketch** > **Include Library** > **Manage Libraries...**):
1. `<Wire.h>` (Built-in) — Handles the I2C communications layout.
2. `<LiquidCrystal_I2C.h>` — Drives your character LCD interface over I2C.
3. `<OneWire.h>` — Manages the physical one-wire bus communication protocol.
4. `<DallasTemperature.h>` — Translates data sent from the DS18B20 temperature probe.
5. `<Servo.h>` (Built-in) — Provides smooth positional hardware sweeps for safety shutoff.


### Flashing the Code
1. Clone or download this repository.
2. Open `Code.ino` in the Arduino IDE.
3. Connect your Arduino board via USB and select the corresponding **Board** and **Port** under the `Tools` menu.
4. Click **Verify** (the checkmark icon) to compile, followed by **Upload** (the arrow icon) 🚀.
