/**
 * \file differentialsensor.h
 * \brief Arduino library for reading a sensor that measures the difference between two analog inputs.
 *
 * \author Marco Boeris Frusca
 *
 */
#ifndef DIFFERENTIALSENSOR_H_
#define DIFFERENTIALSENSOR_H_

#include "Arduino.h"
#include "genericsensor.h"
#include "analogsensor.h"

/**
 *  The DifferentialSensor class implements the GenericSensor interface to model a analog differential
 *  sensor. This sensor has got a primary input and a secondary one. The measurement is the difference
 *  between primary and secondary input.
 *
 */
class DifferentialSensor: public GenericSensor
{
    //! Primary pin number
    AnalogSensor m_primary;

    //! Auxiliary Electrode pin number
    AnalogSensor m_secondary;

  public:
    /**
     * \enum    _differentialSensor_const
     *
     * Differential sensor constants.
     */
    enum _differentialSensor_const
    {
      //! Default pin number of the primary input
      DEFAULT_PRIMARY_PIN = 0,

      //! Default pin number of the secondary electrode
      DEFAULT_SECONDARY_PIN = 1,
    };

    /**
     * Default Constructor
     *
     */
    DifferentialSensor();

    /**
     * Constructor
     *
     * \param[in] pin0 primary pin number
     * \param[in] pin1 secondary pin number
     * \param[in] Vpower power supply
     */
    DifferentialSensor(int pin0, int pin1, float Vpower = AnalogSensor::VDD);

    /**
     * Copy Constructor
     *
     */
    DifferentialSensor(DifferentialSensor &s);

    /**
     * Destructor
     */
    virtual ~DifferentialSensor();

    /**
     * Gets reference to the primary sensor
     *
     * \return reference to the primary sensor
     */
    virtual AnalogSensor& getPrimary()
    {
      return m_primary;
    }
    ;

    /**
     * Gets reference to the secondary sensor
     *
     * \return reference to the secondary sensor
     */
    virtual AnalogSensor& getSecondary()
    {
      return m_secondary;
    }
    ;

    /**
     * Reads sensor and gets the current measurement.
     *
     * \return value of the measurement
     *
     */
    virtual float measure();

    /**
     * This function calculates the cumulative average value of n samples:
     *
     * \param[in] n number of samples
     *
     * \return measure as voltage
     */
    virtual float measureAvg(unsigned int n);

};

#endif /* DIFFERENTIALSENSOR_H_ */
