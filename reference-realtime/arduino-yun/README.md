Smart Data Platform (SDP)
==============

Arduino Yun and Yucca
--------------
This code is an example how to send a message to Yucca platform using an Arduino Yun.

- *sketch/* directory has got an example of sketch that sends a message to Yucca server.

Requirement
--------------
Libraries required are:
- [PubSubClient](https://github.com/csipiemonte/pubsubclient). This library is a fork from [pubsubclient project](https://github.com/knolleary/pubsubclient), if you get the original code, you have to apply the patch *pubsubclient_20140706*.
- [aJson](https://github.com/interactive-matter/aJson).
- [MemoryFree](http://playground.arduino.cc/code/AvailableMemory) only for debug


Install libraries
--------------
To install a library, you have to copy the directory that contains all source files into your libraries folder. Restart Arduino to rescan for new libraries. For more information look Arduino [guide](http://arduino.cc/en/Guide/Libraries).
Install all required and SDP libraries. For example, if you have installed Arduino IDE in /opt directory, you can use the following instructions:

```
cd libraries/
cp -r <library> <arduino-path>/libraries/
```


Sketch
--------------
This sketch sends a constants JSON to the Yucca server. 

```
{
  "stream":"flussoProva",
  "sensor":"6e6fdf33-e8f0-4ee8-ba35-386de213e722",
  "values":[
   {
     "time":"2015-01-27T16:48:11Z",
     "components":
       {
         "valoreDouble":"17.4",
         "valoreString":"50"
       }
   }
 ]
}
```


To customize your message, you ave to change your sensor id and stream name:

```
//! Stream
char* stream = "flussoProva";
// Sensor identifier
char* id = "6e6fdf33-e8f0-4ee8-ba35-386de213e722";
```

and modify the createMsg function, changing components:

```
          
          // Component 1
          aJson.addStringToObject(compontents, "valoreDouble", "17.4");
          // Component 2
          aJson.addStringToObject(compontents, "valoreString", "50");
 ```


