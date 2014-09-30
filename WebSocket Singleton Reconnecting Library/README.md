smartlab
========

This is a javascript library that wraps  STOMP.js , adding some extra functionality as :

1) Attempts to reconnect 5 times to the server if the previous attempts fail;

2) Creates a single instance, (Singleton) in order to have  a central point, and semplify the unsubscribe;

3) Adds the functionality to Unsubscribe from all Queues.
