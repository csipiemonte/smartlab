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

#include <sdppublishmsg.h>
/*
#include <MemoryFree.h>
*/

using namespace sdp::client;

const char* SDPSource::DEFAULT_ID = "ARDUINO";
const char* SDPSource::DEFAULT_TENANT = "smartlab";
const char* SDPSource::CONNECTION_TYPE = "input";
const char* SDPSource::CONNECTION_TYPE_1 = "output";
const char* SDPSource::CONNECTION_TYPE_2 = "control";
const char* SDPSource::DEFAULT_COMPONENT_LABEL = "c0";
const char SDPSource::CONC_CHAR = '/';

char* SDPSource::CONTROL_MSG = 0;
//sdp::message::CSVLine* SDPSource::configuration = 0;
const uint16_t SDPSource::RBUF_SIZE = 128;

SDPSource::SDPSource() :
    m_isCopy(false), m_subclient(0), m_message(0), m_hmacKeylength(0)
{
  m_server = 0;
  m_netclient = 0;

  memset(m_username, 0, USERNAME_SIZE);
  memset(m_password, 0, PASSWORD_SIZE);

  memset(m_hmacKey, 0, HMAC_KEY_SIZE);

//  memset(m_message, 0, MSG_SIZE);
  memset(m_id, 0, ID_SIZE);
  memcpy(m_id, DEFAULT_ID, strlen(DEFAULT_ID));
}

SDPSource::SDPSource(SDPServer& server, Client& client, const char* id) :
    m_isCopy(false), m_subclient(0), m_message(0), m_hmacKeylength(0)
{
  this->m_server = &server;
  this->m_netclient = &client;

  memset(m_username, 0, USERNAME_SIZE);
  memset(m_password, 0, PASSWORD_SIZE);
  memset(m_hmacKey, 0, HMAC_KEY_SIZE);

  //memset(m_message, 0, MSG_SIZE);
  memset(m_id, 0, ID_SIZE);
  memcpy(m_id, id, strlen(id));
}

SDPSource::SDPSource(const SDPSource& s)
{
  m_server = s.m_server;
  m_netclient = s.m_netclient;
  m_isCopy = true;
  m_subclient = s.m_subclient;

  m_message = s.m_message;
  m_hmacKeylength = s.m_hmacKeylength;

  memcpy(m_username, s.m_username, USERNAME_SIZE);
  memcpy(m_password, s.m_password, PASSWORD_SIZE);
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
  }
}

void SDPSource::setId(char * id)
{
  memset(m_id, 0, ID_SIZE);
  memcpy(m_id, id, strlen(id));

}

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

uint8_t SDPSource::publish(SDPStream& stream, Measure& measure,
    const char* tenant)
{
#define TOPIC_SIZE 128

  char topic[TOPIC_SIZE] = { 0 };

  GenericSensor *pSensor = (GenericSensor*) stream.sensor();
  GenericSensor &sensor = (*pSensor);

/*
  size_t lenStreamId = strlen(stream.id());
  size_t lenSensorId = strlen(sensor.id());
*/
  size_t lenType = strlen(CONNECTION_TYPE);
  size_t lenTenant = strlen(tenant);

  if ((lenType + lenTenant /*+ lenStreamId + lenSensorId*/ + 3/*5*/) > TOPIC_SIZE)
  {
    return 0;
  }

  // Create topic
  size_t cIndex = 0;

  memcpy(&topic[cIndex], CONNECTION_TYPE, lenType);
  cIndex += lenType;
  topic[cIndex++] = CONC_CHAR;

  memcpy(&topic[cIndex], tenant, lenTenant);
  cIndex += lenTenant;

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
    /*
     uint8_t hmacKey2[]={
     0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11,0x12,0x13,0x14,
     0x15,0x16,0x17,0x18,0x19
     };
     */
    secjson.setKey(m_hmacKey, m_hmacKeylength);
    secjson.create(json);

    // Publish _JSON
    m_message = aJson.print(secjson.getJson());
  }
  else
  {
    m_message = aJson.print(json.getJson());
  }

  Serial.print("json: ");
  Serial.println(m_message);

  uint8_t n = 0;
  if (m_message != 0)
  {
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

uint8_t SDPSource::subscribe(const char* tenant, SDPStream& stream, GenericSensor& sensor)
{

  char topic[TOPIC_SIZE] = { 0 };

  size_t lenType = strlen(CONNECTION_TYPE_1);
  size_t lenTenant = strlen(tenant);
  size_t lenStreamId = strlen(stream.id());
  size_t lenSensorId = strlen(sensor.id());

  if ((lenType + lenTenant + lenStreamId + lenSensorId + 5) > TOPIC_SIZE)
  {
    return 0;
  }

// Create topic
  size_t cIndex = 0;

  memcpy(&topic[cIndex], CONNECTION_TYPE_1, lenType);

  cIndex += lenType;
  topic[cIndex++] = CONC_CHAR;

  memcpy(&topic[cIndex], tenant, lenTenant);
  cIndex += lenTenant;

  topic[cIndex++] = CONC_CHAR;

  memcpy(&topic[cIndex], CONNECTION_TYPE_2, strlen(CONNECTION_TYPE_2));

  if (m_subclient->subscribe(topic))
  {
    Serial.println(topic);
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
