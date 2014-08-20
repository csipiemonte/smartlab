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
  if (m_json == NULL)
    return false;

  char* string = aJson.print(m_json);
/*
  stream.print( "dump" );
  stream.print( " : " );
  stream.println( freeMemory() );
*/
  if (string != NULL)
  {
    aJsonStream serial_stream(&stream);
    aJson.print(m_json, &serial_stream);

  } else
  {
    return false;
  }

  if (string != NULL)
  {
    free(string);
  } else
  {
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

