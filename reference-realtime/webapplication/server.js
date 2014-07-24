/**
 * Smart Data Platform - Reference Implementation 
 * 
 * This is a Node.JS script that emulates the behaviour of the SDP.
 * It implements a Websocket server (using Socket.IO library) that
 * will provide the following interaction with the client (browser script).
 * The MQTT client upport is based on "mqtt" package available through npm.
 * 
 * This script will:
 * 1) Wait for incomeing websocket connections
 * 2) Uopon successful connection, it will receive from the client
 *    a JSON object containing the topic name.
 * 3) Using the topic provided by the client, a new subscription si
 *    requested to the broker
 * 4) Upon receiving an object from the broker it will be sent to che
 *    client using the Websocket connection
 * 
 * Usage: node server.js
 */

var mqtt = require('mqtt');
var socketio = require('socket.io').listen('8000');
var temperature = "550e8400-e29b-41d4-a716-446655440000/temperature";//sensorID
socketio.on('connection', function(client) {
	client.on('message', function(temperature) {
		subscriber = mqtt.createClient();
		subscriber.subscribe(temperature);
		subscriber.on('message', function(temperature, message) {
			client.send(message);
		});
	});
});
