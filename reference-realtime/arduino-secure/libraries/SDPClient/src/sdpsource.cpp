/*
 * sdpsource.cpp
 *         Arduino library for modeling a SDP data source.
 *
 * Authors:
 *         Marco Boeris Frusca
 *
 */
#include "sdpsource.h"
#include <WiFi.h>
#include <stringparser.h>
#include <sdppublishmsg.h>
/*
#include <MemoryFree.h>
*/

using namespace sdp::client;

const char SDPSource::CONN_DIRECTION_I[] PROGMEM = "input";
const char SDPSource::CONN_DIRECTION_O[] PROGMEM = "output";
const char SDPSource::ATTR_CONTROL[] PROGMEM = "control";

PGM_P const SDPSource::SOURCE_TABLE_P[] PROGMEM =
{
    SDPSource::CONN_DIRECTION_I,
    SDPSource::CONN_DIRECTION_O,
    SDPSource::ATTR_CONTROL,
};


const char* SDPSource::DEFAULT_ID = "ARDUINO";

const char* SDPSource::DEFAULT_COMPONENT_LABEL = "c0";
const char SDPSource::CONC_CHAR = '/';

char* SDPSource::CONTROL_MSG = 0;
//sdp::message::CSVLine* SDPSource::configuration = 0;
const uint16_t SDPSource::RBUF_SIZE = 128;

SDPSource::SDPSource() :
    m_isCopy(false), m_subclient(0), m_message(0), m_hmacKeylength(0), m_id(0), m_username(0), m_password(0)
{
  m_server = 0;
  m_netclient = 0;

  // Set source ID
  size_t maxSize = strlen(DEFAULT_ID) + 1;
  StringParser::initBuffer(m_id, (char*) DEFAULT_ID, maxSize);

  // Set username and password
  maxSize = 2;
  StringParser::initBuffer(m_username, (char*) "", maxSize);
  StringParser::initBuffer(m_password, (char*) "", maxSize);
  //memset(m_username, 0, USERNAME_SIZE);
  //memset(m_password, 0, PASSWORD_SIZE);

  memset(m_hmacKey, 0, HMAC_KEY_SIZE);

/*
//  memset(m_message, 0, MSG_SIZE);
  memset(m_id, 0, ID_SIZE);
  memcpy(m_id, DEFAULT_ID, strlen(DEFAULT_ID));
*/
}

SDPSource::SDPSource(SDPServer& server, Client& client, const char* id) :
    m_isCopy(false), m_subclient(0), m_message(0), m_hmacKeylength(0), m_id(0), m_username(0), m_password(0)
{
  this->m_server = &server;
  this->m_netclient = &client;

  // Set source ID
  size_t maxSize = strlen(id) + 1;
  StringParser::initBuffer(m_id, (char*) id, maxSize);

  // Set username and password
  maxSize = 2;
  StringParser::initBuffer(m_username, (char*) "", maxSize);
  StringParser::initBuffer(m_password, (char*) "", maxSize);
  //memset(m_username, 0, USERNAME_SIZE);
  //memset(m_password, 0, PASSWORD_SIZE);

  memset(m_hmacKey, 0, HMAC_KEY_SIZE);
/*
  //memset(m_message, 0, MSG_SIZE);
  memset(m_id, 0, ID_SIZE);
  memcpy(m_id, id, strlen(id));
*/
}

SDPSource::SDPSource(const SDPSource& s)
{
  m_server = s.m_server;
  m_netclient = s.m_netclient;
  m_isCopy = true;
  m_subclient = s.m_subclient;

  m_message = s.m_message;
  m_hmacKeylength = s.m_hmacKeylength;

  m_id = s.m_id;
  m_username = s.m_username;
  m_password = s.m_password;

  //memcpy(m_username, s.m_username, USERNAME_SIZE);
  //memcpy(m_password, s.m_password, PASSWORD_SIZE);
  memcpy(m_hmacKey, s.m_hmacKey, HMAC_KEY_SIZE);

/*
  memset(m_message, 0, MSG_SIZE);
  memcpy(m_message, s.m_message, MSG_SIZE);
*/
}

SDPSource::~SDPSource()
{
  if (!m_isCopy && m_subclient != 0)
  {
    delete (m_subclient);
    StringParser::delBuffer(m_id);
    StringParser::delBuffer(m_username);
    StringParser::delBuffer(m_password);
  }
}

bool SDPSource::setId(char * id)
{
  return StringParser::initBuffer(m_id, (char*) id, ID_SIZE);
}
bool SDPSource::setUsername(char * user)
{
  return StringParser::initBuffer(m_username, (char*) user, USERNAME_SIZE);
}

bool SDPSource::setPassword(char * pass)
{
  return StringParser::initBuffer(m_password, (char*) pass, PASSWORD_SIZE);
}
;

void SDPSource::setHMACKey(uint8_t * key, size_t length)
{
  memset(m_hmacKey, 0, HMAC_KEY_SIZE);
  if (length > HMAC_KEY_SIZE)
  {
    length = HMAC_KEY_SIZE;
  }
  memcpy(m_hmacKey, key, length);
  m_hmacKeylength = length;

}

int8_t SDPSource::connect()
{
  if (m_subclient == 0)
  {
    if (m_server->isDefinedByIP())
    {
      m_subclient = new PubSubClient((uint8_t*) m_server->ip(),
          m_server->port(), SDPSource::callback, *(this->m_netclient));
    } else
    {
      m_subclient = new PubSubClient((char*) m_server->domain(),
          m_server->port(), SDPSource::callback, *(this->m_netclient));
    }
  } else
  {
  }
  if (strlen(m_username))
  {
//    Serial.println("authentication");

    return m_subclient->connect(m_id, m_username, m_password);
  }

//  Serial.println("no authentication");
  return m_subclient->connect(m_id);
}

int8_t SDPSource::disconnect()
{
  if (m_netclient->connected())
  {
    while (m_netclient->available())
    {
      char c = m_netclient->read();
    }
  }

  m_subclient->disconnect();

  return 1;
}

uint8_t SDPSource::publish(char* topic, char* msg)
{
  uint8_t n = 0;

  n = m_subclient->publish(topic, msg);

  return n;
}

bool SDPSource::createTopic(char* direction, char* tenant, char* attribute, char* buffer, unsigned int bSize)
{
  size_t lenDirection = strlen(direction);
  size_t lenTenant = strlen(tenant);
  size_t lenAttrib = (attribute != 0) ?strlen(attribute):0;

  memset(buffer, 0, bSize);

  if ((lenDirection + lenTenant + lenAttrib + 3) >= bSize)
  {
    return false;
  }

// Create topic
  size_t cIndex = 0;

  memcpy(&buffer[cIndex], direction, lenDirection);

  cIndex += lenDirection;
  buffer[cIndex++] = CONC_CHAR;

  memcpy(&buffer[cIndex], tenant, lenTenant);
  cIndex += lenTenant;

  if (lenAttrib > 0)
  {
    buffer[cIndex++] = CONC_CHAR;
    memcpy(&buffer[cIndex], attribute, lenAttrib);
  }

  return true;

}

uint8_t SDPSource::sendStatus(const char* tenant, unsigned long time, unsigned long lastTS, unsigned long nextTS, char* version, char* status, char* data, bool enable)
{
#define TOPIC_SIZE 128

  char topic[TOPIC_SIZE] = { 0 };

  char connO[10] = {0};
  StringParser::getFlashString((const char**) SDPSource::SOURCE_TABLE_P, 0, &connO[0], 10);

  char attrContrl[10] = {0};
  StringParser::getFlashString((const char**) SDPSource::SOURCE_TABLE_P, 2, &attrContrl[0], 10);

  if ( !createTopic((char*) connO, (char*) tenant, (char*) attrContrl, topic, (unsigned int) TOPIC_SIZE) )
  {
    return 0;
  }
  Serial.println(topic);

  char buffer[25] = {0};

  sdp::message::JSON json;
  json.add("id", m_id);
  StringParser::convertTimeISO8601(time, buffer);
  json.add("time", buffer);
  StringParser::convertTimeISO8601(lastTS, buffer);
  json.add("last", buffer);
  StringParser::convertTimeISO8601(nextTS, buffer);
  json.add("next", buffer);


  StringParser::delBuffer(m_message);
  m_message = aJson.print(json.getJson());


  uint8_t n = 0;
  if (m_message != 0)
  {
    Serial.print( F("json: ") );
    Serial.println(m_message);
    n = m_subclient->publish(topic, m_message);
  }

  if (m_message == 0)
  {
    //Serial.println("return 0");
    return 0;
  }

  StringParser::delBuffer(m_message);

  return n;

}

uint8_t SDPSource::publish(SDPStream& stream, Measure& measure,
    const char* tenant)
{

  char topic[TOPIC_SIZE] = { 0 };

  GenericSensor *pSensor = (GenericSensor*) stream.sensor();
  GenericSensor &sensor = (*pSensor);

  char connI[10] = {0};
  StringParser::getFlashString((const char**) SDPSource::SOURCE_TABLE_P, 0, &connI[0], 10);

  if ( !createTopic(connI, (char*) tenant, (char*) 0, topic, (unsigned int) TOPIC_SIZE) )
  {
    return 0;
  }

  // Create JSON
  sdp::message::PublishJSON json(stream.id(), m_id);

  int validity = 0;
  switch (measure.quality())
  {
    case sdp::sensor::Measure::VALID : { validity = sdp::message::ValueJSON::VALID; }; break;
    case sdp::sensor::Measure::ERRONEOUS : { validity = sdp::message::ValueJSON::ERRONEOUS; }; break;
    case sdp::sensor::Measure::DOUBTFUL : { validity = sdp::message::ValueJSON::DOUBTFUL; }; break;
    case sdp::sensor::Measure::UNKNOWN : { validity = sdp::message::ValueJSON::UNKNOWN; }; break;
    default : { validity = sdp::message::ValueJSON::UNKNOWN;}
  }

  sdp::message::ValueJSON value(measure.timestamp(0), validity);

  // Add components
  if ( !value.addComponent(DEFAULT_COMPONENT_LABEL, measure.value()) )
  {
    return 0;
  }
  json.addValue(value);

  if (m_message != 0)
  {
    free(m_message);
    m_message = 0;
  }
//  m_message = aJson.print(json.getJson());

  if(m_hmacKeylength > 0)
  {
    sdp::message::SecureJSON secjson;
    secjson.setKey(m_hmacKey, m_hmacKeylength);
    secjson.create(json);

    // Publish _JSON
    m_message = aJson.print(secjson.getJson());
  }
  else
  {
    m_message = aJson.print(json.getJson());
  }


  uint8_t n = 0;
  if (m_message != 0)
  {
    Serial.print( F("json: ") );
    Serial.println(m_message);
    n = m_subclient->publish(topic, m_message);
  }

  if (m_message != 0)
  {
    //Serial.println("free m_message");
    free(m_message);
    m_message = 0;
  }
  else
  {
    //Serial.println("return 0");
    return 0;
  }

  return n;
}

uint8_t SDPSource::publish(SmartObjStatus & status, const char* tenant)
{
#define TOPIC_SIZE 128

  char topic[TOPIC_SIZE] = { 0 };

  return 1;
}

uint8_t SDPSource::subscribe(const char* tenant, SDPStream& stream, GenericSensor& sensor)
{

  char topic[TOPIC_SIZE] = { 0 };

  char connO[10] = {0};
  StringParser::getFlashString((const char**) SDPSource::SOURCE_TABLE_P, 1, &connO[0], 10);

  char attrContrl[10] = {0};
  StringParser::getFlashString((const char**) SDPSource::SOURCE_TABLE_P, 2, &attrContrl[0], 10);

  if ( !createTopic((char*) connO, (char*) tenant, (char*) attrContrl, topic, (unsigned int) TOPIC_SIZE) )
  {
    return 0;
  }
  Serial.println(topic);
  if (m_subclient->subscribe(topic))
  {
    //Serial.println(topic);
    return 1;
  }
  return 0;
}

void SDPSource::callback(char* topic, byte* payload, unsigned int length)
{
  // In order to republish this payload, a copy must be made
  // as the orignal payload buffer will be overwritten whilst
  // constructing the PUBLISH packet.

  // Free the memory old buffer
  deleteControlMsg();

  // Copy the payload to the new buffer
  CONTROL_MSG = new char[length + 1];

  if(CONTROL_MSG != 0)
  {
    memcpy(CONTROL_MSG, payload, length);
    CONTROL_MSG[length] = 0;
  }
  //SDPSource::configuration->set((const char*) payload, length);
}

bool SDPSource::loadConfiguration(char* filename, sdp::message::CSVLine &conf)
{
  char rBuffer[RBUF_SIZE];
  File myFile = SD.open(filename, FILE_READ);
  memset(rBuffer, 0, sizeof(char) * RBUF_SIZE);

  // if the file opened okay, read it and get only the first line
  if (myFile)
  {
    size_t i = 0;
    bool endline = false;
    while (myFile.available() && !endline)
    {
      rBuffer[i] = myFile.read();
      if (rBuffer[i] == '\n' || i == RBUF_SIZE)
      {
        endline = true;
      }
      i++;
    }
    myFile.close();
  }
  else
  {
    return false;
  }
/*
  Serial.print("Conf: ");
  Serial.println(rBuffer);
*/
  conf.set(rBuffer, RBUF_SIZE);

  return true;
}

bool SDPSource::saveConfiguration(char* filename, sdp::message::CSVLine &conf)
{
  // Save new configuration
  //    Serial.println( F("Save on SD") );
  if (SD.exists(filename))
  {
    // Delete old configuration
    SD.remove(filename);
  }

  File myFile = SD.open(filename, FILE_WRITE);
  // if the file opened okay, write to it:
  if (myFile)
  {
    for (size_t i = 0; i < conf.NF(); i++)
    {

      //Serial.println(conf.getItem(i));
      myFile.print(conf.getItem(i));
      myFile.print(sdp::message::CSVLine::FS);
    }
    // close the file:
    myFile.close();
    //      Serial.println();
  } else
  {
    // if the file didn't open, print an error:
    return false;
  }
  return true;

}
