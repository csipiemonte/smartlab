/**
 * \file analogsensor.h
 * \brief Arduino library for reading an analog sensor.
 *
 * \author Marco Boeris Frusca
 *
 */
#ifndef ANALOGSENSOR_H
#define ANALOGSENSOR_H

#include "Arduino.h"
#include "genericsensor.h"

/**
 *  The AnalogSensor class implements the GenericSensor interface to model an analog sensor.
 *  An analog sensor can be connected to a analog input pin of the Arduino board. VDD is the power
 *  supply (default value 5.0V).
 *  Arduino uses 10 bits ADCs, so the smaller voltage readable (VDD / 1024) is 4.88 mV.
 */
class AnalogSensor: public GenericSensor
{
  public:
    //! Default power supply (5.0V)
    static const float VDD = 5.0;

    /**
     * Default Constructor
     * It sets all internal variables as the power supply is 5V
     */
    AnalogSensor();

    /**
     * Constructor
     * It sets all internal variables and power supply to VDD
     *
     * \param[in] pin analog input pin
     * \param[in] Vpower power supply
     */
    AnalogSensor(int pin, float Vpower = VDD);

    /**
     * Copy Constructor
     */
    AnalogSensor(const AnalogSensor &s);

    /**
     * Destructor
     */
    ~AnalogSensor();

    /**
     * Override operator =
     */
    AnalogSensor & operator=(const AnalogSensor &s);

    /**
     * Gets analog pin number
     *
     * \return analog pin number
     */
    int pin() const
    {
      return m_pin;
    }
    ;

    /**
     * Gets value of the input pin as a digital number
     *
     * \return value of the input pin
     */
    int digital();

    /**
     * Converts ADC counts into measurement.
     *
     * \return measure as voltage
     *
     */
    virtual float measure();

    /**
     * Converts ADC counts into measurement. This function calculates
     * the cumulative average value of n samples:
     *
     * \param[in] n number of samples
     *
     * \return measure as voltage
     */
    virtual float measureAvg(unsigned int n);

    /**
     * Calibrates the sensor. This function is empty but classes that inherit from AnalogicSensor can
     * implements ad-hoc methods.
     *
     */
    virtual void calibrate();

  protected:
    //! Number of the analog input pin
    int m_pin;

    //! Power supply
    float m_power;

    //! Multiply ADC counts by voltsPerBit
    float m_vbit;

    //! ADC value
    int m_digit;
};

inline int AnalogSensor::digital()
{
  return m_digit;
}

#endif /* ANALOGSENSOR_H */
