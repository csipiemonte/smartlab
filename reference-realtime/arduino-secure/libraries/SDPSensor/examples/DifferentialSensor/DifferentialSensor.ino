/*
 * SensorRead.ino
 *          Demonstrates the sensor Arduino library
 *          This program reads a sensor that measures a physical phenomenon as difference between 2 analog 
 *          output. Analog Input 1 is connected to the primary output line and Analog Input 0 to the 
 *          secondary one.  
 * 
 *          For each sensor in your system you must instantiate an
 *          instance of its using the analog input ports to
 *          which it is connected, i.e.--
 * 
 *              DifferentialSensor <name_of_the_instance>(<primary_pin_number>, <secondary_pin_number>);
 *              DifferentialSensor indoor(1,0);
 *              DifferentialSensor outdoor(3,2);

 * 
 * Authors:
 *         Marco Boeris Frusca <marco.boeris@csp.it>
 * 
 */ 
#include <Time.h>
#include <analogsensor.h>
#include <differentialsensor.h>

DifferentialSensor s(1, 0);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Start program!");
}

void loop() {
  // put your main code here, to run repeatedly: 

  Serial.print(s.getWorkingElectrode().measure());
  Serial.print("  ");  
  Serial.print(s.getAuxiliaryElectrode().measure());
  Serial.print("  ");  
  Serial.println(s.measure());
  
  delay(1000);
}