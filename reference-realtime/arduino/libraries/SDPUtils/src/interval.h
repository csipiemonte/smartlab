/**
 * \file interval.h
 * \brief Arduino Library to handle time interval (in milliseconds).
 *
 * \author Marco Boeris Frusca
 *
 */
#ifndef INTERVAL_H_
#define INTERVAL_H_

#include "Arduino.h"

/**
 * The Interval class handles time interval (in milliseconds). An interval has a time start,
 * and calling time() function you can get the time passed from starting time.
 * Using residualTime( T ) function you can get the time to wait before time T is passed.
 */
class Interval
{
  public:
    /**
     * Default Constructor
     * It sets all internal variables as the start time at current time.
     */
    Interval();

    /**
     * Constructor
     * It sets the start time at a given value.
     *
     * \param[in] start start time
     */
    Interval(unsigned long start);

    /**
     * Copy Constructor
     */
    Interval(const Interval& s);

    /**
     * Destructor
     */
    virtual ~Interval();

    /**
     * Updates the start time at current time.
     *
     */
    void update();

    /**
     * Returns the start time (in milliseconds).
     *
     * \return start time
     */
    unsigned long start();

    /**
     * Resets the start time to now().
     *
     * \param[in] start new starting time
     */
    void reset(unsigned long start = 0);

    /**
     * Returns the time passed from the start time (in milliseconds).
     *
     * \return time passed
     */
    unsigned long time();

    /**
     * Returns the remaining time calculated as endTime - m_start (in milliseconds).
     *
     * \param[in] endTime end time of an interval
     *
     * \return residual time
     */
    unsigned long residualTime(unsigned long endTime);

    /**
     *  Overload operator =
     *
     */
    Interval& operator=(const Interval&);

  protected:
    //! start time (in milliseconds)
    unsigned long m_start;

};

inline unsigned long Interval::start()
{
  return m_start;
}

inline void Interval::update()
{
  m_start = millis();
}

#endif /* INTERVAL_H_ */
