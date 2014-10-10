/*
 Smart Data Platform Client

 Basic node for the Smard Data Platform. This sketch can be used with WiFi and 
 Ethernet shield. Ethernet on Arduino Mega has got some bugs in connection 
 management and require a periodacal hardware reset.
 Time is got from a Network Time Protocol (NTP) time server in the setup 
 function, and periodically in the loop.

 created 1 Jul 2014
 by Marco Boeris Frusca
 modified 7 Jul 2014
 by Marco Boeris Frusca

 */




/*----------------------------------------------------------------------------------------
/* INCLUDE
/*----------------------------------------------------------------------------------------*/

// Arduino IDE
#include <SPI.h>
#include <SD.h>

// Non-standard library
#include <Time.h>

#include <Time.h>
#include <PubSubClient.h> 

#include <sdpserver.h>
#include <sdpsource.h>
#include <sdpstream.h>
#include <analogsensor.h>
#include <gpsposition.h>
#include <measure.h>

#include <ntpclient.h>
#include <networkconfig.h>

#include <interval.h>

#include <sha256.h>
#include <Base64.h>
#include <aJSON.h>
#include <sdppublishmsg.h>
#include <sdpctrlmsg.h>

#include <stringparser.h>
#include <csvline.h>

#include <avr/pgmspace.h>


/*----------------------------------------------------------------------------------------
/* CONFIGURATION
/*----------------------------------------------------------------------------------------*/
//! Debug memory usage
#define DEBUG_MEMORY

#ifdef DEBUG_MEMORY
#include <MemoryFree.h>
#endif



/******************************************************************************************
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
#define MQTT_SERVER_IP { 192, 168, 1, 200 };

//! domain of MQTT broker
#define MQTT_SERVER_DOMAIN "stream.smartdatanet.it";

//! MQTT Username
#define MQTT_USERNAME "smartlab"

//! MQTT Username
#define MQTT_PASSWORD "smartlab$1"



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



//! Size of a local buffer used to store data read from Flash memory
#define FLASH_BUFFER_SIZE 40


/*------------------------------------------------------------------------------------
/* Enum and type definiton
/*------------------------------------------------------------------------------------*/
/*
 * Running errors code
 */
enum _error_code
{
  //! No error
  NO_ERROR = 0,

  //! GPS position, invalid latitude
  GPS_INVALID_LATITUDE = 100,

  //! GPS position, invalid longitude
  GPS_INVALID_LONGITUDE = 101,

  //! GPS position, invalid altitude
  GPS_INVALID_ALTITUDE = 102,
  
  //! SD Error
  OPEN_SD_ERROR = 104,
};


enum _node_state
{
  //! Idle, do nothing
  IDLE = 0, 
  
  //! Read sensors
  READ_SENSORS = 1,
   
  //! Update time through NTP request
  NTPUPDATE = 2,
 
};


enum _wireless_index
{
  //! Index for flash wireless table to get ssid
  W_SSID = 0, 
  
  //! Index for flash wireless table to get keyword
  W_KEY = 1,
};

enum _mqtt_index
{
  //! Index for flash MQTT table to get username
  MQTT_USER = 0,
  
  //! Index for flash MQTT table to get password
  MQTT_PASS = 1,
};

enum _sensor_index
{
  //! Index for flash sensor table to get tenant
  NODE_TENANT = 0,
  
  //! Index for flash sensor table to get sensor identifier 
  NODE_ID = 1,

  //! Index for flash sensor table to get stream identifier 
  STREAM_ID = 2,
};


enum _config_index
{
  //! Index for flash sensor table to get configuration directory
  CFG_DIR = 0,
  
  //! Index for flash sensor table to get time configuration filename 
  TIME_FN = 1,
};

/*
 * Arduino_boards
 */
enum _board
{
  //! DEFAULT ARDUINO
  GENERIC_ARDUINO,
  
 //! ARDUINO MEGA
 ARDUINO_MEGA,
};

/**
 * type of arduino board
 *
 */
enum _shield
{
  //! Ethernet shield
  SHIELD_ETHERNET,
  
  //! WiFi shield
  SHIELD_WIFI,
  
  //! Sparkfun
  SHIELD_SPARKFUN,
  
  //! Adafruit
  SHIELD_ADAFRUIT,
};


/*----------------------------------------------------------------------------------------
/* GLOBAL VARIABLES
/*----------------------------------------------------------------------------------------*/
//! Flash variable buffer
char flashBuffer[FLASH_BUFFER_SIZE] = {0};

//! Network configuration
NetworkConfig netConfig;


#ifdef WIFI

//! Wireless network SSID (name)
const char w_ssid[] PROGMEM = WIRELESS_SSID;

//! Wireless network password
const char w_key[] PROGMEM = WIRELESS_KEY;

//! Flash table for wireless configuration
const char* const wireless_table[] PROGMEM =
{   
  w_ssid,
  w_key
};

//! Wireless network key Index number (needed only for WEP)
int keyIndex = WIRELESS_INDEX ;

//! Wireless code (0: open, 1: WEP, 2: WPA)
int type = WIRELESS_TYPE;

//! Wireless connection handler
WiFiManager wifiHandler;

//! WiFi flag
bool wifiEnable = true;

#else

//" MAC address of ethernet shield
byte mac[]    = ARDUINO_MAC_ADDRESS; 

#endif


//! Local port to listen for UDP packets
unsigned int localPort = 2390;

//! Address of NTP server
byte ntpServerIP[4] = NTP_SERVER_IP; 

#ifdef WIFI
//! UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;

//! Wireless MQTT connection
WiFiClient client;

#else
//! UDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

//! MQTT cable connection
EthernetClient client;

#endif

//! IP Address of NTP server
IPAddress ntpServer(ntpServerIP);

//! NTP client
NTPClient ntp(ntpServer);

// MQTT broker
//! MQTT username (for authentication on MQTT broker
const char mqtt_username[] PROGMEM = MQTT_USERNAME;

//! MQTT username (for authentication on MQTT broker
const char mqtt_password[] PROGMEM = MQTT_PASSWORD;

//! Flash table for wireless configuration
const char* const mqtt_table[] PROGMEM =
{   
  mqtt_username,
  mqtt_password,
};

//! Port number of MQTT broker. Default number is 1883
uint16_t serverPort = 1883;

#ifdef SDP_SERVER_AS_IP
//! IP address of MQTT broker

byte serverIP[4] = MQTT_SERVER_IP;

//! domain of MQTT broker
sdp::client::SDPServer *MQTTserver = NULL;

#else
//! domain of MQTT broker
char *serverDomain = MQTT_SERVER_DOMAIN;

//! domain of MQTT broker
sdp::client::SDPServer *MQTTserver = NULL;
#endif



// Smart object property
//! GPS position of the node (and sensors)
GPSPosition location;

//! Input sensor
sdp::sensor::AnalogSensor sensor(SENSOR_ANALOG_INPUT_PIN);

//! Data stream
sdp::client::SDPStream stream;

//! SDP source
sdp::client::SDPSource *MTTQClient= NULL;

//! Smart object state
uint8_t state = IDLE;

//! Tenant
const char tenant[] PROGMEM = TENANT;

//! Sensor Identifier
const char sensor_id[] PROGMEM = SMRTOBJ_ID;

//! Stream Identifier
const char stream_id[] PROGMEM = STREAM_IDENTIFIER;

//! Flash table for wireless configuration
const char* const node_table[] PROGMEM =
{
  tenant,
  sensor_id,
  stream_id,
};


//! configuration directory
const char cfg_dir[] PROGMEM = CFG_DIRECTORY;

//! Wireless network password
const char time_fn[] PROGMEM = SEC_TIME_FN;

//! Flash table for wireless configuration
const char* const configuration_table[] PROGMEM =
{   
  cfg_dir,
  time_fn,
};


uint8_t hmacKey[HMAC_KEYWORD_LENGTH] = HMAC_KEYWORD;


//! NTP update period
Interval tNtpUpdateTime;

//! Read sensor period
Interval tReadTime; //(READ_SENSOR_PERIOD + 1);

// Read interval (in ms)
unsigned long tRead = 0;

// NTP update  interval (in ms)
unsigned long tNtpUpdate = 0;

//! Node Error
bool SDstate = true;


/*------------------------------------------------------------------------------------
/* FUNCTION PROTOTYPEs
/*------------------------------------------------------------------------------------*/

/**
 * setNetworkConfiguration()
 * Sets the network configuration (if static or dynamic). This function use global 
 * variables and initializes the NetworkConfig object (netConfig).
 *
 * \return none.
 */
void setNetworkConfiguration();

/**
 * startNetwork()
 * Starts the network. This function use global variables. WiFi networks are handled
 * using WiFiHandler object (wifiHandler), Ethernet object handles cable connections.
 *
 * \return none.
 */
void startNetwork();


/**
 * createNTPConnection()
 * Opens NTP connection.
 *
 * \return none.
 */
void createNTPConnection();


/**
 * ntpdate()
 * Sets local date and time using NTP protocol.

 *
 * \return true if error, false otherwise
 */
bool ntpupdate();

/**
 * generateRandMeasurement()
 * Generates a random measurement between n1 and n2 with decimal precision.
 *
 * \param n1 lower value in the range
 * \param n2 lower value in the range
 * \param decimal number of decimals (precision)
 *
 * \return random value between n1 and n2 as a float
 */
float generateRandMeasurement(int n1, int n2, int decimal);

/**
 * convertLM35()
 * Converts a voltage in temperature for a sensor LM35
 *
 * \param n1 value voltage output of the sensor LM35
 *
 * \return temperature in Celsius degrees
 */
float convertLM35(float value);


/**
 * setGPSPosition()
 * Sets a GPS location to given latitude, longitude and altitude.
 *
 * \param coordinate GPS location to set up
 * \param latitude latitude coordinate of the GPS location
 * \param longitude longitude coordinate of the GPS location
 * \param altitude altitude coordinate of the GPS location
 *
 * \return error code: GPS_INVALID_LATITUDE, GPS_INVALID_LONGITUDE, GPS_INVALID_LATITUDE if position is not valid. NO_ERROR otherwise.
 */
uint8_t setGPSPosition(GPSPosition &coordinate, char* latitude, char* longitude, char* altitude);

/**
 * printNetworkConfiguration()
 * Prints network configuration: current IP address, netmask, ...
 *
 */
void printNetworkConfiguration();

/**
 * printServerConfiguration()
 * Prints server MQTT configuration
 *
 * \param server MQTT server
 */
void printServerConfiguration(sdp::client::SDPServer &server);

/**
 * printStream()
 * Prints stream informtions
 *
 * \param stream stream
 */
void printStream(sdp::client::SDPStream &stream);

/**
 * printSensor()
 * Prints all sensor properties
 *
 * \param sensor sensor
 */
void printSensor(sdp::sensor::AnalogSensor &sensor);

/**
 * printPosition()
 * Prints a location (as a GPS position)
 *
 * \param coordinate GPS location
 */
void printPosition(GPSPosition &coordinate);

/**
 * getFlashString
 * Reads a word from flash and returns it as a string, using a global buffer ( flashBuffer ).
 */
char* getFlashString(PROGMEM const char *string_table[], byte index);

/**
 * openSD
 * Opens SD card.
 *
 * \param[in] board type of board (Mega, Uno, ...)
 * \param[in] shield type of shield (Ethernet, Wifi,...)
 *
 * \return true: opened, false: error while opening SD
 */
bool openSD(uint8_t board, uint8_t shield);

/**
 * checkControlMsg
 * Checks if there i sa new control message to process (received from the broker MQTT).
 *
 * \return true: it is received a new configuration, false: no new configuration
 */
bool checkControlMsg();

/**
 * loadConfig
 * Loads configuration from SD card, if configuration file is not present, it will be use the default values.
 *
 */
void loadConfig();

/**
 * defaultConfig
 * Loads default onfiguration.
 *
 */
void defaultConfig();




/*------------------------------------------------------------------------------------
/* Setup
/*------------------------------------------------------------------------------------*/
void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  Serial.println( F("Start node") );
  
  Serial.print( F("Open SD card...") );
  if ( !openSD(ARDUINO_MEGA, SHIELD_WIFI) )
  {
     Serial.println( F(" failed!") );
  }
  else
  {
    Serial.println( F(" done.") );    
  }

#ifdef CONTROL
  Serial.println( );
  Serial.println( F("Load Configuration...") );
  loadConfig();
#else
  defaultConfig();
#endif

  Serial.println( );
  Serial.println( F("Configure smart object...") );
  
  // Set up SDP structures (sensor, stream and node position)
  sensor.setID(/*getFlashString((const char**) node_table, NODE_ID)*/ "LM35");
  stream.setID(getFlashString((const char**) node_table, STREAM_ID));
  
  stream.setSensor(sensor);
  printStream(stream);

  int errCode = NO_ERROR;
  
  if ( ( errCode = setGPSPosition(location, GPS_LATITUDE, GPS_LONGITUDE, GPS_ALTITUDE)) != NO_ERROR )
  {
    Serial.print(F("Error: invalid GPS "));
    switch (errCode)
    {
      case GPS_INVALID_LATITUDE  : { Serial.print(F("latitude"));} break;
      case GPS_INVALID_LONGITUDE : { Serial.print(F("longitude"));} break;
      case GPS_INVALID_ALTITUDE  : { Serial.print(F("altitude"));} break;
    }
  }
  else
  {
    printPosition(location);
  }


  // Set network configuration (static or DHCP)
  setNetworkConfiguration();
  
  // Start network
  startNetwork();

  // Create UDP istance to communicate with NTP server
  createNTPConnection();

  
  // Create connection with the SPD server
#ifdef SDP_SERVER_AS_IP  
  MQTTserver = new sdp::client::SDPServer(serverIP, serverPort);
#else
  MQTTserver = new sdp::client::SDPServer(serverDomain, serverPort);
#endif

  MTTQClient = new sdp::client::SDPSource(*MQTTserver, client, getFlashString((const char**) node_table, NODE_ID));
  
  MTTQClient->setUsername(getFlashString((const char**) mqtt_table, MQTT_USER));
  MTTQClient->setPassword(getFlashString((const char**) mqtt_table, MQTT_PASS));
 
#ifdef SECURE_JSON
  MTTQClient->setHMACKey(hmacKey, HMAC_KEYWORD_LENGTH);
#endif
 //MTTQClient->connect();

  
  state = NTPUPDATE;
}




/*------------------------------------------------------------------------------------
/* Loop
/*------------------------------------------------------------------------------------*/
void loop()
{
  // Handle node state
  switch(state)
  {
    // Update time using NTP protocol
    case NTPUPDATE : 
    {


      Serial.println( );
      Serial.println( F("Updating time...") );
      tNtpUpdateTime.update(); 
      if ( ntpupdate() )
      {
        Serial.println( F("done") );
      }
      else
      {
        Serial.println( F("Failed") );
      }
      state = IDLE;
    } break;
    
     // Read sensor and publish measurement
    case READ_SENSORS : 
    {
      tReadTime.update(); 

      float value = sensor.measure();
/*        
      Serial.print("read: ");
      Serial.print(sensor.digital());
      Serial.print("  ");
      Serial.println(value);
*/  
      // Generate a random value for tests
      //float measure = generateRandMeasurement(13, 30, 10);
               
      // Convert measure fot LM35 temperature sensor
      float measure = convertLM35(value);

      // Index of GPS coordinate, it is used to know which coordinate is in wrong format 
      byte coordIndex = 0;
        
      // Measurement
      Measure m;
      m.setValue(measure);
      m.setTimestamp(now());
      m.setPosition(location, coordIndex);
  
      Serial.print( stream.id() );
      Serial.print( ";" );
      Serial.print( m.timestamp(0) );
      Serial.print( ";" );
      Serial.println( m.value() );

      // Check if client is connected with the server
      if (MTTQClient->isConnected())
      {
        Serial.print ( F("Sending measurement... ") );

        // Publish measurement
        int n = 0;
        if ( n = (MTTQClient->publish(stream, m, getFlashString((const char**) node_table, NODE_TENANT))) )
        {
         Serial.println( F("done") );
        }
        else
        {
          Serial.println( F("Failed") );
          MTTQClient->disconnect();
          Serial.println( F("SDP broker disconnected!") );
        }

        // Check if the data transmission failed
        if (!n)
        {
#ifdef WIFI
          // Reset wifi 
          Serial.println( F("Reconnecting wifi...") );
          wifiHandler.refresh();
#endif      
        }
      }
      else
      {
        Serial.println( F("SDP no connection") );
        if ( MTTQClient->connect() )
        {
#ifdef CONTROL      
          if ( (MTTQClient->subscribe(getFlashString((const char**) node_table, NODE_TENANT), stream, sensor)) )
          {}
#endif
          Serial.println( F("SDP connected") );
        }
        else
        {
          Serial.println( F("Connection failed") );
        }
      }
  
#ifdef DEBUG_MEMORY
Serial.print( F("freeMemory()=") );
Serial.println(freeMemory());
#endif
      
      state = IDLE;
    } break;

    case IDLE :
    {
      // Do nothing and check the following state
      if (tReadTime.residualTime(tRead) == 0)
      {
        state = READ_SENSORS;
      }
      else if (tNtpUpdateTime.residualTime(tNtpUpdate) == 0)
      {
        state = NTPUPDATE;
      }
      MTTQClient->loop();
      
#ifdef CONTROL  
      if ( checkControlMsg() )
      {
        sdp::message::SDPCtrlMsg msg;
        msg.initialize(sdp::client::SDPSource::getControlMsg());
        sdp::client::SDPSource::deleteControlMsg();
        
        msg.dump(Serial);
        Serial.println();
        
        sdp::message::SDPCtrlMsgHandler ctrlHandler;
        if ( ctrlHandler.isBroadcast(msg) || ctrlHandler.isToProcess( msg, (char*) MTTQClient->id()) )
        {
          switch (ctrlHandler.getType( msg ))
          {
            case 0 : { Serial.println(F("INVALID!")); }; break;
            case 1 : { Serial.println(F("UPDATE")); }; break;
            case 2 : { Serial.println(F("RESTART")); }; break;
            case 3 : { Serial.println(F("ENABLE")); }; break;
            case 4 : { Serial.println(F("FACTORY")); }; break;
            case 5 : { Serial.println(F("STATUS")); }; break;
            case 6 : { Serial.println(F("TEST")); }; break;
            case 7 : { Serial.println(F("CALIBRATE")); }; break;
            case 8 : { Serial.println(F("SYNC")); }; break;
            case 9 : { Serial.println(F("RCONFIG")); }; break;
            case 10 : { 
              Serial.println(F("WCONFIG")); 
              aJsonObject* data = msg.dataObj();
              if ( data != 0 )
              {
                Serial.print( "Data field: " ); 
                Serial.println( data->valuestring ); 
                aJsonObject* sec = aJson.getObjectItem(msg.dataObj(), "sec");
                aJsonObject* csv = aJson.getObjectItem(msg.dataObj(), "csv");
                if (sec != 0)
                  Serial.println( sec->valuestring ); 
                if (csv != 0)
                {
                  Serial.println( csv->valuestring );  
                  sdp::message::CSVLine configuration;
                  configuration.set((const char*) csv->valuestring, strlen(csv->valuestring));
                  sdp::client::SDPSource::saveConfiguration((char*) getFlashString((const char**) configuration_table, TIME_FN), configuration);
                }

              }
            }; break;
            case 11 : { 
              Serial.println(F("CONFIG")); 
              loadConfig();
            }; break;
            case 12 : { Serial.println(F("CMD")); }; break;
            case 13 : { Serial.println(F("HISTORY")); }; break;
            default : { Serial.println(F("INVALID!"));}; break;
          }
        }
        else
        {
          Serial.println("It isn't for me!");
        }
        
        Serial.print( F("freeMemory()=") );
        Serial.println(freeMemory());

      }
#endif

    } break;
    
    default : 
    {
      state = IDLE;
    }
  }
  
  delay(10);
}



/*------------------------------------------------------------------------------------
 * Functions
/*------------------------------------------------------------------------------------*/

void setNetworkConfiguration()
{
#ifdef STATIC_NETWORK_CFG
  byte ip[4]   = ARDUINO_IP_ADDRESS;
  byte mask[4] = NETWORK_SUBNET ;
  byte gw[4]   = NETWORK_GATEWAY;
  byte dns[4]  = NETWORK_DNS;
  
  netConfig.setDHCP(false);
  netConfig.setIPAddress(ip);
  netConfig.setNetmask(mask);
  netConfig.setGateway(gw);
  netConfig.setDNS(dns);

/*
  Serial.print("IP: ");
  Serial.println(IPAddress(netConfig.ip()));
  Serial.print("Mask: ");
  Serial.println(IPAddress(netConfig.netmask()));
  Serial.print("GW:  ");
  Serial.println(IPAddress(netConfig.gateway()));
*/

#else
  netConfig.setDHCP(true);
#endif

  if ( !netConfig.dhcp())
  {
#ifdef WIFI
    wifiHandler.config(netConfig);
#else
#endif
  }
}


void startNetwork()
{
  // Start connection
  Serial.println( );
  
#ifdef WIFI
  switch (type)
  {
    case 0 : { wifiHandler.setType(WiFiManager::WL_OPEN); }; break;
    case 1 : { wifiHandler.setType(WiFiManager::WL_WEP); }; break;
    case 2 : { wifiHandler.setType(WiFiManager::WL_WPA);}; break;
    default : { wifiHandler.setType(WiFiManager::WL_OPEN); }; break;
  }
  wifiHandler.setSSID(getFlashString((const char**) wireless_table, W_SSID));
  wifiHandler.setKey(getFlashString((const char**) wireless_table, W_KEY));

/*
  Serial.print("SSID: ");
  Serial.println(wifiHandler.ssid());
  Serial.print("Pass: ");
  Serial.println(wifiHandler.key());
  Serial.print("type: ");
  Serial.println(wifiHandler.type());
*/

  Serial.print( F("Attempting to connect to SSID: ") );
  Serial.println(wifiHandler.ssid());
  if( !wifiHandler.connect() )
  {
    Serial.println( F("Error: no wireless connection!") );
    for ( ; ; )
    {
      delay(5000);
    }
  }

  Serial.println( F("Connected to wifi") );
  printWifiStatus();
#else
  Serial.println( F("Attempting to connect to cable connecton") );
  if ( netConfig.dhcp() )
  {
    // DHCP
   if (Ethernet.begin(mac) == 0) 
    {
      Serial.println(F ("Failed to configure Ethernet using DHCP") );
      // no point in carrying on, so do nothing forevermore:
      for (;;)
      { 
        delay(5000);
      }
    }
  }
  else
  {
    Ethernet.begin(mac, netConfig.ip(), netConfig.dns(), netConfig.gateway(), netConfig.netmask());
  }

#endif
}

void createNTPConnection()
{
  Udp.begin(localPort);
  ntp.setUDP(&Udp);
}


bool ntpupdate()
{
  ntp.send();
  // wait to see if a reply is available
  delay(3000);
  
  if ( ntp.receive() )
  {
    return true;
  }
 return false;
}

float generateRandMeasurement(int n1, int n2, int decimal)
{
  // Generate a random value for tests
  long lower = n1 * decimal;
  long upper = n2 * decimal;
  float measure = random(lower,upper) / decimal;
/*  
  char msg[10] = {0};
  dtostrf(measure, 4, 2, msg);*/ 
  return measure;
}        

float convertLM35(float value)
{
  return value * 100;;
}        


void printNetworkConfiguration()
{
  Serial.println( );
  Serial.println(F("Network configuration: "));
#ifdef STATIC_NETWORK_CFG
  Serial.print( F("\tIP Address: ") );
  Serial.println(IPAddress( netConfig.ip() ));  
  Serial.print( F("\tNetmask   : ") );
  Serial.println(IPAddress( netConfig.netmask() ));  
  Serial.print( F("\tGateway   : ") );
  Serial.println(IPAddress( netConfig.gateway() ));  
  Serial.print( F("\tDNS       : ") );
  Serial.println(IPAddress( netConfig.dns() ));
#else
  Serial.print( F("\tIP Address: ") );
  
  #ifdef WIFI

  Serial.println(IPAddress(WiFi.localIP()));

  #else
  
  Serial.println(IPAddress(Ethernet.localIP()));
  
  #endif

#endif
}

void printServerConfiguration(sdp::client::SDPServer &server)
{
  Serial.println( );
  Serial.println( F("Server configuration: ") );
  if (server.isDefinedByIP())
  {
    Serial.print( F("\tIP Address: ") );
    Serial.println(IPAddress(server.ip()));  
  }
  else
  {
    Serial.print( F("\tDomain: ") );
    Serial.println(server.domain());  
  }
  Serial.print( F("\tPort: ") );
  Serial.println(server.port());  
}

void printPosition(GPSPosition &coordinate)
{
  Serial.println( F("GPS position: ") );
  Serial.print( F("\tlatitude:  ") );
  Serial.println(coordinate.latitude());  
  Serial.print( F("\tlongitude: ") );
  Serial.println(coordinate.longitude());  
  Serial.print( F("\taltitude:  ") );
  Serial.println(coordinate.altitude());  
}

void printSensor(sdp::sensor::AnalogSensor &sensor)
{
  Serial.println( F("Sensor:") );
  Serial.print( F("\tIdentifier:") );
  Serial.println(sensor.id());
  Serial.print( F("\tInput pin :") );
  Serial.println(sensor.pin());
}


void printStream(sdp::client::SDPStream &stream)
{
  Serial.println( F("Stream:" ) );
  Serial.print( F("\tIdentifier:") );
  Serial.println(stream.id());
  sdp::sensor::AnalogSensor *pSensor = (sdp::sensor::AnalogSensor*) stream.sensor();
  sdp::sensor::AnalogSensor &sensor = (*pSensor);
  printSensor(sensor);
  /*
  Serial.println(sensor.id());
  Serial.print(F("\tSensor Input pin :"));
  Serial.println(sensor.pin());
  */
}

uint8_t setGPSPosition(GPSPosition &coordinate, char* latitude, char* longitude, char* altitude)
{
  int errGPS = 0;
  int errCode = 0;
  if ((errGPS = coordinate.setLatitude(latitude)) < 0)
  {
    Serial.print( F("Error: invalid GPS latitude (") );
    Serial.print(errGPS);
    Serial.print( F(")") );
    errCode = GPS_INVALID_LATITUDE;
  } 
  else
  if ((errGPS = coordinate.setLongitude(longitude)) < 0)
  {
    Serial.print( F("Error: invalid GPS longitude (")) ;
    Serial.print(errGPS);
    Serial.print( F(")") );
    errCode = GPS_INVALID_LONGITUDE;
  }
  else
  if ((errGPS = coordinate.setAltitude(altitude)) < 0)
  {
    Serial.print( F("Error: invalid GPS altitude (") );
    Serial.print(errGPS);
    Serial.print( F(")") );
    errCode = GPS_INVALID_ALTITUDE;
  }
  
  return errCode;

}

#ifdef WIFI
void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print( F("SSID: ") );
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print( F("IP Address: ") );
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print(F ("signal strength (RSSI):") );
  Serial.print(rssi);
  Serial.println( F(" dBm") );
}
#endif

char* getFlashString(PROGMEM const char *string_table[], byte index)
{
  memset(flashBuffer, 0, FLASH_BUFFER_SIZE);
  strcpy_P(flashBuffer, (char*) pgm_read_word(&(string_table[index])));
  return flashBuffer;
}

bool openSD(uint8_t board, uint8_t shield)
{

  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output
  // or the SD library functions will not work.
  switch(board)
  {
    case ARDUINO_MEGA    : { pinMode(53, OUTPUT); }; break;
    case GENERIC_ARDUINO : { pinMode(10, OUTPUT); }; break;
    default              : { pinMode(10, OUTPUT); }; break;
  }
  
  uint8_t m_cspin = 4; 
  switch(shield)
  {
    case SHIELD_WIFI :
    case SHIELD_ETHERNET : {
      m_cspin = 4;
    } break;
    case SHIELD_SPARKFUN : {
      m_cspin = 8;
    } break;
    case SHIELD_ADAFRUIT : {
      m_cspin = 10;
    } break;
    default: {
      m_cspin = 4;
    } break;
  }

  if (!SD.begin(m_cspin)) {
    return false;
  }
  
  return true;
}

void defaultConfig()
{
  Serial.println( F("Default values") );
  tRead = READ_SENSOR_PERIOD;
  tNtpUpdate = UPDATE_TIME_PERIOD;
}

#ifdef CONTROL
bool checkControlMsg()
{
  return (sdp::client::SDPSource::getControlMsg() != 0);
}



#define RBUF_SIZE 128
const uint8_t cfgNF = 2;

void loadConfig()
{
  if (SD.exists((char*) getFlashString((const char**) configuration_table, TIME_FN)))
  {
//    Serial.println( F("Load from SD") );

    char rBuffer[RBUF_SIZE] = {0};
    size_t i = 0;

    sdp::message::CSVLine conf;
    if (sdp::client::SDPSource::loadConfiguration((char*) getFlashString((const char**) configuration_table, TIME_FN), conf))
    {
      //check number of fields
      if ( conf.NF() == cfgNF)
      {
        for (i = 0; i < cfgNF; i++)
        {
          switch (i)
          {
            case 0 : {
              int n = 0;
              
              if ( conf.getItemAsInt(i, n) )
              {
                tRead = ( (unsigned long) n) * 1000;
              }
              else
              {
                tRead = READ_SENSOR_PERIOD;
              }
              
              Serial.print( F("\ttRead: ") );
              Serial.println(tRead);
              
            } break;
            
            case 1 : {
              int n = 0;
              
              if ( conf.getItemAsInt(i, n) )
              {
                tNtpUpdate = ( (unsigned long) n) * 1000;
              }
              else
              {
                tNtpUpdate = UPDATE_TIME_PERIOD;
              }
              
              Serial.print( F("\ttNtpUpdate: ") );
              Serial.println(tNtpUpdate);
            } break;
            default : {} break;
            
          }
        }
        
        return ;
      }
      else
      {
        Serial.println( F("Invalid config file!") ); 
      }
      // close the file:
    }
    // if the file didn't open, print an error:
      Serial.println( F("Error opening") );
  }
  else
  {
  }
  defaultConfig();
}

#endif
