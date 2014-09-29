/**
 * Class for reconnecting the Websocket if the first attempt fails.
 */


var WebSocketService =  (function(){
	var stompClient = {};	
//	var self = this;
	var connectedFlag = false;
	var SingletonClient = null;
	
	
	var SubscriptedElementsList = [];
    
	var WEB_SOCKET_BASE_URL = null;
	
    var CancelAllSubscriptions = function(){
  	  for(var i =0; i< SubscriptedElementsList.length ; i++){
			  var widget = SubscriptedElementsList[i];
			  console.debug(':::: Unsubscribe for ::::', widget);
			   widget.unsubscribe();      				  
			}
	 SubscriptedElementsList = [];
    };
	
	
	function ConnectTheSocket(username,password,on_connect, on_error, vhost,count){
		
		
		CancelAllSubscriptions();
		
		/*
		 * Fai la disconnect
		 */
		
		if(connectedFlag){
			stompClient.disconnect(function(){
				connectedFlag=false;
			});
		}
		
		stompClient = Stomp.client(WEB_SOCKET_BASE_URL);
		
		stompClient.connect(username, password, function(frame) {
			connectedFlag=true;			
			on_connect(frame);
			
		}, function(frame) {			 
		      if (count<5) {
		       console.debug("Tentativo di riconnessione numero : ",count);	
		       setTimeout(function(){ new ConnectTheSocket(username, password,on_connect, on_error, vhost,++count);},count*1000);
		       console.debug("awake.. ");		         	       
		      on_error(frame);
		      }			
		}, vhost);
	};
	
	
	function NGStomp(URL) {
		console.debug("Stomp",Stomp);
		if(URL)
			WEB_SOCKET_BASE_URL = URL;
		this.count=1;
	}
	
	NGStomp.prototype.subscribe = function(queue, callback) {
		var subscribedClient = stompClient.subscribe(queue, function() {
			var args = arguments;
				callback(args[0]);
		});
		
		SubscriptedElementsList.push(subscribedClient);
		
		return subscribedClient;
	};

	NGStomp.prototype.unsubscribeAll = function(){
		CancelAllSubscriptions();
	};
	
	
	NGStomp.prototype.send = function(queue, headers, data) {
		stompClient.send(queue, headers, data);
	};

	
	NGStomp.prototype.connect = function(username,password,on_connect, on_error, vhost) {
		this.count=1;
		new ConnectTheSocket(username,password,on_connect, on_error, vhost,this.count);
	};

	NGStomp.prototype.disconnect = function(callback) {
		stompClient.disconnect(function() {
			var args = arguments;
			callback(args);
		});
	};

	return function(url) {
		if(!SingletonClient){
		SingletonClient = new NGStomp(url);
		}
		return SingletonClient;
	};
})();
