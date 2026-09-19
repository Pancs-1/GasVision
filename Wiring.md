### 🔌 Circuit Wiring Reference

| Component | Component Pin | Arduino Pin | Description / Notes | Power / Ground Connections |
| :--- | :--- | :--- | :--- | :--- |
| **Potentiometer** | Wiper (Middle Pin) | **A0** | Analog input adjustment | VCC to 5V, GND to GND |
| **MQ-6 LPG Sensor** | AOUT | **A1** | Gas concentration analog signal | VCC to 5V, GND to GND |
| **I2C LCD Display** | SDA <br> SCL | **A4** <br> **A5** | Data Line <br> Clock Line | VCC to 5V, GND to GND |
| **DS18B20 Temp Sensor** | Data | **D2** | One-Wire data stream | **Requires 4.7kΩ pull-up resistor** to 5V |
| **Servo Motor** | Signal (Yellow/Orange) | **D9** | PWM control for position | VCC to 5V, GND to GND |
| **Cooling Fan** | Control (Base/Gate) | **D8** | Digital output to trigger relay/transistor | External power usually required for fan motor |
| **Push Button** | Pin 1 | **D7** | Input trigger configured as `INPUT_PULLUP` | Pin 2 connects directly to GND |
