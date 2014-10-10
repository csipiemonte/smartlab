/*
 * sdpbasemsg.cpp
 *
 *  Created on: Jul 25, 2014
 *      Author: dtvlab
 */

#include "sdpbasemsg.h"
/*
#include <MemoryFree.h>
*/
using namespace sdp::message;

bool JSON::dump(Stream& stream)
{
  if (m_json == 0)
    return false;

  char* string = aJson.print(m_json);
/*
  stream.print( "dump" );
  stream.print( " : " );
  stream.println( freeMemory() );
*/
  if (string != 0)
  {
    aJsonStream serial_stream(&stream);
    aJson.print(m_json, &serial_stream);

  } else
  {
    return false;
  }

  if (string != 0)
  {
    free(string);
  } else
  {
  }
  return true;
}


bool JSON::initialize(char* json)
{
  if (m_json != 0)
  {
    aJson.deleteItem(m_json);
    m_json = 0;
  }

  if ( (m_json = aJson.parse(json)) == 0 )
  {
    return false;
  }

  return true;
}


SDPBaseMsg::SDPBaseMsg()
{
  // TODO Auto-generated constructor stub

}

SDPBaseMsg::~SDPBaseMsg()
{
  // TODO Auto-generated destructor stub
}

