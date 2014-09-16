SOURCES=observation.c observation.h sender.c sender.h  mqtt.c mqtt.h cJSON.c cJSON.h                                                                                                                                          
CC=gcc                                                                                                                                                                                    
                                                                                                                                                                                    
OBJECTS=$(SOURCES:.c=.o)                                                                                                                                                            
                                                                                                                                                                                    
EXECUTABLE=libsdpC.so                                                                                                                                                            
                                                                                                                                                                                    
libsdp.so : observation.c observation.h sender.c sender.h mqtt.c mqtt.h cJSON.c cJSON.h                                                                                                                                
	$(CC) -fPIC -c observation.c 
	$(CC) -fPIC -c sender.c
	$(CC) -fPIC -c mqtt.c
	$(CC) -fPIC -c cJSON.c
	$(CC) -shared -o libsdpC.so observation.o sender.o mqtt.o cJSON.o
                                                                                                                                                                                    
clean:                                                                                                                                                                              
	rm libsdpC.so observation.o sender.o mqtt.o cJSON.o
install:
	cp libsdpC.so observation.o sender.o mqtt.o cJSON.o /usr/local/lib
	cp observation.h sender.h mqtt.h cJSON.h /usr/local/include
