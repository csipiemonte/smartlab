/**
 * \file sdppublishmsg.h
 * \brief Arduino Library for describes a SDP publish message.
 *
 * \author Marco Boeris Frusca
 *
 */

#ifndef SDPPUBLISHMSG_H_
#define SDPPUBLISHMSG_H_

#include "Arduino.h"
#include <avr/pgmspace.h>

#include "aJSON.h"
#include "sdpbasemsg.h"
#include "sha256.h"
#include "Base64.h"

namespace sdp
{
  namespace message
  {
    /**
     * Class ValueJSON model a the value field in the publish message
     *
     */
    class ValueJSON: public JSON
    {
        //! JSON for the component field
        JSON m_components;

      public:
        //! Label components
        static const char LABEL_COMPONENTS[] PROGMEM;

        //! Label for a valid measurement
        static const char LABEL_VALID[] PROGMEM;

        //! Label for a erroneous measurement
        static const char LABEL_ERRONEOUS[] PROGMEM;

        //! Label for a doubtful measurement
        static const char LABEL_DOUBTFUL[] PROGMEM;

        //! Label for a unknown measurement
        static const char LABEL_UNKNOWN[] PROGMEM;

        //! Flash table for wireless configuration
        static const char* const VALUE_TABLE[] PROGMEM;

        /**
         * \enum   _value_table_index
         *
         * Index of the elements in the value_table
         */
        enum _value_table_index
        {
          //! Valid value
          VALID = 0,

          //! Erroneous value
          ERRONEOUS = 1,

          //! Doubtful value
          DOUBTFUL = 2,

          //! Unknown validity for the value
          UNKNOWN = 3,

          //! Component value
          COMPONENT = 4,
        };

        /**
         * Default Constructor.
         *
         */
        ValueJSON()
        {
          aJsonObject* p = m_components.getJson();
          char lcomponents[15] = { 0 };

          memset(lcomponents, 0, 15);
          strcpy_P(lcomponents,
              (char*) pgm_read_word(&(VALUE_TABLE[COMPONENT])));

          if (p != NULL)
          {
            m_components.setAsRoot(false);
            aJson.addItemToObject(m_json, lcomponents, p);
          }
        }

        /**
         * Constructor.
         *
         * \param[in] time time stamp for the measurement
         * \param[in] validity validity field
         */
        ValueJSON(const char* time, int validity)
        {
          aJsonObject* p = m_components.getJson();
          char lbuffer[15] = { 0 };

          memset(lbuffer, 0, 15);
          strcpy_P(lbuffer,
              (char*) pgm_read_word(&(VALUE_TABLE[COMPONENT])));

          if (p != NULL)
          {
            add("time", time);
            m_components.setAsRoot(false);
            aJson.addItemToObject(m_json, lbuffer, p);
            getValilidity(validity, lbuffer, 15);
            add("validity", lbuffer);
          } else
          {
          }
        }

        /**
         * Destructor
         *
         */
        virtual ~ValueJSON()
        {
        }

        /**
         * Adds a component
         *
         * \param[in] label label of the component
         * \param[in] value value as string
         *
         * \retrun false if there is an error, true otherwise
         */
        bool addComponent(const char* label, const char* value)
        {
          if (m_components.getJson() == NULL)
          {
            return false;
          }

          if (!m_components.add(label, value))
          {
            return false;
          }
          return true;
        }

        /**
         * Adds a component
         *
         * \param[in] label label of the component
         * \param[in] value value as double
         *
         * \retrun false if there is an error, true otherwise
         */
        bool addComponent(const char* label, double value)
        {
          if (m_components.getJson() == NULL)
          {
            return false;
          }

          if (!m_components.add(label, value))
          {
            return false;
          }
          return true;
        }

      private:
        /**
         * Gets the quality of the measurement ad a string
         *
         * \param[in] validity quality of the value
         * \param[in] buffer char array where the validity will be stored as a string
         * \param[in] bSize size of the buffer
         *
         * \retrun false if there is an error, true otherwise
         */
        bool getValilidity(uint16_t validity, char* buffer, size_t bSize = 15)
        {
          bool ret = true;
          memset(buffer, 0, bSize);

          switch (validity)
          {
            case VALID:
            case ERRONEOUS:
            case DOUBTFUL:
            case UNKNOWN:
            {
              ret = true;
            }
              ;
              break;
            default:
            {
              ret = false;
            }
              ;
              break;
          };

          if (ret)
          {
            strncpy_P(buffer, (char*) pgm_read_word(&(VALUE_TABLE[validity])),
                bSize);
          } else
          {
            strncpy_P(buffer, (char*) pgm_read_word(&(VALUE_TABLE[UNKNOWN])),
                bSize);
          }

          return ret;

        }
        ;

    };

    /**
     * Class PublishJSON model a SDP publish message
     *
     */
    class PublishJSON: public JSON
    {
        //! Value array
        aJsonObject* m_values;

      public:
        //! Label values
        static const char PROGMEM LABEL_VALUES[];

        //! Label stream
        static const char PROGMEM LABEL_STREAM[];

        //! Label sensor
        static const char PROGMEM LABEL_SENSOR[];

        //! Labels table
        static const char* const LABEL[] PROGMEM;

        /**
         * Default Constructor.
         *
         */
        PublishJSON() :
            m_values(NULL)
        {
          char lvalues[10] = { 0 };

          strcpy_P(lvalues, (char*) pgm_read_word(&(LABEL[0])));
          m_values = aJson.createArray();
          aJson.addItemToObject(m_json, lvalues, m_values);
        }

        /**
         * Constructor.
         *
         *
         * \param[in] stream identifier of the stream
         * \param[in] smartObj identifier of the smartobject
         */
        PublishJSON(const char* stream, const char* smartObj)
        {
          char label[10] = { 0 };

          setAsRoot(true);
          strcpy_P(label, (char*) pgm_read_word(&(LABEL[1])));

          add(label, stream);
          strcpy_P(label, (char*) pgm_read_word(&(LABEL[2])));
          add(label, smartObj);

          m_values = aJson.createArray();

          strcpy_P(label, (char*) pgm_read_word(&(LABEL[0])));
          aJson.addItemToObject(m_json, label, m_values);
        }

        /**
         * Destructor
         *
         */
        virtual ~PublishJSON()
        {
        }
        ;

        /**
         * Adds a value to the JSON
         *
         * \param[in] value value object
         *
         * \return true if there is not error
         */
        bool addValue(ValueJSON &value)
        {
          aJson.addItemToArray(m_values, value.getJson());
          value.setAsRoot(false);
          return true;
        }
        ;

    };

    /**
     * Class SecureJSON model a proposal SDP publish message in secure format
     *
     */
    class SecureJSON: public JSON
    {
      public:
        /**
         * \enum    _sdpsource_error
         *
         * Error codes for SDP data source
         */
        enum _secure_json_property
        {
          //! HMAC keyword length
          KEY_SIZE = 64,
        };

        //! Label values
        static const char PROGMEM LABEL_MESSAGE[];

        //! Label stream
        static const char PROGMEM LABEL_DIGEST[];

        //! Labels table
        static const char* const LABEL[] PROGMEM;

        /**
         * Default Constructor.
         *
         */
        SecureJSON() :
            m_validKey(false), m_msg(NULL)
        {
        }

        /**
         * Destructor
         *
         */
        virtual ~SecureJSON()
        {
        }
        ;

        /**
         * Sets keyword. It must be of #KEY_SIZE bytes.
         *
         * \param key keyword used to calculate HMAC-SHA256
         * \param length length of the keyword.
         */
        void setKey(uint8_t* key, size_t length)
        {
          if (key != NULL && length <= KEY_SIZE)
          {
            Sha256.initHmac(key, KEY_SIZE);
            m_validKey = true;
          } else
          {
            m_validKey = false;
          }
        }

        /**
         * Sets create hash and json
         *
         * \param msg message
         */
        void create(char* msg)
        {
          Sha256.print(msg);
          uint8_t* result = Sha256.resultHmac();
          printHash(result);
          char resultB24[99] = { 0 };
          base64_encode(resultB24, (char *) result, 32);
        }

        /**
         * Sets create hash and json
         *
         * \param msg message
         *
         * \return false if there is an error, true otherwise
         */
        bool create(JSON &json)
        {
          char label[10] = { 0 };

          char *msg = aJson.print(json.getJson());
          if (msg == NULL)
          {
            return false;
          }
          char resultB64[99] = { 0 };

          Sha256.print(msg);
          uint8_t* result = Sha256.resultHmac();
          base64_encode(resultB64, (char *) result, 32);

          json.setAsRoot(false);
          aJsonObject* p = json.getJson();

          strncpy_P(label, (char*) pgm_read_word(&(LABEL[0])), 10);
          aJson.addItemToObject(m_json, label, p);

          strncpy_P(label, (char*) pgm_read_word(&(LABEL[1])), 10);
          add(label, resultB64);
          /*
           Serial.print( F("freeMemory()=") );
           Serial.println(freeMemory());
           */
          if (msg != NULL)
          {
            free(msg);
            msg = NULL;
          }
        }

      private:

        void printHash(uint8_t* hash)
        {
          int i;
          for (i = 0; i < 32; i++)
          {
            Serial.print("0123456789abcdef"[hash[i] >> 4]);
            Serial.print("0123456789abcdef"[hash[i] & 0xf]);
          }
          Serial.println();
        }
        //! true if keyword is set
        bool m_validKey;

        char* m_msg;
//        byte *m_key;

    };

  }
}
#endif /* SDPPUBLISHMSG_H_ */
