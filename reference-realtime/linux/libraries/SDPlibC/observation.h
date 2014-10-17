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
  int /*char* */sensor_value; /**< The value of the sensor */
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
 * @return Date and time
 */
char* getTime();

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
 * @return l'observation with new value
 */
Observation observationAddValue(Observation obs, Value value);

/**
 * @brief Create a object values
 * 
 * Create a new values. If the field time is empty the time is added automatically
 * @param time the date and time of values. 
 * @return the object value
 */
Value newValue(char *time);

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
 * @return l'oggetto component
 */
Component newComponent(char *name,int value/*char* value*/);

/**
 * @brief Create a new component
 * 
 * Create a new component with default name ZERO
 * @param value the value of component
 * @return l'object component
 */
Component newComponentDefault( int value/*char* value*/);

/**
 * @brief Create the json
 * 
 * Create the json from observation for send message
 * @param obs l'observation 
 * @return the message
 */
char* toJson(Observation obs);

#endif /* JSON_H_ */
