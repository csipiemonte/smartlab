/*
 * interval.h
 *         Arduino Library to handle time interval (in milliseconds).
 *
 * Authors:
 *         Marco Boeris Frusca
 *
 */

#include "interval.h"

Interval::Interval()
{
  m_start = millis();
}
Interval::Interval(unsigned long start)
{
  m_start = (m_start < 0) ? 0 : start;
}

Interval::Interval(const Interval& s)
{
  m_start = s.m_start;
}

Interval::~Interval()
{
}

Interval& Interval::operator=(const Interval& s)
{
  m_start = s.m_start;

  return *this;
}

void Interval::reset(unsigned long start)
{
  if (start != 0)
  {
    m_start = start;
  } else
  {
    m_start = millis();
  }
}

unsigned long Interval::time()
{
  unsigned long now = millis();
  unsigned long t = now - m_start;

  if (now < m_start)
  {
    // unsigned long is 4 Byte, so the maximum value is 0xFFFFFFFF
    // interval time is
    //   time before roll over (max unsigned long - m_start) +
    //    time after roll over (now)
    t = 0xFFFFFFFF - m_start;
    t += now;
  }
  return t;
}

unsigned long Interval::residualTime(unsigned long endTime)
{
  if (time() >= endTime)
  {
    return 0;
  }

  return (endTime - time());
}
