var WebsocketStompSingleton = (function(){
             
			  var clientInstance; //private variable to hold the
                               //only instance of client that will exits.
 
			  
			  var SubscriptionList = [];
			  var SubscriptedElementsList = [];
              var connectedClient = false;
              
              
              var CancelAllSubscriptions = function(){
            	  for(var i =0; i< SubscriptedElementsList.length ; i++){
      				  var widget = SubscriptedElementsList[i];
      				  console.debug(':::: Unsubscribe for ::::', widget);
      				   widget.unsubscribe();      				  
					}
              };
            
              var createClient = function(settings,count){
            	  CancelAllSubscriptions();
            	  var intSettings = settings;	                    
                  var client = Stomp.client(intSettings.ws_url);
                  console.debug(':::: connesso::::',intSettings.ws_url,intSettings);
                  client.connect(intSettings.ws_user,intSettings.ws_pwd,
          			function(frame) { //success Callback
              			  console.debug(':::: connesso::::');
              			  for(var i =0; i< SubscriptionList.length ; i++){
              				  var widget = SubscriptionList[i];
              				  console.debug(':::: connesso in For subscribe for ::::', widget);
              				SubscriptedElementsList.push( client.subscribe(widget.keyTopic,widget.keyCallback));
              				  
        					}
              			  connectedClient=true;
    					},
    					function(frame) //error Callback
    					{
    						if (count<5) {
    						       console.debug("createClient count ::::::::::::: ",count);    						       
    						       setTimeout(function(){createClient(intSettings,++count);},count*1000);
    						       console.debug("awake.. ");		         	       
					      } else{
    						    	  console.debug(':::: Impossibile connettersi::::');
    						    }	
    					});
                    
                    
                  return {
                	  getWebClient: function(){               		 
                		  
                		  return client;
                	  },
                	  addSubscription : function(topic,callback){
                		  console.debug(':::: addSubscription::::',topic,callback);
                		  if(connectedClient){
                			  console.debug(':::: addSubscription Client connesso::::');
                			  SubscriptionList.push({
                				  keyTopic:topic,
                				  keyCallback:callback
                			  });
                			  client.subscribe(topic,callback);
                		  }else{
                			  console.debug(':::: addSubscription Client NON connesso Add to SubscriptionList::::');
                			  SubscriptionList.push({
                				  keyTopic:topic,
                				  keyCallback:callback
                			  });
                		  }
                	  },
                	  cancelAllSubscriptions:CancelAllSubscriptions
                  };                         
              };
 
              return {
                    getInstance: function(settings){
                          if(!clientInstance){
                        	  console.debug("::::  New Stomp Client Created ::::");
                        	  clientInstance = createClient(settings,1);
                        	  
                          }
                          return clientInstance;
                   }
              };
})();

(function()
{
	freeboard.loadDatasourcePlugin({
		"type_name"   : "websocket_stomp_plugin",
		"display_name": "Datasource Plugin For Websocket STOMP",
		"description" : "<strong>Datasource Plugin For Websocket STOMP</strong>",
		"settings"    : [
			{
				"name"         : "ws_url",
				"display_name" : "Websocket URL",
				"type"         : "text",
				"default_value": "ws://stream.smartdatanet.it/ws",
				"description"  : "Url of wesocket stomp server"
			},
			{
				"name"         : "ws_user",
				"display_name" : "Username",
				"type"         : "text",
				"default_value": "guest",
				"description"  : "Username"
			},
			{
				"name"         : "ws_pwd",
				"display_name" : "Password",
				"type"         : "text",
				"default_value": "Aekieh6F",
				"description"  : "Password"
			},
			{
				"name"         : "ws_topic",
				"display_name" : "Topic to subscribe",
				"type"         : "text",
				"default_value": "/topic/output.csp.89f84a22-1e2e-5882-bbf0-9c6efffb8ce7_FrmHyd",
				"description"  : "Topic to subscribe"
			}
		],
		newInstance   : function(settings, newInstanceCallback, updateCallback)
		{
			newInstanceCallback(new websocketStompPlugin(settings, updateCallback));
		}
	});
		var websocketStompPlugin = function(settings, updateCallback) 
		{
			var self = this;
			var currentSettings = settings;
			console.debug("::::::: Settings:::::",settings);
		//	Stomp.WebSocketClass = ReconnectingWebSocket;
			var clientSingleton = WebsocketStompSingleton.getInstance(currentSettings);
			var client = clientSingleton.getWebClient();
//			var cont = 0;
			
			clientSingleton.addSubscription(currentSettings.ws_topic,
					function(message) {
				jsonMessage = JSON.parse(message.body);
				updateCallback(jsonMessage);
			});
//			function createConnection(intSettings, intUpdateCallback,intClient)
//			{
//				client = intClient;
//				intClient.connect(intSettings.ws_user,intSettings.ws_pwd,function(frame) 
//					{
//					subscribe(intSettings.ws_topic,intClient);
//					},
//					function(frame) 
//					{
////						cont = cont +1;
////						if (cont<5) {
////							intClient = Stomp.client(settings.ws_url);
////							createConnection(intSettings, intUpdateCallback,intClient);
////						} else 
//						//ert('Impossibile connettersi');
//					});
//			}
			
//			function subscribe(wstopic,intClient)
//			{
//						intClient.subscribe(wstopic,
//									function(message) {
//										jsonMessage = JSON.parse(message.body);
//										updateCallback(jsonMessage);
//									}
//						);
//			}
			
			self.onSettingsChanged = function(newSettings)
			{
				currentSettings = newSettings;
			}
			self.updateNow = function()
			{
//				createConnection(currentSettings,updateCallback,client);
			}
			self.onDispose = function()
			{
				client.disconnect();
			}
			
			//add Callback to Queue for subscription once Connected
		

//			createConnection(currentSettings,updateCallback);
			
		}
}());