/*
 * differentialsensor.cpp
 *         Arduino library for reading a sensor that measures the difference between two analog inputs.
 *
 * Authors:
 *         Marco Boeris Frusca
 *
 */

#include "differentialsensor.h"

DifferentialSensor::DifferentialSensor() :
    m_primary(DEFAULT_PRIMARY_PIN), m_secondary(DEFAULT_SECONDARY_PIN)
{
}

DifferentialSensor::DifferentialSensor(int pin0, int pin1, float Vpower) :
    m_primary(pin0, Vpower), m_secondary(pin1, Vpower)
{
}

DifferentialSensor::DifferentialSensor(DifferentialSensor &s) :
    GenericSensor(s)
{
  m_primary = s.getPrimary();
  m_secondary = s.getSecondary();
}

DifferentialSensor::~DifferentialSensor()
{
}

float DifferentialSensor::measure()
{
  float measure = (m_coefficient * (m_primary.measure() - m_secondary.measure())
      + m_offset);

  return measure;
}

float DifferentialSensor::measureAvg(unsigned int n)
{
  // Cumulative average
  unsigned int i = 0;
  float average = 0;

  for (i = 0; i < n; i++)
  {
    // CA(i+1) = CA(i) + ( x(i+1) - CA(i) ) / ( i + 1 )
    average += (measure() - average) / (i + 1);
  }

  return average;
}

