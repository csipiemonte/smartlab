/*
 * SensorRead.ino
 *          Demonstrates the sensor Arduino library
 *          This program reads a generic sensor connected to Analog Input 0 
 *          and sends the relative values as voltage to the serial monitor
 *          every 2 seconds.
 * 
 *          For each sensor in your system you must instantiate an
 *          instance of its using the analog input port to
 *          which it is connected, i.e.--
 * 
 *              AnalogSensor <name_of_the_instance>(<pin_number>);
 *              AnalogSensor indoor(0);
 *              AnalogSensor outdoor(1);

 * 
 * Authors:
 *         Marco Boeris Frusca <marco.boeris@csp.it>
 * 
 */ 
#include <analogsensor.h>

//! Time between two consecutive measurements
#define INTERVAL 2000

long prevMillis;           // for main timing loop
AnalogSensor humidity(0);   // temperature sensor instance

void setup()
{
  // Serial port is used to transmit converted data
  Serial.begin(9600);

  // Put a reasonable value into our timing variable so the
  // delay before the main loop isn't something wacky.
  prevMillis = millis();
}

void loop()
{
  /*
   The portion of the main loop below executes once every
   'INTERVAL' milliseconds.
   */
  if ((millis() - prevMillis) > INTERVAL) 
  {
    prevMillis = millis();
    
    // Measure and show the temperature with a little 
    // formatting.
    Serial.print(humidity.measure());
    Serial.println("V. ");
    
    Serial.print("Average on 20 samples: ");
    Serial.print(humidity.measureAvg(20));
    Serial.println("V. ");
    
  } // if ((millis() - prevMillis) > INTERVAL) ...  
}