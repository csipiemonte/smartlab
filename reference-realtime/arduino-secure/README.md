Smart Data Platform (SDP)
==============

Arduino secure JSON
--------------
This folder contains a first version of a secure protocol to send on Arduino platform a JSON created using smartdatanet specification and control management. T
This code extends the features of the [*reference implementation*](https://github.com/csipiemonte/smartlab/tree/master/reference-realtime/arduino): commenting the line 

```
//! Global macro value to define if use secure JSON or standard SDP JSON
#define SECURE_JSON 1
```

the sketch will send a JSON according to the [*smartdatanet specification*](http://www.smartdatanet.it/).

- *libraries/* directory has got all classes and interfaces that implement SDP specification.
- *sketch/* directory has got an example of sketch that uses SDP Arduino libraries.
- *patch/* patch file to use Time library on Arduino IDE > 1.5.6

MQTTClient_sec.ino sketch can be work with WiFi and Ethernet shield. Ethernet on Arduino Mega has got some bugs in connection management and require a periodacal hardware reset. It is tested in the following configuration:
- Arduino Mega 2560 rev3
- Arduino WiFi Shield
- Arduino IDE 1.5.6-r2. You can download it from [Arduino Previous IDE Versions](http://arduino.cc/en/Main/OldSoftwareReleases)
- TI LM35 (temperature sensor)
The Arduino updates its local time from a NTP server. If you want to use a different sensor, you have to create e new convertion function in according to the sensor datasheet. 

**Note:**
Secure JSON and control management are developing features so to communicate with the smartdata platform you have to disable them.

Requirement
--------------
Libraries required are:
- [Time](http://playground.arduino.cc/code/time) 
- [PubSubClient](https://github.com/csipiemonte/pubsubclient). This library is a fork from [pubsubclient project](https://github.com/knolleary/pubsubclient), if you get the original code, you have to apply the patch *pubsubclient_20140706*.
- [aJson](https://github.com/csipiemonte/aJson). This library is a fork from [aJson project](https://github.com/interactive-matter/aJson).
- [Cryptosuite](https://github.com/csipiemonte/Cryptosuite). This library is a fork from [cryptosuite project](https://github.com/Cathedrow/Cryptosuite).
- [arduino-base64](https://github.com/csipiemonte/arduino-base64). This library is a fork from [arduino-base64 project](https://github.com/adamvr/arduino-base64).
- [MemoryFree](http://playground.arduino.cc/code/AvailableMemory) only for debug


Install libraries
--------------
To install a library, you have to copy the directory that contains all source files into your libraries folder. Restart Arduino to rescan for new libraries. For more information look Arduino [guide](http://arduino.cc/en/Guide/Libraries).
Install all required and SDP libraries. For example, if you have installed Arduino IDE in /opt directory, you can use the following instructions:

```
cd libraries/
cp -r SDP* /opt/arduino-1.5.6-r2/libraries/
```
If you use the lastest version of Arduino IDE (>= 1.5.7) you have to patch the Time library:

```
cd arduino-1.5.7/libraries/
patch -p1 < Time_ide_1.5.7.patch
```



Secure JSON format
--------------
The *secure json* has the following format:

```
{"object":<SDP JSON>,"signature":"<SDP JSON signature>"}
```

SDP JSON signature is coded in Base64 format and it is calculated taking the JSON as a string without *new line* and *spaces* characters. For example:

```
{"object":{"stream":"temperature","sensor":"550e8400-e29b-41d4-a716-446655440000","values":[{"time":"2014-08-20T15:26:57Z","components":{"c0":24.41406},"validity":"valid"}]},"signature":"9rfugFJ7qm0my+wu/a0WrlNArYYg3ntnd78xOSwqipg="}
```

**Note:**
MQTT publish packet can be large maximum 256 Byte!




Remote control and configuration
--------------
The remote control and configuration is a developing SDP feature. It uses the broker MQTT to receive command and new configuration.
A standard for the format of the configuration message sent to the Arduino in not already defined, so this a proposal.
The Arduino subscribes itself to this topic on the MQTT broker:

```
output/<tenant>/control
```

the new command will have the following format:

```
{ "to":"<smartobject_id>\","msg": "<control_msg>","data": <data_filed> }
```

where:
- *smartobject_id*: the object identifier
- *control_msg*: control message. At the moment it is support only WCONFIG (write new configuration in local file) and CONFIG (load configuration from file)
- *data*: this field can contain some information or be empty. 

All messages response, when they are expected, are sent to the following topic

```
output/<tenant>/control
```

**WCONFIG message**

WCONFIG is used to change current configuration. The "data" field contains the new configuration as a CSV line. For example:
```
{"to":"550e8400-e29b-41d4-a716-446655440000","msg": "wconfig","data": {"sec": "time","csv": "15;300;"}}
```
This message will change the reading time for the sensor*550e8400-e29b-41d4-a716-446655440000* and the time interval to update the system time. This changes are not active, they will be only saved in the configuration file on the SD card. They will be active at the next boot.

**CONFIG message**

CONFIG is used to load the configuration from the SDcard. It is useful to update configuration after the WCONFIG message without rebooting the Arduino. This is an example:
```
{"to":"550e8400-e29b-41d4-a716-446655440000","msg": "config","data": ""}
```

**STATUS message**

STATUS is used to get the status of the smart object. At  the moment the response message is a subset of all property defined for this message. The information included  in Status message are:
- *id*: smart object identifier
- *time*: system time
- *last*: time stamp of the last successfully sent
- *next*: time stamp of the expected measurement

```
{"id":"550e8400-e29b-41d4-a716-446655440000","time":"2014-10-20T09:22:29Z","last":"2014-10-20T09:22:24Z","next":"2014-10-20T09:22:12Z"}

```

**Note:**
This features is work in progress. 


Documentation
--------------
Arduino SDP libraries use [*doxygen*](http://www.stack.nl/~dimitri/doxygen/) tool to create the source code documentation.
To create documentation for SDP libraries use the command:

```
cd libraries/
doxygen sdp.doxygen
```

Sketch MQTT
--------------
The sketch *MQTTClient_sec.ino* can be used with WiFi and Ethernet shield. The ethernet shield on Arduino Mega has got some bugs in connection management and require a periodacal hardware reset.
The Arduino node takes current time from a NTP server in the setup function, and periodically updates it  in the loop.
You can configure node behavior comment or uncomment some MACRO at the begin of the sketch. This is an example for WiFi connection and automatic configuration (DHCP). Change IP address of the MQTT broker with your working one and WiFi configuration with your networnk SSID and keyword (for WEP or WPA).
This version supports MQTT authentication. To use it, you must set up the following parameters:

```
//! MQTT Username
#define USERNAME "smartlab"

//! MQTT Username
#define PASSWORD "smartlab$1"
```

This is an example of configuration (at the moment GPS location are not used):

```
//******************************************************************************************
 * Global configuration
 * In this section you can enable or disable feature of the node.
 *
 * WIFI               : define if the network coonection is Cable or Wifi. Comment it if
 *                      you use a cable connection (ethernet shield)
 *
 * STATIC_NETWORK_CFG : define if network configuration is static or automatic. Comment it
 *                      if you want to use automatic configuration (DHCP). 
 *                      If nerwork configuration is static, it is required to define the
 *                      following parameter:
 *                        - ARDUINO_IP_ADDRESS
 *                        - NETWORK_SUBNET
 *                        - NETWORK_GATEWAY
 *                        - NETWORK_DNS
 *
 * SDP_SERVER_AS_IP   : define if MQTT server is described as a IP address or a domain.
 *                      Comment it to use a domain definition
 *
 * SECURE_JSON        : use secure json. Default value is 0 (not). Comment it if you want 
 *                      to use standard SDP JSON. 
 * CONTROL            : process control message coming from the broker MQTT (work in progress)
 *
 ******************************************************************************************/
//! Global macro value to define if network connection is cable or Wi-Fi
#define WIFI 1

//! Global macro value to define if network configuration is static or automatic. Comment this define to use automatic configuration (DHCP)
//#define STATIC_NETWORK_CFG 1

//! Global macro value to define if MQTT server is defined by IP address or domain string. Comment this define to use domain string
//#define SDP_SERVER_AS_IP 1

//! Global macro value to define if use secure JSON or standard SDP JSON
//#define SECURE_JSON 1

//! Global macro value to define if handle changing configuration form remote using the broker MQTT
//#define CONTROL 1



#ifdef WIFI

// Arduino IDE
#include <WiFi.h>
#include <WiFiUdp.h>

// Non-standard library
#include <wifimanager.h>

#else

// Arduino IDE
#include <Ethernet.h>
#include <EthernetUdp.h>

#endif


/******************************************************************************************
 * Arduino network configuration
 * In this section you can set up the network configuration of Arduino if it is static.
 * DHCP and static configuration require you to define a MAC address.
 *
 * ARDUINO_MAC_ADDRESS : MAC address of the Arduino board. This address has to be unique.
 *
 * ARDUINO_IP_ADDRESS  : IP address of the Arduino board.
 *
 * NETWORK_SUBNET      : Network mask.
 *
 * NETWORK_GATEWAY     : Default gateway of the network.
 *
 * NETWORK_DNS         : Primary DNS of the network.
 *
 ******************************************************************************************/
//! Arduino MAC address (make this unique). It is used by Ethernet shield
#define ARDUINO_MAC_ADDRESS { 0xDE, 0xED, 0xBA, 0xDE, 0xFE, 0xED }

//! Arduino IP address 
#define ARDUINO_IP_ADDRESS  { 192, 168, 1, 100 } 

//! IP address mask (usually this value) 
#define NETWORK_SUBNET  { 255, 255, 255,0 };

//! Default gateway (your router) 
#define NETWORK_GATEWAY { 192, 168, 1, 254 };

//! DNS address (e.g. 208.67.222.222 from open DNS)
#define NETWORK_DNS  { 208, 67, 222, 222 };




/******************************************************************************************
 * Arduino wireless configuration
 * In this section you can set up the wireless network configuration.
 *
 * WIRELESS_TYPE       : Wireless network type. This value can be 0 for OPEN, 1 for WEP 
 *                       or 2 for WPA
 *
 * WIRELESS_SSID       : Wireless network identifier
 *
 * WIRELESS_KEY        : Cryptography key for WEP or WPA.
 *
 * WIRELESS_INDEX      : Key index (Only for WEP coding)
 *
 ******************************************************************************************/
// Wireless type or coding
#define WIRELESS_TYPE 2

// Wireless SSID
#define WIRELESS_SSID "network";

// Wireless key or password
#define WIRELESS_KEY "secret";

// Wireless index (WEP code)
#define WIRELESS_INDEX 0



/******************************************************************************************
 * NTP configuration
 * In this section you can set up the IP Address of NTP server. If it is used to get the 
 * current data and time.
 *
 * NTP_SERVER_IP     : IP address of the MQTT server.
 *
 ******************************************************************************************/
//! IP address of NTP server
#define NTP_SERVER_IP { 193, 204, 114, 232 };



/******************************************************************************************
 * MQTT server configuration
 * In this section you can set up the MQTT server configuration. If it is defined 
 * SDP_SERVER_AS_IP you have to set MQTT_SERVER_IP value, otherwise MQTT_SERVER_DOMAIN.
 *
 * MQTT_SERVER_IP     : IP address of the MQTT server.
 *
 * MQTT_SERVER_DOMAIN : Doamin of the MQTT server.
 *
 ******************************************************************************************/
//! IP address of MQTT broker
#define MQTT_SERVER_IP { 192, 168, 1, 50 };

//! domain of MQTT broker
#define MQTT_SERVER_DOMAIN "stream.smartdatanet.it";

//! MQTT Username
#define MQTT_USERNAME "smartlab"

//! MQTT Username
#define MQTT_PASSWORD "smartlab$01"



/******************************************************************************************
 * Smart object description
 * In this section you can set all main parameter of the smart object.
 *
 * TENANT                  : Publishing tenant (or domain)
 *
 * SENSOR_ANALOG_INPUT_PIN : Number if the analog input where the sensor is connected
 *
 * SMRTOBJ_ID              : Identifier of the smartdata object
 *
 * STREAM_IDENTIFIER       : Identifier of the data stream
 *
 * GPS_LATITUDE  : GPS latitude of the smart object. It must be in Decimal Degrees
 *
 * GPS_LONGITUDE : GPS longitude of the smart object. It must be in Decimal Degrees
 *
 * GPS_ALTITUDE  : GPS altitude of the smart object. It must be in Decimal Degrees
 *
 * POLLING_TIME  : Time between two consecutive reads of a sensor. It is in seconds
 *
 ******************************************************************************************/
//! Publishing Tenant
#define TENANT "smartlab"
 
//! Analog input pin of the Arduino connected to the sensor
#define SENSOR_ANALOG_INPUT_PIN  0

//! Sensor identifier
#define SMRTOBJ_ID  "550e8400-e29b-41d4-a716-446655440000"

//! Sensor identifier
#define STREAM_IDENTIFIER  "temperature"

//! GPS latitude of the smart object
#define GPS_LATITUDE  "45.065913"

//! GPS Longitude of the smart object
#define GPS_LONGITUDE "7.658273"

//! GPS Altitude of the smart object
#define GPS_ALTITUDE  "248"



/******************************************************************************************
 * Smart Object timing
 * In this section you can set up the timing when change state and do operations (e.g. read
 * sensors, update date).
 *
 * READ_SENSOR_PERIOD : read sensors period or polling time (in milliseconds).
 *
 * UPDATE_TIME_PERIOD : time between two NTP request (in milliseconds).
 *
 ******************************************************************************************/
//! IP address of MQTT broker
#define READ_SENSOR_PERIOD 5000L

//! domain of MQTT broker
#define UPDATE_TIME_PERIOD 60000L



/******************************************************************************************
 * Secure
 * In this section you can set up the secure parameters (keyword for h-mac).
 *
 * HMAC_KEYWORD        : H-MAC secret or keyword
 *
 * HMAC_KEYWORD_LENGTH : length of the h-MAc secret
 *
 ******************************************************************************************/
//! H-MAC secret or keyword
//#define HMAC_KEYWORD  { 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19 };
#define HMAC_KEYWORD  { 's','m','a','r','t','l','a','b' };

//#define HMAC_KEYWORD_LENGTH  25
#define HMAC_KEYWORD_LENGTH  8


/******************************************************************************************
 * Configuration file system
 * In this section you can se the configuration file system.
 *
 * CFG_DIRECTORY        : directory where configuration files are saved
 *
 * SEC_TIME             : filename for section "time"
 *
 ******************************************************************************************/
//! Configuration directory
#define CFG_DIRECTORY "/etc"

//! filename for section time
#define SEC_TIME_FN "time.cvs"



```



