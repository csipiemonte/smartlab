/*

 This example create  JSON for a value in the "Smart Data Platform".


 created 29 July 2014
 by Marco Boeris Frussca
 modified 29 July 2014
 by Marco Boeris Frusca
 */
 
#include <aJSON.h>
#include <sdppublishmsg.h>

void setup() 
{
  Serial.begin(9600);
  
  bool error = false;
  {
    // Create JSON passing time and valid fields
    sdp::message::ValueJSON value("2014-07-28T16:15:32Z", "valid");

    // Add components 
    if ( !value.addComponent("temperature", 21.4) )
    {
      error = true;
      Serial.println(F("Error: impossible create component"));
    }
    if ( !value.addComponent("latitude", +7.684521) )
    {
      error = true;
      Serial.println(F("Error: impossible create component"));
    }
    if ( !value.addComponent("longitude", +45.196482) )
    {
      error = true;
      Serial.println(F("Error: impossible create component"));
    }
    if ( !value.addComponent("altitude", +254) )
    {
      error = true;
      Serial.println(F("Error: impossible create component"));
    }

    // print json  
    value.dump(Serial);
    Serial.println();
  }

}

void loop() {
  // put your main code here, to run repeatedly:

}
