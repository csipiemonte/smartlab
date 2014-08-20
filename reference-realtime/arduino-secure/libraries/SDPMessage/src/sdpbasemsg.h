/**
 * \file sdpbasemsg.h
 * \brief Arduino Library for describes a SDP message interface.
 *
 * \author Marco Boeris Frusca
 *
 */

#ifndef SDPBASEMSG_H_
#define SDPBASEMSG_H_

#include "Arduino.h"
#include "aJSON.h"

namespace sdp
{
  namespace message
  {
    /**
     * Class Component model a component for a value
     *
     */
    class JSON
    {
      protected:
        //! Object JSON
        aJsonObject* m_json;

        //! Flag for root JSON;
        bool m_isRoot;

      public:
        /**
         * Default Constructor.
         *
         */
        JSON() :
          m_json(NULL), m_isRoot(true)
        {
          m_json = aJson.createObject();
        }

        /**
         * Destructor
         *
         */
        virtual ~JSON()
        {
          if (m_isRoot)
          {
            aJson.deleteItem(m_json);
          };
        }

        /**
         * Sets element as a root
         *
         * \param[in] isRoot true: if it is root element of the json, false otherwise
         */
        void setAsRoot(bool isRoot)
        {
          m_isRoot = isRoot;
        }

        bool add(const char* label, const char* value)
        {
          return aJson.addItemToObject(m_json, label, aJson.createItem(value) );
        }

        bool add(const char* label, double value)
        {
          return aJson.addNumberToObject(m_json, label, value );
        }

        bool dump(Stream& stream);

        aJsonObject* getJson() const { return m_json; }

    };

    /**
     * SDPBaseMsg Class defines a interface for SDP messages.
     */
    class SDPBaseMsg
    {
      public:
        /**
         * Default Constructor.
         *
         */
        SDPBaseMsg();

        /**
         * Destructor
         *
         */
        virtual ~SDPBaseMsg();
    };
  }
}
#endif /* SDPBASEMSG_H_ */
