/*
 /**
 * \file smartobjstatus.h
 * \brief Arduino library for modeling a Smart Object status.
 *
 * \author Marco Boeris Frusca
 *
 */
#ifndef SDPSTATUS_H_
#define SDPSTATUS_H_

#include "Arduino.h"
#include "intervalseconds.h"

namespace sdp
{
  namespace client
  {

    class SmartObjStatus
    {
        //! Current state
        uint8_t m_status;

        //! Time from the boot
        IntervalSeconds m_uptime;

        //! Time from the last sensor reading
        IntervalSeconds m_lastReadTime;

        //! Time from the last sending of  a measurement
        IntervalSeconds m_lastSendTime;

        //! Time from the last sending of  a measurement
        IntervalSeconds m_lastErrorTime;

      public:
        /**
         * \enum   _node_state
         *
         * State of the node
         */
        enum _node_state
        {
          //! Idle, do nothing
          IDLE = 0,

          //! Read sensors
          READ_SENSORS = 1,

          //! Send measure
          SEND_MEASURE = 2,

          //! Handle control message
          CONTROL = 3,

          //! Update system time
          UPDATE_TIME = 4,

          //! Connect to the server
          CONNECT = 5,

          //! Disconnect to the server
          DISCONNECT = 6,

        };

        SmartObjStatus();
        virtual ~SmartObjStatus();

        void updateLastReadTime()
        {
          m_lastReadTime.update();
        }
        ;

        void updateLastSendTime()
        {
          m_lastSendTime.update();
        }
        ;

        void updateLastErrorTime()
        {
          m_lastErrorTime.update();
        }
        ;

        unsigned long lastReadTime()
        {
          return m_lastReadTime.time();
        }
        ;

        unsigned long lastSendTime()
        {
          return m_lastSendTime.time();
        }
        ;

        unsigned long lastErrorTime()
        {
          return m_lastErrorTime.time();
        }
        ;

        unsigned long uptime()
        {
          return m_uptime.time();
        }
        ;
        bool updateStatus(uint8_t status);
        uint8_t status()
        {
          return m_status;
        }
        ;
        void start()
        {
          m_uptime.update();
        }
    };

  } /* namespace client */
} /* namespace sdp */

#endif /* SDPSTATUS_H_ */
