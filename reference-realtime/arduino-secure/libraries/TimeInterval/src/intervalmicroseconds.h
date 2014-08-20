/**
 * \file intervalmicroseconds.h
 * \brief Arduino Library to handle time interval (in microseconds).
 *
 * \author Marco Boeris Frusca
 *
 */
#ifndef INTERVALMICROSECONDS_H_
#define INTERVALMICROSECONDS_H_

#include "Arduino.h"
#include "interval.h"

/**
 * The IntervalMicroSeconds class handles time intervals (in microseconds). This class inheritances from
 * #Interval class.
 */
class IntervalMicroSeconds: public Interval
{
  public:
    /**
     * Default Constructor
     */
    IntervalMicroSeconds();

    /**
     * Constructor
     * It sets the start time at a given value.
     *
     * \param[in] start start time
     */
    IntervalMicroSeconds(unsigned long start);

    /**
     * Copy Constructor
     */
    IntervalMicroSeconds(const IntervalMicroSeconds& s);

    /**
     * Destructor
     */
    virtual ~IntervalMicroSeconds();

    /**
     * Resets the start time to now().
     *
     */
    void reset(unsigned long start = 0);

    /**
     * Updates the start time at current time.
     *
     */
    virtual void update()
    {
      m_start = micros();
    }
    ;

    /**
     * Returns the start time (in microseconds).
     *
     * \return start time
     */
    unsigned long start()
    {
      return m_start;
    }
    ;

    /**
     * Returns the time passed from the start time (in microseconds).
     *
     * \return  time passed
     */
    virtual unsigned long time();

    /**
     *  Overload operator =
     *
     */
    IntervalMicroSeconds& operator=(const IntervalMicroSeconds&);

};

#endif /* INTERVALMICROSECONDS_H_ */
