Smart Data Platform (SDP)
==============

Server reference implementation
--------------
The reference solution uses a server environment that allow to emulate some of the interfaces that the SDP will provide. In particular, the components that must be installed on the server allow you to emulate the functions of acquisition and MQTT stream to the browser via WebSockets.

Installation
-----------

 *  Installation of the Broker MQTT
The broker MQTT chosen for the realization of the reference solution is mosquitto. The software can be installed via the package management system of Ubuntu.
```
sudo apt-get install mosquitto mosquitto-clients
```

 *  Installation of the server Websocket
The implementation WebSocket choice for the reference solution is based on Node.JS and Socket.IO. 
Node.JS must be installed by downloading and compiling from source.

 Prerequisites for Node.js:

```
GCC 4.2 or newer
Python 2.6 or 2.7
GNU Make 3.81 or newer
libexecinfo (FreeBSD and OpenBSD only)
```

 Installation of Node.js:
```
tar xzf node-<version>.tar.gz
cd node-<version>
./configure
make
make install
```

 After installing Node.JS, you must install the necessary add-on modules that are used for the operation of the reference solution. They can be installed using the package management tool that is installed as part of Node.JS: npm. The first of these components is Socket.IO which is the module that provides an implementation of Node.JS websocket. For simplicity, we install all modules globally so that they are accessible to any Linux user (option "-g").
```
sudo npm install –g socket.io
sudo npm install –g mqtt
sudo npm install –g forever
```
 Node.JS is necessary to indicate the location to which you installed the add-on modules that are managed by the tool "npm". For simplicity you can use the file provided in the archive that accompanies this document.
``` 
cp node.sh /etc/profile.d/
```

 *  Installation of the web page
The final part is to insert the page html demo and the necessary dependencies JavaScript within a web server. For example, if you installed Apache from the official repositories of Ubuntu, just copy the necessary files into a folder under "/var/www".
```
sudo apt-get install apache2
sudo mkdir /var/www/sdp-ref
sudo cp index.html socket.io.js style.css /var/www/sdp-ref
```
Usage
-----   
    
 *  At this point it is necessary to log in again for the changes to be made active. To start the server, you can use the command "node" that it starts in an interactive way.
```
node server.js
```    
 *  Alternatively, you can use the command "forever" to send the server in the background. Through the same command you can list the active nodes and arrest them.
``` 
forever start server.js
forever list
   info:    Forever processes running
   data:        uid  command             script    forever pid  logfile                       uptime        
   data:    [0] nwGu /usr/local/bin/node server.js 6400    6403 /home/<user>/.forever/nwGu.log 0:0:11:34.604
forever stop 0
```    
    
 *  Using your browser you can then connect to the server prepared to see the demo page.
``` 
http://<indirizzo-server>/sdp-ref/
```