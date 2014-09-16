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
     * Class Component model a component for a value
     *
     */
    class ValueJSON: public JSON
    {
        JSON m_components;

      public:
        //! Label components
        static const char PROGMEM LABEL_COMPONENTS[];
        
        //! Labels table
        static const char* const LABEL[] PROGMEM;
//        static const char PROGMEM *LABEL[];

        /**
         * Default Constructor.
         *
         */
        ValueJSON()
        {
          aJsonObject* p = m_components.getJson();
          char lcomponents[15] = { 0 };

          strcpy_P(lcomponents, (char*)pgm_read_word(&(LABEL[0])));
          if (p != NULL)
          {
            m_components.setAsRoot(false);
            aJson.addItemToObject(m_json, lcomponents, p);
          }
        }

        /**
         * Constructor.
         *
         */
        ValueJSON(const char* time, char* validity)
        {
          aJsonObject* p = m_components.getJson();
          char lcomponents[15] = { 0 };

          strcpy_P(lcomponents, (char*)pgm_read_word(&(LABEL[0])));
          if (p != NULL)
          {
            add("time", time);
            m_components.setAsRoot(false);
            aJson.addItemToObject(m_json, lcomponents, p);
            add("validity", validity);
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
         * adds components
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

    };

    /**
     * Class Component model a component for a value
     *
     */
    class PublishJSON: public JSON
    {
        //! Value array
        aJsonObject* m_values;

      public:
        //! Label values
        static const char PROGMEM LABEL_VALUES[];
//        static prog_char LABEL_VALUES[] PROGMEM;

        //! Label stream
        static const char PROGMEM LABEL_STREAM[];
//        static prog_char LABEL_STREAM[] PROGMEM;

        //! Label sensor
        static const char PROGMEM LABEL_SENSOR[];
//        static prog_char LABEL_SENSOR[] PROGMEM;

        //! Labels table
        static const char* const LABEL[] PROGMEM;
//        static PROGMEM const char *LABEL[];

        /**
         * Default Constructor.
         *
         */
        PublishJSON() :
            m_values(NULL)
        {
          char lvalues[10] = { 0 };

          strcpy_P(lvalues, (char*)pgm_read_word(&(LABEL[0])));
          m_values = aJson.createArray();
          aJson.addItemToObject(m_json, lvalues, m_values);
        }

        /**
         * Constructor.
         *
         */
        PublishJSON(const char* stream, const char* sensor)
        {
          char label[10] = { 0 };

          setAsRoot(true);
          strcpy_P(label, (char*)pgm_read_word(&(LABEL[1])));

          add(label, stream);
          strcpy_P(label, (char*)pgm_read_word(&(LABEL[2])));
          add(label, sensor);

          m_values = aJson.createArray();

          strcpy_P(label, (char*)pgm_read_word(&(LABEL[0])));
          aJson.addItemToObject(m_json, label, m_values);
        }

        bool addValue(ValueJSON &value)
        {
          aJson.addItemToArray(m_values, value.getJson());
          value.setAsRoot(false);
          return true;
        }

        /**
         * Destructor
         *
         */
        virtual ~PublishJSON()
        {
        }
        ;

    };

    /**
     * Class Component model a component for a value
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
//        static prog_char LABEL_MESSAGE[] PROGMEM;
        static const char PROGMEM LABEL_MESSAGE[];

        //! Label stream
//        static prog_char LABEL_DIGEST[] PROGMEM;
        static const char PROGMEM LABEL_DIGEST[];

        //! Labels table
//        static PROGMEM const char *LABEL[];
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

          strcpy_P(label, (char*)pgm_read_word(&(LABEL[0])));
          aJson.addItemToObject(m_json, label, p);

          strcpy_P(label, (char*)pgm_read_word(&(LABEL[1])));
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
