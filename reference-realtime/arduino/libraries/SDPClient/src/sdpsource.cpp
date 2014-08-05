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

const char* SDPSource::DEFAULT_ID = "SDP_ARDUINO_CLIENT";
const char* SDPSource::OPEN_CURLY_BRACKET = "{";
const char* SDPSource::OPEN_SQUARE_BRACKET = "[";
const char* SDPSource::CLOSE_CURLY_BRACKET = "}";
const char* SDPSource::CLOSE_SQUARE_BRACKET = "]";
const char* SDPSource::SLASH = "\"";
const char* SDPSource::LABEL_STREAM = "\"stream\":\"";
const char* SDPSource::LABEL_SENSOR = "\"sensor\":\"";
const char* SDPSource::LABEL_VALUE = "\"values\":";
const char* SDPSource::LABEL_TIME = "\"time\":\"";
const char* SDPSource::LABEL_COMPONENTS = "\"components\":";
const char* SDPSource::LABEL_COMPONENT_0 = "\"c0\":\"";
const char* SDPSource::SEPARATOR = "\",";

const char* SDPSource::CONNECTION_TYPE = "input";
const char* SDPSource::DEFAULT_TENANT = "smartlab";

const char SDPSource::CONC_CHAR = '/';

SDPSource::SDPSource() :
    m_isCopy(false), m_subclient(NULL)
{
  m_server = NULL;
  m_netclient = NULL;

  memset(m_username, 0, USERNAME_SIZE);
  memset(m_password, 0, PASSWORD_SIZE);

  memset(m_message, 0, MSG_SIZE);
  memset(m_id, 0, ID_SIZE);
  memcpy(m_id, DEFAULT_ID, strlen(DEFAULT_ID));
}

SDPSource::SDPSource(SDPServer& server, Client& client, const char* id) :
    m_isCopy(false), m_subclient(NULL)
{
  this->m_server = &server;
  this->m_netclient = &client;

  memset(m_username, 0, USERNAME_SIZE);
  memset(m_password, 0, PASSWORD_SIZE);

  memset(m_message, 0, MSG_SIZE);
  memset(m_id, 0, ID_SIZE);
  memcpy(m_id, id, strlen(id));
}

SDPSource::SDPSource(const SDPSource& s)
{
  m_server = s.m_server;
  m_netclient = s.m_netclient;
  m_isCopy = true;
  m_subclient = s.m_subclient;
  memset(m_message, 0, MSG_SIZE);
  memcpy(m_message, s.m_message, MSG_SIZE);
}

SDPSource::~SDPSource()
{
  if (!m_isCopy && m_subclient != NULL)
  {
    delete (m_subclient);
  }
}

void SDPSource::setId(char * id)
{
  memset(m_id, 0, ID_SIZE);
  memcpy(m_id, id, strlen(id));

}

int8_t SDPSource::connect()
{
  if (m_subclient == NULL)
  {
    if (m_server->isDefinedByIP())
    {
      m_subclient = new PubSubClient((uint8_t*) m_server->ip(),
          m_server->port(), SDPSource::callback, *(this->m_netclient));
    } else
    {
      m_subclient = new PubSubClient((uint8_t*) m_server->domain(),
          m_server->port(), SDPSource::callback, *(this->m_netclient));
    }
  } else
  {
  }
  if (strlen(m_username))
  {
    Serial.println("authentication");
    return m_subclient->connect( m_id, m_username, m_password);
  }

  Serial.println("no authentication");
  return m_subclient->connect( m_id );
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

uint8_t SDPSource::publish(SDPStream& stream, Measure& measure, const char* tenant)
{
  #define TOPIC_SIZE 128

  char topic[TOPIC_SIZE] = { 0 };

  GenericSensor *pSensor = (GenericSensor*) stream.sensor();
  GenericSensor &sensor = (*pSensor);

  size_t lenStreamId = strlen(stream.id());
  size_t lenSensorId = strlen(sensor.id());
  size_t lenType = strlen(CONNECTION_TYPE);
  size_t lenTenant = strlen(tenant);


  if ((lenType + lenTenant + lenStreamId + lenSensorId + 5) > TOPIC_SIZE)
  {
    return 0;
  }

  // Create topic
  size_t cIndex = 0;

  memcpy(&topic[cIndex], CONNECTION_TYPE, lenType);
  cIndex+=lenType;
  topic[cIndex++] = CONC_CHAR;

  memcpy(&topic[cIndex], tenant, lenTenant);
  cIndex+=lenTenant;
/*
  topic[cIndex++] = CONC_CHAR;
  memcpy(&topic[cIndex], sensor.id(), lenSensorId);
  cIndex+=lenSensorId;
  topic[cIndex++] = '_';
  memcpy(&topic[cIndex++], stream.id(), lenStreamId);
*/
  Serial.print("topic: ");
  Serial.println(topic);

  if (!createJSON(stream, measure))
  {
    return 0;
  }

  return publish(topic, m_message);
}

bool SDPSource::MsgAppend(const char* field, uint16_t &length)
{
  uint16_t l = strlen(field);

  if (length - l >= 0)
  {
    memcpy(&m_message[(MSG_SIZE - 1) - length], field, l);
    length -= l;
  } else
  {
    return false;
  }

  return true;
}

uint8_t SDPSource::createJSON(SDPStream& stream, Measure& measure)
{
  uint16_t len = MSG_SIZE - 1;
  GenericSensor *pSensor = (GenericSensor*) stream.sensor();
  GenericSensor &sensor = (*pSensor);

  // Reset message buffer
  memset(m_message, 0, MSG_SIZE);

  // Convert value from float to string
  char n[10] = { 0 };
  dtostrf(measure.value(), 6, 2, n);
  int i = 0;
  while (i < 9 && n[i] == ' ')
  {
    i++;
  }
  char *number = &n[i];

  // Create JSON
  if (!MsgAppend(OPEN_CURLY_BRACKET, len))
    return 0;

  if (!MsgAppend(LABEL_STREAM, len))
    return 0;
  if (!MsgAppend(stream.id(), len))
    return 0;
  if (!MsgAppend(SEPARATOR, len))
    return 0;
  if (!MsgAppend(LABEL_SENSOR, len))
    return 0;
  if (!MsgAppend(sensor.id(), len))
    return 0;
  if (!MsgAppend(SEPARATOR, len))
    return 0;
  if (!MsgAppend(LABEL_VALUE, len))
    return 0;

  if (!MsgAppend(OPEN_SQUARE_BRACKET, len))
    return 0;
  if (!MsgAppend(OPEN_CURLY_BRACKET, len))
    return 0;

  if (!MsgAppend(LABEL_TIME, len))
    return 0;
  if (!MsgAppend(measure.timestamp(0), len))
    return 0;
  if (!MsgAppend(SEPARATOR, len))
    return 0;

  if (!MsgAppend(LABEL_COMPONENTS, len))
    return 0;

  if (!MsgAppend(OPEN_CURLY_BRACKET, len))
    return 0;

  if (!MsgAppend(LABEL_COMPONENT_0, len))
    return 0;
  if (!MsgAppend(number, len))
    return 0;
  if (!MsgAppend(SLASH, len))
    return 0;

  if (!MsgAppend(CLOSE_CURLY_BRACKET, len))
    return 0;
  if (!MsgAppend(CLOSE_CURLY_BRACKET, len))
    return 0;
  if (!MsgAppend(CLOSE_SQUARE_BRACKET, len))
    return 0;

  if (!MsgAppend(CLOSE_CURLY_BRACKET, len))
    return 0;

  return len;
}

void SDPSource::callback(char* topic, byte* payload, unsigned int length)
{

}
