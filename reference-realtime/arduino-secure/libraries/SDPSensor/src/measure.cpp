/*
 * measure.cpp
 *         Arduino Library for modeling a generic measurement.
 *
 * Authors:
 *         Marco Boeris Frusca
 *
 */
#include "measure.h"
#include "stringparser.h"

using namespace sdp::sensor;

Measure::Measure() :
    m_value(0), m_timestamp(0.0), m_quality(VALID)

{
  memset(m_buffer, 0, BUFFER_SIZE);

}

Measure::Measure(const Measure &m)
{
  m_value = m.m_value;
  m_timestamp = m.m_timestamp;
  memcpy(m_buffer, m.m_buffer, BUFFER_SIZE);
  m_position = m.m_position;
  m_quality = m.m_quality;
}

Measure::~Measure()
{
}

const char* Measure::timestamp(byte format)
{
  switch (format)
  {
    case DEFAULT_TS:
    {
      StringParser::convertTimeISO8601(m_timestamp, m_buffer, BUFFER_SIZE);
/*
      snprintf(m_buffer, BUFFER_SIZE, "%04d-%02d-%02dT%02d:%02d:%02dZ",
          year(m_timestamp), month(m_timestamp), day(m_timestamp),
          hour(m_timestamp), minute(m_timestamp), second(m_timestamp));
*/
    }
      break;
    case IOT:
    {
      snprintf(m_buffer, BUFFER_SIZE, "%02d-%02d-%04dT%02d:%02dZ",
          month(m_timestamp), day(m_timestamp), year(m_timestamp),
          hour(m_timestamp), minute(m_timestamp));
    }
      break;
    default:
    {
    }
      break;
  }

  return m_buffer;
}

int Measure::setPosition(GPSPosition &pos, byte &coord)
{
  coord = 1;

  int err = m_position.setLatitude(pos.latitude());

  if (err == 1)
  {

    coord = 2;
    err = m_position.setLongitude(pos.longitude());

    if (err == 1)
    {

      coord = 3;
      err = m_position.setAltitude(pos.altitude());

    }
  }

  if (err == 1)
  {
    coord = 0;
  }
  return err;
}

int Measure::setPosition(char* latitude, char* longitude, char* altitude,
    byte &coord)
{
  coord = 1;

  int err = m_position.setLatitude(latitude);

  if (err == 1)
  {

    coord = 2;
    err = m_position.setLongitude(longitude);

    if (err == 1)
    {

      coord = 3;
      err = m_position.setAltitude(altitude);

    }
  }

  if (err == 1)
  {
    coord = 0;
  }
  return err;
}

void Measure::setQuality(uint8_t quality)
{
  switch (quality)
  {
    case VALID:
    case ERRONEOUS:
    case DOUBTFUL:
    case UNKNOWN:
    {
      m_quality = quality;
    }
      ;
      break;
    default:
    {
      m_quality = UNKNOWN;
    }
  };

}
