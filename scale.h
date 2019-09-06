//-------------------------------------------------------------------------------------
// HX711_ADC.h
// Arduino master library for HX711 24-Bit Analog-to-Digital Converter for Weigh Scales
// Olav Kallhovd sept2017
// Tested with      : HX711 asian module on channel A and YZC-133 3kg load cell
// Tested with MCU  : Arduino Nano, ESP8266
//-------------------------------------------------------------------------------------
// This is an example sketch on how to use this library
// Settling time (number of samples) and data filtering can be adjusted in the config.h file

#include <EEPROM.h>
#include "userConfig.h"

//HX711 constructor (dout pin, sck pin):

int eepromAdress = 0;
#include <HX711_ADC.h>
HX711_ADC LoadCell(SCALE1,0);
HX711_ADC LoadCell2(SCALE2,4);
long tim;
long tim2;

void initScale() {
  float calValue; // calibration value
  calValue = 182.76;  //369.80; // uncomment this if you want to set this value in the sketch
  //#if defined(ESP8266)
  //EEPROM.begin(512); // uncomment this if you use ESP8266 and want to fetch this value from eeprom
  //#endif
  //EEPROM.get(eepromAdress, calValue); // uncomment this if you want to fetch this value from eeprom
  LoadCell.begin();
  long stabilisingtime = 2000; // tare preciscion can be improved by adding a few seconds of stabilising time
  //LoadCell.start(stabilisingtime);
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Tare timeout, check MCU>HX711 wiring and pin designations");
  }
  else {
    LoadCell.setCalFactor(calValue); // set calibration factor (float)
    Serial.println("Startup + tare is complete");
  }
  while (!LoadCell.update());
  Serial.print("Calibration factor: ");
  Serial.println(LoadCell.getCalFactor());  
  Serial.print("HX711 measured conversion time ms: ");
  Serial.println(LoadCell.getConversionTime());
  Serial.print("HX711 measured sampling rate HZ: ");
  Serial.println(LoadCell.getSPS());
  Serial.print("HX711 measured settlingtime ms: ");
  Serial.println(LoadCell.getSettlingTime());
  Serial.println("Note that the settling time may increase significantly if you use delay() in your sketch!");
  if (LoadCell.getSPS() < 7) {
    Serial.println("!!Sampling rate is lower than specification, check MCU>HX711 wiring and pin designations");
  }
  else if (LoadCell.getSPS() > 100) {
    Serial.println("!!Sampling rate is higher than specification, check MCU>HX711 wiring and pin designations");
  }
}

void initScale2() {
  float calValue2; // calibration value
  calValue2 = 202.01;  //369.80; // uncomment this if you want to set this value in the sketch
  LoadCell2.begin();
  long stabilisingtime = 2000; // tare preciscion can be improved by adding a few seconds of stabilising time
  if (LoadCell2.getTareTimeoutFlag()) {
    Serial.println("Tare timeout, check MCU>HX711 wiring and pin designations");
  }
  else {
    LoadCell2.setCalFactor(calValue2); // set calibration factor (float)
    Serial.println("Startup + tare is complete");
  }
  while (!LoadCell2.update());
  Serial.print("Calibration factor: ");
  Serial.println(LoadCell2.getCalFactor());  
  Serial.print("HX711 measured conversion time ms: ");
  Serial.println(LoadCell2.getConversionTime());
  Serial.print("HX711 measured sampling rate HZ: ");
  Serial.println(LoadCell2.getSPS());
  Serial.print("HX711 measured settlingtime ms: ");
  Serial.println(LoadCell2.getSettlingTime());
  Serial.println("Note that the settling time may increase significantly if you use delay() in your sketch!");
  if (LoadCell2.getSPS() < 7) {
    Serial.println("!!Sampling rate is lower than specification, check MCU>HX711 wiring and pin designations");
  }
  else if (LoadCell2.getSPS() > 100) {
    Serial.println("!!Sampling rate is higher than specification, check MCU>HX711 wiring and pin designations");
  }
}
int getWeight() {
  float weight = 0;
     LoadCell.update();
  //get smoothed value from data set + current calibration factor
  if (millis() > tim + 250) {
    float i = LoadCell.getData();
    Serial.print("Load_cell output val: ");
    Serial.println(i);
    weight = i;
    tim = millis();
  }
  return(weight);
}
int getWeight2() {
  //update() should be called at least as often as HX711 sample rate; >10Hz@10SPS, >80Hz@80SPS
  //use of delay in sketch will reduce effective sample rate (be carefull with delay() in the loop)
  float weight2 = 0;
     LoadCell2.update();
  //get smoothed value from data set + current calibration factor
  if (millis() > tim2 + 250) {
    float i = LoadCell2.getData();
    Serial.print("Load_cell output val: ");
    Serial.println(i);
    weight2 = i;
    tim2 = millis();

  return(weight2);
}
