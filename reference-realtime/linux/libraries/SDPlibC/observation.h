/**
 * @file observation.h
 * @author Leonardo Sileo
 * @date 18 Luglio 2014
 * @brief Allows you to create an observation and returns the json of this last
 * 
 */

#ifndef JSON_H_
#define JSON_H_

#include <time.h>

/**
 * @brief Struct for the object Stream
 *
 * 
 */
typedef struct 
{
  char *idStream; /**< L'indentifier of the stream */
} Stream;

/**
 * @brief Struct for the object Sensor
 *
 * 
 */
typedef struct 
{
  char *idSensor; /**< L'indentifier of the  sensore */
} Sensor;

/**
 * @brief Struct for the object Component
 *
 * 
 */
typedef struct 
{
  char *sensor_name; /**< The type of sensor */
  double sensor_value; /**< The value of the sensor */
  char* stringValue; /**< The string of the sensor */
  char type; /**< The type of the value (double=f, string=s, boolean=b) */
} Component;

/**
 * @brief Struct for the object Value
 *
 * 
 */
typedef struct 
{
  
  char* time; /**< Date and time in format ISO8601 */
  int contComponents; /**< Component counter  */
  Component *components; /**< Array of components */
  char* validity; /**< Validity for the sensor */
} Value;


/**
 * @brief Struct for the object Observation
 *
 * 
 */
typedef struct 
{

  Stream mStream; /**< Stream struct */
  Sensor mSensor; /**< Sensor struct*/
  int contValues; /**< Values counter*/
  Value *mValues; /**< Array of values */
} Observation;

/**
 * @brief Return the actual time
 * 
 * Allow of return the date and time in the format ISO8601
 * 
 * @param buffer the container of the time
 * @return Date and time
 */
char* getTime(char *buffer);

/**
 * @brief Return the time of next message
 * 
 * Allow of return the next message the date and time in the format ISO8601
 * 
 * @param buffer the container of the time
 * @param seconds the time between messages
 * @return Date and time
 */
char* getNextTime(char *buffer, int seconds);
/**
 * @brief Create a object Observation
 * 
 * Crea a new Observation whit streamID and sensorID 
 * of the survey
 * @param idStream identifier of the stream
 * @param idSensor identifier of the sensor
 * @return the observation 
 */
Observation newObservation(char *idStream, char *idSensor);

/**
 * @brief Add a values at Observation
 * 
 * Add a values at Observation 
 * @param obs the observation 
 * @param value the object value to add al observation
 * @return the observation with new value
 */
Observation observationAddValue(Observation obs, Value value);

/**
 * @brief Create a object values
 * 
 * Create a new values. If the field time is empty the time is added automatically
 * @param time the contaioner the date and time of values. 
 * @return the object value
 */
Value newValue(char *time);

/**
 * @brief Create a object values
 * 
 * Create a new values. If the field time is empty the time is added automatically
 * @param time the date and time of values. 
 * @return the object value
 */
Value newValueWithTime(char *time);
/**
 * @brief Add a component at Value
 * 
 * Add a component at Value
 * @param value the value
 * @param component the object component to add al value
 * @return il value con il nuovo component
 */
Value addComponetsAtValue(Value value, Component component);

/**
 * @brief Create a new component
 * 
 * Create a new component
 * @param name the name of component
 * @param value the value of component
 * @return the object component
 */
Component newComponentValue(char *name,double value);

/**
 * @brief Create a new component
 * 
 * Create a new component with default name c0
 * @param value the value of component
 * @return the object component
 */
Component newComponentDefaultValue( double value);

/**
 * @brief Create a new component
 * 
 * Create a new component
 * @param name the name of component
 * @param stringVal the string of component
 * @return the object component
 */
Component newComponentString(char *name, char* stringVal);

/**
 * @brief Create a new component
 * 
 * Create a new component with default name c0
 * @param stringVal the string of component
 * @return the object component
 */
Component newComponentDefaultString( char* stringVal);
/**
 * @brief Create the json
 * 
 * Create the json from observation for send message
 * @param obs the observation
 * @param sendMessage the container of the message 
 * @return the message
 */

/**
 * @brief Create a new component
 * 
 * Create a new component
 * @param name the name of component
 * @param boolVal the boolean rappresentation of component ( 0=false 1=true)
 * @return the object component
 */
Component newComponentBoolean(char *name, int boolVal);

/**
 * @brief Create a new component
 * 
 * Create a new component with default name c0
 * @param boolVal the boolean rappresentation of component ( 0=false 1=true)
 * @return the object component
 */
Component newComponentDefaultBoolean( int boolVal);

/**
 * @brief Create a new component
 * 
 * Create a new component
 * @param name the name of component
 * @param stringVal the string of component
 * @return the object component
 */
Component newComponentString(char *name, char* stringVal);

/**
 * @brief Calculate the length of message
 * 
 * Calculate the length of message
 * @param observation the observation
  * @return the the length of observation
 */
int* toLengthJson(Observation observation);
/**
 * @brief Create the message
 * 
 * Create the messega
 * @param observation the observation
 * @param sendMessage the buffer of array 
  * @return the the length of observation
 */

char* toJson(Observation obs, char *sendMessage);

#endif /* JSON_H_ */
