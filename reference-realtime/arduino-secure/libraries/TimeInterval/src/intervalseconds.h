/**
 * \file intervalmicroseconds.h
 * \brief Arduino library to handle time interval using second as unit.
 *
 * \author Marco Boeris Frusca
 *
 */
#ifndef INTERVALSECONDS_H_
#define INTERVALSECONDS_H_

#include "Arduino.h"
#include "interval.h"
#include <Time.h>

/**
 * The IntervalSeconds class handles time intervals (in seconds). This class inheritances from
 * #Interval class.
 */
class IntervalSeconds: public Interval
{
  public:
    /**
     * \enum   _size
     *
     * Size of internal buffers
     */
    enum _size
    {
      //! Internal buffer size
      SIZE_BUFFER = 20,
    };

    /**
     * \enum   _time_format
     *
     * Time format
     */
    enum _time_format
    {
      //! Default format: DD/MM/YYYY hh:mm:ss
      DEFAULT_TF = 0,
      //! English format: MM/DD/YYYY hh:mm:ss
      EN = 1,
      //! Default format: MM/DD/YYYY hh:mm
      IOT = 2,
    };

    /**
     * Default Constructor
     * It sets all internal variables as the start time at current time as seconds from 1970.
     */
    IntervalSeconds();

    /**
     * Constructor
     * It sets the start time at a given value.
     *
     * \param[in] start start time
     */
    IntervalSeconds(unsigned long start);

    /**
     * Copy Constructor
     */
    IntervalSeconds(const IntervalSeconds& s);

    /**
     * Destructor
     */
    virtual ~IntervalSeconds();

    /**
     * Resets the start time to now().
     *
     * \param[in] start start time
     *
     */
    void reset(unsigned long start = 0);

    /**
     * Updates the start time at current time.
     *
     */
    virtual void update();

    /**
     * Returns the start time (in seconds from 1970).
     *
     * \return start time
     */
    unsigned long start();

    /**
     * Returns the time passed from the start time (in seconds).
     *
     * \return  time passed
     */
    virtual unsigned long time();

    /**
     * Gets time in readable form as a string.
     *
     * \param[in] format format time accorting to #_time_format
     *
     * \return time as a string
     */
    char* getTimeAsString(byte format = DEFAULT_TF);

    /**
     *  Overload operator =
     *
     */
    IntervalSeconds& operator=(const IntervalSeconds&);

  private:
    //! Internal buffer. It is used to get time in readable form
    char m_buffer[SIZE_BUFFER];

};

inline void IntervalSeconds::update()
{
  m_start = now();
}

inline unsigned long IntervalSeconds::start()
{
  return m_start;
}

#endif /* INTERVALSECONDS_H_ */
