/*

 This example create JSON to publish measurement in the "Smart Data Platform".


 created 29 July 2014
 by Marco Boeris Frussca
 modified 29 July 2014
 by Marco Boeris Frusca
 */
 
#include <aJSON.h>
#include <sdppublishmsg.h>
#include <MemoryFree.h>

void setup() 
{
  Serial.begin(9600);
  
  printMemory("Setup 000");
  
  bool error = false;
  {
    sdp::message::PublishJSON publish("Thermos_a", "550e8400-e29b-41d4-a716-446655440000");
    printMemory("PublishJSON 000");
    for (int i = 0; i < 2; i++)
    {
      // Create JSON passing time and valid fields
      sdp::message::ValueJSON value("2014-07-28T16:15:32Z", "valid");

      printMemory("ValueJSON 000");
      // Add components 
      if ( !value.addComponent("temperature", 21.4 + i) )
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

      publish.addValue(value);
      
      // print json  
      value.dump(Serial);
      Serial.println();
      printMemory("ValueJSON 100");
    }

    // print json  
    publish.dump(Serial);
    Serial.println();
    printMemory("PublishJSON 100");
  }
  printMemory("Setup 100");
}

void loop() {
  // put your main code here, to run repeatedly:

}


void printMemory(char* msg)
{
  Serial.print( msg );
  Serial.print( " : " );
  Serial.println( freeMemory() );  
}
