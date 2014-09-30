/*
 * average.cpp
 *         Arduino library for calculating a cumulative moving average for a measurement.
 *
 * Authors:
 *         Marco Boeris Frusca
 *
 */

#include "average.h"

using namespace sdp::sensor;

Average::Average() :
    m_i(0), m_value(0.0)
{
}

Average::~Average()
{
}

float Average::update(float value)
{
  m_value += (value - m_value) / (++m_i);

  return m_value;
}
