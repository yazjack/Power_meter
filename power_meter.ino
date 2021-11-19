#include <Wire.h>
#include <Adafruit_INA219.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"

#define I2C_ADDRESS 0x3C
SSD1306AsciiAvrI2c oled;
Adafruit_INA219 ina219_A(0x40);
Adafruit_INA219 ina219_B(0x41);
Adafruit_INA219 ina219_C(0x44);
Adafruit_INA219 ina219_D(0x45);

float shuntvoltage_A = 0, shuntvoltage_B = 0, shuntvoltage_C = 0, shuntvoltage_D = 0, busvoltage_A = 0, busvoltage_B = 0, busvoltage_C = 0, busvoltage_D = 0, current_A = 0, current_B = 0, current_C = 0, current_D = 0, loadvoltage_A = 0, loadvoltage_B = 0, loadvoltage_C = 0, loadvoltage_D = 0, power_A = 0, power_B = 0, power_C = 0, power_D = 0, offset = 0, current_off_A = 0;

void setup(void)  {
  uint32_t currentFrequency;

  Serial.begin(9600);

  // Initialize the INA219.
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range (see comments).
  ina219_A.begin();
  ina219_B.begin();
  ina219_C.begin();
  ina219_D.begin();
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  oled.setFont(System5x7);
  
  // To use a slightly lower 32V, 1A range (higher precision on amps):
  //ina219.setCalibration_32V_1A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  ina219_A.setCalibration_16V_400mA();
  ina219_B.setCalibration_16V_400mA();
  ina219_C.setCalibration_16V_400mA();
  ina219_D.setCalibration_16V_400mA();
}

void loop(void) {  
  shuntvoltage_A = ina219_A.getShuntVoltage_mV();
  shuntvoltage_B = ina219_B.getShuntVoltage_mV();
  shuntvoltage_C = ina219_C.getShuntVoltage_mV();
  shuntvoltage_D = ina219_D.getShuntVoltage_mV();
  busvoltage_A = ina219_A.getBusVoltage_V();
  busvoltage_B = ina219_B.getBusVoltage_V();
  busvoltage_C = ina219_C.getBusVoltage_V();
  busvoltage_D = ina219_D.getBusVoltage_V();
  current_A = ina219_A.getCurrent_mA();
  current_B = ina219_B.getCurrent_mA();
  current_C = ina219_C.getCurrent_mA();
  current_D = ina219_D.getCurrent_mA();
  loadvoltage_A = busvoltage_A + (shuntvoltage_A / 1000);
  loadvoltage_B = busvoltage_B + (shuntvoltage_B / 1000);
  loadvoltage_C = busvoltage_C + (shuntvoltage_C / 1000);
  loadvoltage_D = busvoltage_D + (shuntvoltage_D / 1000);

  power_A = loadvoltage_A * current_off_A / 1000;
  power_B = loadvoltage_B * current_B / 1000;
  power_C = loadvoltage_C * current_C / 1000;
  power_D = loadvoltage_D * current_D / 1000;

  current_off_A = current_A+0.7;
  
  if(current_off_A<=0){
    current_off_A=0;
    power_A = 0;
  }
  if(current_B<=0){
    current_B = 0;
    power_B = 0;
  }
  if(current_C<=0){
    current_C = 0;
    power_C = 0;
  }
  if(current_D<=0){
    current_D = 0;
    power_D = 0;
  }
  float busvoltage_A_r = 0;
  busvoltage_A_r = ((int)(busvoltage_A*100))/100.0;

  oled.setCursor(16,1); oled.print("BUS1"); oled.setCursor(46,1); oled.print("BUS2"); oled.setCursor(76,1); oled.print("BUS3"); oled.setCursor(106,1); oled.print("BUS4");
  oled.setCursor(0,2);
  oled.print("Vb"); oled.setCursor(16,2); oled.print(busvoltage_A, 1); oled.setCursor(46,2); oled.print(busvoltage_B, 1); oled.setCursor(76,2); oled.print(busvoltage_C, 1); oled.setCursor(106,2); oled.print(busvoltage_D, 1);
  oled.setCursor(0,3);
  oled.print("Vl"); oled.setCursor(16,3); oled.print(loadvoltage_A, 2); oled.setCursor(46,3); oled.print(loadvoltage_B, 2); oled.setCursor(76,3); oled.print(loadvoltage_C, 2); oled.setCursor(106,3); oled.print(loadvoltage_D, 2);
  oled.setCursor(0,4);
  oled.print("mA"); oled.setCursor(16,4); oled.print(current_A, 0); oled.setCursor(46,4); oled.print(current_B, 0); oled.setCursor(76,4); oled.print(current_C, 0); oled.setCursor(106,4); oled.print(current_D, 0);
  oled.setCursor(0,5);
  oled.print("W"); oled.setCursor(16,5); oled.print(power_A, 2); oled.setCursor(46,5); oled.print(power_B, 2); oled.setCursor(76,5); oled.print(power_C, 2); oled.setCursor(106,5); oled.print(power_D, 2);

  delay(100);
}
