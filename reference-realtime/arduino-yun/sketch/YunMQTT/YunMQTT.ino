/**
 * YunMQTT
 * Sends a JSON message to Yucca server.
 * This sketch requires the following libraries:
 * - aJson: https://github.com/interactive-matter/aJson
 * - PubSubClient fork: https://github.com/csipiemonte/pubsubclient
 * - MemortFree (for debug): http://playground.arduino.cc/code/AvailableMemory
 * 
 * created 27 Jun 2015
 * modified 27 Jun 2015
 * by Marco Boeris Frusca
 */
//#define MEMORY_DEBUG 1

#include <Bridge.h>
#include <YunClient.h>

#include <aJSON.h>
#include <PubSubClient.h>

#ifdef MEMORY_DEBUG
#include <MemoryFree.h>
#endif

// Update these with values suitable for your network.
char * server = "stream.smartdatanet.it";

// MQTT username
char * userid = "sandbox";
// MQTT password
char * psw = "sandbox$1";
// MQTT Topic
char * topic = "input/sandbox";
// MQTT client identifier
char * clientID = "yunTEST";


//! Stream
char* stream = "flussoProva";
// Sensor identifier
char* id = "6e6fdf33-e8f0-4ee8-ba35-386de213e722";
//char* json =  "{\"stream\":\"flussoProva\",\"sensor\":\"6e6fdf33-e8f0-4ee8-ba35-386de213e722\",\"values\":[{\"time\":\"2014-05-13T17:08:58+0200\",\"components\":{\"valoreDouble\":\"17.4\",\"valoreString\":\"51\"}}]}";

// buffer to store the timestamp
char timestamp[20] = {0};

// Yun network client
YunClient yunClient;

// MQTT client
PubSubClient client(server, 1883, callback, yunClient);

/******************************************
 * Functions
 ******************************************/
/**
 * Creates a new timestamp using the format:
 * YYYY-MM-DDT%hh:mm:ssZ
 *
 * \return pointer to buffer where timestamp is stored.
 */
char* getTimeStamp();

/**
 * Creates a new Yucca message. This is an example and the JSON is constant.
 * This is the message generates:
 *
 * {
 *   "stream":"flussoProva",
 *   "sensor":"6e6fdf33-e8f0-4ee8-ba35-386de213e722",
 *   "values":[
 *    {
 *      "time":"2015-01-27T16:48:11Z",
 *      "components":
 *        {
 *          "valoreDouble":"17.4",
 *          "valoreString":"50"
 *        }
 *    }
 *  ]
 * }
 *
 * You can modify the stream name and sensor id changing the global variables:
 * - stream
 * - id
 * To customize your message you can edit this function.
 * NOTE:
 * This function allocate a new object in the heap memory, remember to delete it calling  aJson.deleteItem function!
 *
 * \return pointer to JSON message.
 */
aJsonObject* createMsg();

/**
 * Deletes a JSON message.
 *
 * \param msg message to delete
 */
void deleteMsg(aJsonObject* msg);

/**
 * Handles message arrived from the MQTT broker.
 *
 * \param topic topic
 * \param payload message buffer
 * \param length number of byte received
 */
void callback(char* topic, byte* payload, unsigned int length) {


/******************************************
 * Setup
 ******************************************/
void setup()
{
  Bridge.begin();
  // Open Serial monitor
  Serial.begin(115200);
  Serial.println(F("Start!"));
  
  // Cpnnect to MQTT broker
  if (client.connect(clientID, userid, psw)) {
    Serial.println(F("Connected!"));
  }
  else
  {
    Serial.println(F("Not connected!"));
  }
}

/******************************************
 * Loop
 ******************************************/
void loop()
{
  // Create message
  aJsonObject* msg = createMsg();
  
  if ( msg != NULL )
  {
    char* string = aJson.print(msg);
    Serial.print(F("json: "));
    Serial.println(string);
    
    client.publish(topic, string);
    free(string);
    
    // Delete message
    deleteMsg(msg);
  }
#ifdef MEMORY_DEBUG
  Serial.print( F("freeMemory()=") );
  Serial.println(freeMemory());
#endif
  delay(3000);
}


/******************************************
 * Functions
 ******************************************/
char* getTimeStamp() 
{
  String result;
  memset(timestamp, 0, 20);
  Process time;
  time.runShellCommand("date -u +\"%Y-%m-%dT%H:%M:%SZ\"");

  while (time.available() > 0) {
    char c = time.read();
    if (c != '\n')
      result += c;
  }

  memcpy( timestamp, result.c_str(), strlen(result.c_str()) );
  timestamp[strlen(result.c_str())] = 0;
  return timestamp;
}

aJsonObject* createMsg()
{        
  // Get timestamp
  char* ts = getTimeStamp();
  
  // Create JSON
  aJsonObject* root = aJson.createObject();
  
  if (root != NULL) 
  {
    // Stream
    aJson.addStringToObject(root, "stream",stream);
    // Sensor Identifier
    aJson.addStringToObject(root, "sensor", id);
    
    // Value array
    aJsonObject *values = aJson.createArray();
    
    if (values != NULL )
    {
      aJson.addItemToObject(root, "values", values);
      
      // Measurement
      aJsonObject* value = aJson.createObject();
      if (value != NULL)
      {
        // Add Measurement to values array
        aJson.addItemToArray(values, value);
        
        // Timestamp
        aJson.addStringToObject(value, "time", ts);
        
        // Measurement components
        aJsonObject* compontents = aJson.createObject();
        
        if ( compontents != NULL )        
        {
          // Add components to mwasuremnt
          aJson.addItemToObject(value, "components", compontents);
          
          // Component 1
          aJson.addStringToObject(compontents, "valoreDouble", "17.4");
          // Component 2
          aJson.addStringToObject(compontents, "valoreString", "50");
          
#ifdef MEMORY_DEBUG
          Serial.print( F("Max freeMemory()=") );
          Serial.println(freeMemory());
#endif

        }
        else
        {
          Serial.println(F("OBJECT_COMPENTENTS_FAILED"));
        }
      }
      else
      {
        Serial.println(F("OBJECT_VALUE_FAILED"));
      }

    }
    else
    {
      Serial.println(F("OBJECT_VALUES_FAILED"));
    }   
  } 
  else {
    Serial.println(F("OBJECT_ROOT_FAILED"));
  }
  return root;
}

void deleteMsg(aJsonObject* msg)
{
  if (msg != NULL)
  {
     aJson.deleteItem(msg); 
     msg = 0;  
  }
}


void callback(char* topic, byte* payload, unsigned int length) 
{
  // handle message arrived
}

