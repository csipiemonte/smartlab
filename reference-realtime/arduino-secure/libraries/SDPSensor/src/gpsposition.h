/**
 * \file gpsposition.h
 * \brief Arduino library for modeling a position as GPS location.
 *
 * \author Marco Boeris Frusca
 *
 */
#ifndef GPSPOSITION_H_
#define GPSPOSITION_H_

#include "Arduino.h"

namespace sdp
{
  namespace sensor
  {

    /**
     * The class GPSPosition models a GPS location. A GPS location (position) has three component: latitude,
     * longitude and altitude. Latitude and longitude are in decimal degree format (where
     * positive value means North for latitude and West for longitude).
     * Altitude value is an integer and its unit is meter.
     *
     */
    class GPSPosition
    {
      public:
        /**
         * \enum    _gps_error
         *
         * Error code used to signal an invalid coordinate
         */
        enum _gps_error
        {
          /// No errors
          NO_ERROR = 0,
          //! Invalid coordinate length
          INVALID_COORD_LEN = -1,

          //! Invalid format of the coordinate
          INVALID_COORD_STRING_FORMAT = -2,

          //! Invalid value of the coordinate
          INVALID_COORD_VALUE = -3,
        };

        /**
         * \enum    _gps_attributes
         *
         * Length of the GPS class attributes (internal buffer of chars)
         */
        enum _gps_attributes
        {
          //! Length of internal buffer where coordinate (latitude or longitude) is stored
          COODINATE_LENGTH = 0x12,

          //! Length of internal buffer where altitude is stored
          ALTITUDE_LENGTH = 0x08,
        };

        /**
         * Default Constructor
         *
         */
        GPSPosition();

        /**
         * Copy Constructor
         *
         */
        GPSPosition(const GPSPosition& p);

        /**
         * Destructor
         *
         */
        virtual ~GPSPosition();

        /**
         * Sets latitude to a given value. Value have to be in decimal degree. Positive
         * values represent North coordinate and negative ones represent South coordinate.
         *
         * \param[in] coord latitude value (in decimal degree)
         *
         * \return NO_ERROR: if no errors, code errors #_gps_error otherwise
         */
        int setLatitude(const char *coord);

        /**
         * Sets longitude to a given value. Value have to be in decimal degree. Positive
         * values represent West coordinate and negative ones represent East coordinate.
         *
         * \param[in] coord longitude value (in decimal degree)
         *
         * \return NO_ERROR: if no errors, code errors #_gps_error otherwise
         */
        int setLongitude(const char *coord);

        /**
         * Sets altitude to a given value. Value have to be an integer value (in meters).
         *
         * \param[in] coord altitude value (meters)
         *
         * \return NO_ERROR: if no errors, code errors #_gps_error otherwise
         */
        int setAltitude(const char *coord);

        /**
         * Gets latitude in decimal degree as a string.
         *
         * \return coordinate value in decimal degree
         */
        const char* latitude() const;

        /**
         * Gets longitude in decimal degree as a string.
         *
         * \return coordinate value in decimal degree
         */
        const char* longitude() const;

        /**
         * Gets altitude in meters as a string.
         *
         * \return altitude value in meters
         */
        const char* altitude() const;

        /**
         * Overload operator = . It copies latitude, longitude and altitude values
         *
         */
        GPSPosition& operator=(const GPSPosition& p);

      private:
        /**
         * Sets a GPS component (latitude or longitude) to a given coordinate.
         * This function is the same for latitude and longitude and checks the format and the
         * value of the coordinate (it must be between -180 and 180)
         *
         * \param[in] coord coordinate value
         *
         * \return NO_ERROR: if no errors, #_gps_error code errors otherwise
         */
        int setCoordinate(const char *coord, char *dest);

        //! Latitude
        char m_latitude[COODINATE_LENGTH];

        //! Longitude
        char m_longitude[COODINATE_LENGTH];

        //! Altitude
        char m_altitude[ALTITUDE_LENGTH];

    };

    inline const char* GPSPosition::latitude() const
    {
      return m_latitude;
    }

    inline const char* GPSPosition::longitude() const
    {
      return m_longitude;
    }

    inline const char* GPSPosition::altitude() const
    {
      return m_altitude;
    }
  }
}
#endif /* GPSPOSITION_H_ */
