/*
 * genericsensor.cpp
 *         Arduino Library for a generic sensor.
 * 
 * Authors:
 *         Marco Boeris Frusca
 * 
 */

#include "genericsensor.h"

// Default constructor
GenericSensor::GenericSensor() :
    m_tWarm(0), m_coefficient(1.0), m_offset(0.0)
{
  memset(m_identifier, 0, ID_SIZE);
}

// Copy constructor
GenericSensor::GenericSensor(const GenericSensor &s)
{
  m_tWarm = s.m_tWarm;
  m_coefficient = s.m_coefficient;
  m_offset = s.m_offset;
  setID((const char*) s.m_identifier);
}

// Default destructor
GenericSensor::~GenericSensor()
{
}

void GenericSensor::setID(const char* id)
{
  uint8_t len = strlen(id);
  memset(m_identifier, 0, ID_SIZE);
  memcpy(m_identifier, id, (len > ID_SIZE) ? ID_SIZE : len);

}

GenericSensor & GenericSensor::operator=(const GenericSensor &s)
{
  m_tWarm = s.m_tWarm;
  m_coefficient = s.m_coefficient;
  m_offset = s.m_offset;

  return (*this);
}

