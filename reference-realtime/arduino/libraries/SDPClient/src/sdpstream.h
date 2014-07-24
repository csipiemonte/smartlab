/**
 * \file sdpstream.h
 * \brief Arduino library for modeling a SDP data stream.
 *
 * \author Marco Boeris Frusca
 *
 */
#ifndef SDPSTREAM_H_
#define SDPSTREAM_H_

#include "Arduino.h"
#include "genericsensor.h"

/**
 * The SDPStream class describes a Smart Data stream. It is defined by an identifier and a sensor (as data
 * source). It is possible sets a flag to define if the stream is referred at a mobile sensor or not.
 *
 * \note At the moment this library does not support the publishing of measurements from  moving object.
 * source.
 *
 */
class SDPStream
{
  public:

    //! size of the identifier filed
    static const byte ID_SIZE = 128;

    /**
     * Default Constructor
     *
     */
    SDPStream();

    /**
     * Copy Constructor
     */
    SDPStream(const SDPStream &s);

    /**
     * Destructor
     */
    virtual ~SDPStream();

    /**
     * Sets the stream identifier. This is a string of ID_SIZE characters.
     *
     * \param[in] id stream identifier.
     */
    virtual void setID(const char* id);

    /**
     * Gets the stream identifier.
     *
     * \return stream identifier
     */
    virtual const char* id() const
    {
      return (const char*) m_identifier;
    }
    ;

    /**
     * Sets the sensor associated to the stream.
     *
     * \param[in] sensor sensor associated at the stream.
     */
    virtual void setSensor(GenericSensor &sensor)
    {
      this->m_sensor = &sensor;
    }
    ;

    /**
     * Gets the pointer to the sensor associated to the stream
     *
     * \return stream identifier
     */
    virtual GenericSensor* sensor() const
    {
      return m_sensor;
    }
    ;

    /**
     * Sets the property "mobile" of the stream.
     *
     * \param[in] mobile property of the stream: true if is a moving object, false if it a fixed one.
     */
    virtual void setMobile(bool mobile)
    {
      m_mobile = mobile;
    }
    ;

    /**
     * Returns true if the stream is associated to a moving object
     *
     * \return true if the stream is associated to a moving object, false otherwise
     */
    virtual bool mobile() const
    {
      return m_mobile;
    }
    ;

  private:
    //! Sensor identifier
    char* m_identifier[ID_SIZE + 1];

    //! Sensor associated to the stream
    GenericSensor* m_sensor;

    //! Mobile property of the stream. true if the source is a mobile object, false otherwise
    bool m_mobile;
};

#endif /* SDPSTREAM_H_ */
