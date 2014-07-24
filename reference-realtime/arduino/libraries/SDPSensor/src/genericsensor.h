/**
 * \file genericsensor.h
 * \brief Arduino Library for readinf a generic sensor. This class define the interfaces of a generic sensor.
 *
 * \author Marco Boeris Frusca
 *
 */
#ifndef GenericSensor_H
#define GenericSensor_H

#include "Arduino.h"

/**
 * The GenericSensor class defines the standard interface of a sensor. This is a virtual class and defines
 * a virtual method "measure" to read the sensor. It is possible calibrate the sensor through a linear
 * correction using the following parameters: coefficient and offset.
 */
class GenericSensor
{
  public:

    //! Size of the identifier filed
    static const byte ID_SIZE = 128;

    /**
     * Default Constructor
     *
     */
    GenericSensor();

    /**
     * Copy Constructor
     */
    GenericSensor(const GenericSensor &s);

    /**
     * Destructor
     */
    virtual ~GenericSensor();

    /**
     * Override operator =
     */
    GenericSensor & operator=(const GenericSensor &s);

    /**
     * Sets the warm up time. If it is 0, the sensor can be read immediately, without waiting time
     * after it is powered.
     *
     * \param[in] timeWU warm up time in milliseconds.
     */
    virtual void setTWarm(unsigned long timeWU)
    {
      m_tWarm = timeWU;
    }
    ;

    /**
     * Gets the warm up time.
     *
     * \return warm up time in milliseconds.
     */
    virtual unsigned long tWarm()
    {
      return m_tWarm;
    }
    ;

    /**
     * Sets the sensor identifier. This is a string of ID_SIZE characters.
     *
     * \param[in] id sensor identifier.
     */
    virtual void setID(const char* id);

    /**
     * Gets the sensor identifier.
     *
     * \return sensor identifier
     */
    virtual const char* id() const
    {
      return (const char*) m_identifier;
    }
    ;

    /**
     * Sets coefficient parameter. This value can be used to calibrate the sensor.
     *
     * \param[in] coeff multiplier factor.
     */
    void setCoefficient(float coeff)
    {
      m_coefficient = coeff;
    }
    ;

    /**
     * Gets coefficient parameter.
     *
     * \return coefficient parameter.
     */
    unsigned long coefficient()
    {
      return m_coefficient;
    }
    ;

    /**
     * Sets offset parameter. This value can be used to calibrate the sensor.
     *
     * \param[in] offset offset parameter.
     */
    void setOffset(float offset)
    {
      m_offset = offset;
    }
    ;

    /**
     * Gets offset parameter.
     *
     * \return offset parameter.
     */
    unsigned long offset()
    {
      return m_offset;
    }
    ;

    /**
     * Reads sensor and gets the current measurement.
     *
     * \return value of the measurement
     *
     */
    virtual float measure() = 0;

  protected:
    //! Sensor identifier
    char* m_identifier[ID_SIZE + 1];

    //! Warm up time (in millis). Time to wait for having a valid value.
    unsigned long m_tWarm;

    //! Conversion factor (multiplier)
    float m_coefficient;

    //! Conversion factor (offset)
    float m_offset;
};

#endif /* GenericSensor_H */
