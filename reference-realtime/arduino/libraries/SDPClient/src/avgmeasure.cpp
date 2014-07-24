/*
 * avgmeasure.cpp
 *         Arduino library for calculating a cumulative moving average for a measurement.
 *
 * Authors:
 *         Marco Boeris Frusca
 *
 */

#include "avgmeasure.h"

AvgMeasure::AvgMeasure() :
    m_i(0), m_value(0.0)
{
}

AvgMeasure::~AvgMeasure()
{
}

float AvgMeasure::update(float value)
{
  m_value += (value - m_value) / (++m_i);

  return m_value;
}
