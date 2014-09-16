Smart Data Platform (SDP)
==============

Library C
--------------
This libraries provides the API for the development of a publisher or a listener to send MQTT acquisitions for linux systems and to get in receiving update messages from the server.

Installation
-----------

 *  Prerequisites (Unix only):

``` 
GCC 4.2 or newer
Python 2.6 or 2.7
GNU Make 3.81 or newer
```

 *  Installation of library
  The libraries are stored under the path /usr/local/lib 
```
make
make install (from root)
```
    

Usage
-----   
    
 *  For usage the library linking with che command -lsdpC
 
```
gcc test.c -o test -lsdpC
```