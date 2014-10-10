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

        /**
         * Adds an element as couple of "label" and "value"
         *
         * \param[in] label label of the element to add
         * \param[in] value value of the element to add (as char array)
         *
         * \return false if element is not added
         */
        bool add(const char* label, const char* value)
        {
          aJsonObject* item = NULL;
          if ( ( item = aJson.createItem(value) ) == NULL )
          {
            return false;
          }

          return aJson.addItemToObject(m_json, label, item );
        }

        /**
         * Adds an element as couple of "label" and "value"
         *
         * \param[in] label label of the element to add
         * \param[in] value value of the element to add (as double)
         *
         * \return false if element is not added
         */
        bool add(const char* label, double value)
        {
          return aJson.addNumberToObject( m_json, label, value );
        }

        /**
         * Prints the JSON on a stream element (e.g. serial)
         *
         * \param[in] label label of the element to add
         * \param[in] value value of the element to add (as double)
         *
         * \return false if there is an error
         */
        bool dump(Stream& stream);

        /**
         * Initializes message from a char array. This one contains the JSON.
         *
         * \param[in] json JSON string
         *
         * \return false if there is an error, true otherwise
         */
        bool initialize(char* json);

        /**
         * Gets the JSON object
         *
         * \return the JSON object
         */
        aJsonObject* getJson() const { return m_json; }

        /**
         * Gets item selectiong it from the label
         *
         * \param[in] label label used to select the item
         *
         * \return item
         */
        aJsonObject* getItem(char* label) const { return aJson.getObjectItem(m_json, label); }

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
