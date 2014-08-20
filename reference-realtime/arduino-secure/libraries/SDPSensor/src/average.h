/**
 * \file avgmeasure.h
 * \brief Arduino library for calculating a cumulative moving average for a measurement.
 *
 * \author Marco Boeris Frusca
 *
 */
#ifndef AVGMEASURE_H_
#define AVGMEASURE_H_

#include "Arduino.h"

namespace sdp
{
  namespace sensor
  {

    /**
     * The CumulativeAvg calculates a cumulative average for a measurement as a float value.
     * Using "update" function the average is updated adding a new value. To calculate
     * the Cumulative Moving Average (CMA) is calculate using the following formula:
     * CMA(n+1) =  CMA(n) + [ X(n+1) - CMA(n) ] / (n + 1)
     *
     */
    class Average
    {
        //! Index of cumulative average
        unsigned int m_i;

        //! Average value
        float m_value;

      public:
        /**
         * Default Constructor
         */
        Average();

        /**
         * Destructor
         */
        virtual ~Average();

        /**
         * Resets cumulative average. It puts to 0 the average values and the numbers of values (elements)
         * ​​used to calculate the average.
         *
         */
        void reset()
        {
          m_i = 0, m_value = 0.0;
        }
        ;

        /**
         * Gets average value
         *
         * \return current average value
         */
        float value() const
        {
          return m_value;
        }
        ;

        /**
         * Checks if average is valid (not 0 elements)
         *
         * \return true if is valid, false otherwise
         */
        bool isValid() const
        {
          return (m_i > 0);
        }
        ;

        /**
         * Updates the cumulative moving average adding a new value.
         *
         * \param value value to add to the cumulative moving average
         *
         * \return updated average
         */
        float update(float value);

    };
  }
}
#endif /* AVGMEASURE_H_ */
