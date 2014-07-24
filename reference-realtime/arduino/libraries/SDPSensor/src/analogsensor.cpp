/*
 * analogsensor.cpp
 *         Arduino library for reading an analog sensor.
 * 
 * Authors:
 *         Marco Boeris Frusca
 * 
 */

#include "Arduino.h"
#include "analogsensor.h"

// default constructor
AnalogSensor::AnalogSensor() :
    m_pin(0), m_digit(0)
{
  m_power = VDD;
  m_vbit = m_power / 1024;

  analogReference (DEFAULT);
}

// constructor
AnalogSensor::AnalogSensor(int pin, float Vpower) :
    m_pin(pin), m_digit(0)
{
  m_power = Vpower;
  m_vbit = m_power / 1024;

  analogReference (DEFAULT);
}

// copy constructor
AnalogSensor::AnalogSensor(const AnalogSensor &s) :
    GenericSensor(s)
{
  m_pin = s.m_pin;
  m_digit = s.m_digit;
  m_power = s.m_power;
  m_vbit = s.m_vbit;
}

// default constructor
AnalogSensor::~AnalogSensor()
{
}

AnalogSensor & AnalogSensor::operator=(const AnalogSensor &s)
{
  GenericSensor::operator=(s);

  m_pin = s.m_pin;
  m_digit = s.m_digit;
  m_power = s.m_power;
  m_vbit = s.m_vbit;

  return (*this);
}

float AnalogSensor::measure()
{
  float tmpResult;

  m_digit = analogRead(m_pin);
  tmpResult = (float) m_digit * m_vbit;

  tmpResult = (m_coefficient * tmpResult) + m_offset;
  return tmpResult;
}

float AnalogSensor::measureAvg(unsigned int n)
{
  // Cumulative average
  unsigned int i = 0;
  float average = 0;

  for (i = 0; i < n; i++)
  {
    // CA(i+1) = CA(i) + ( x(i+1) - CA(i) ) / ( i + 1 )
    m_digit = analogRead(m_pin);
    average += (m_digit - average) / (i + 1);
  }

  average *= (float) (m_vbit);
  return average;
}

void AnalogSensor::calibrate()
{
  return;
}
