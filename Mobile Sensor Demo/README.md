SmartTrek - mobile sensor demo
=========================
This folder contains a first version of Mobile Sensor Demo application.

The demo consist in two web application:

* **SmartTrekSensor:** represent the mobile sensor. Read, through the device GPS sensor, the location of the user and sends it to the Smart Data Platform. You can set, via GUI, the name of the sensor and the refresh/sending interval.

* **SmartTrekViewer:** represent the client viewer. The application subscribe itself to one of the SDP topics and receives, in push mode, the position of the sensor. You need to select the sensor to be monitored and connect to the platfom
.
The demo was written using the AppStudio IDE [link](https://www.nsbasic.com) which allows quick prototyping of JQuery Mobile Application. To view and to modify the source code, you must have AppStudio IDE. The demo source code is located in the **src** folder.

AppStudio generates an application based on JQuery Mobile. The generated application is purely HTML5 and JavaScript and can be used and changed freely.

Both the demo (sensor and viewer) are located in the **release** folder.
The demo can be performed:
* locally (double click on the index.html file)
* published on a web server (Smart Data Platform supports cross-domain ajax call according to the CORS specific)
* embedded on mobile application using phone gap (cordova)




