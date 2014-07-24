/*
 * intervalmicroseconds.cpp
 *         Arduino Library to handle time interval (in microseconds).
 *
 * Authors:
 *         Marco Boeris Frusca
 *
 */

#include "intervalmicroseconds.h"

IntervalMicroSeconds::IntervalMicroSeconds()
{
  m_start = micros();
}

IntervalMicroSeconds::IntervalMicroSeconds(unsigned long start)
{
  m_start = start;
}

IntervalMicroSeconds::IntervalMicroSeconds(const IntervalMicroSeconds& s)
{
  m_start = s.m_start;
}

IntervalMicroSeconds::~IntervalMicroSeconds()
{
}

IntervalMicroSeconds& IntervalMicroSeconds::operator=(
    const IntervalMicroSeconds& s)
{
  m_start = s.m_start;

  return *this;
}

unsigned long IntervalMicroSeconds::time()
{
  unsigned long _now = micros();
  return (_now - m_start);
}

void IntervalMicroSeconds::reset(unsigned long start)
{
  if (start != 0)
  {
    m_start = start;
  } else
  {
    m_start = micros();
  }
}
