/*
 * Copyright (C) 2014 CSP Innovazione nelle ICT s.c.a r.l. (http://www.csp.it/)
 * All rights reserved.
 *
 * All information contained herein is, and remains the property of
 * CSP Innovazione nelle ICT s.c.a r.l. and its suppliers, if any.
 * The intellectual and technical concepts contained herein are proprietary to
 * CSP Innovazione nelle ICT s.c.a r.l. and its suppliers and may be covered
 * by Italian and Foreign Patents, in process, and are protected by trade secret
 * or copyright law.
 * Dissemination of this information or reproduction of this material
 * is strictly forbidden unless prior written permission is obtained
 * from CSP Innovazione nelle ICT s.c.a r.l.
 *
 * sdpstatus.cpp
 *         <description>
 *
 * Authors:
 *         Marco Boeris Frusca <marco.boeris@csp.it>
 *
 */

#include "smartobjstatus.h"

namespace sdp
{
  namespace client
  {

    SmartObjStatus::SmartObjStatus() :
        m_status(IDLE)
    {
      //! Reset counters
      m_uptime.update();
      updateLastReadTime();
      updateLastSendTime();
      updateLastErrorTime();
    }

    SmartObjStatus::~SmartObjStatus()
    {
      // TODO Auto-generated destructor stub
    }

    bool SmartObjStatus::updateStatus(uint8_t status)
    {
      switch (status)
      {
        IDLE:
        READ_SENSORS:
        SEND_MEASURE:
        CONTROL:
        {
          m_status = status;
        }
        ;
        break;

      default:
      {
        m_status = IDLE;
        return false;
      }
      }
      return true;
    }

  } /* namespace client */
} /* namespace sdp */
