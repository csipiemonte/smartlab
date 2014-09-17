$(document).ready(function(){
  var client, destination;

  $('#connect_form').submit(function() {
/*
    var host = $("#connect_host").val();    
    var port = $("#connect_port").val();
    var clientId = $("#connect_clientId").val();
    destination = $("#destination").val();
*/
    var host = "localhost";
    var port = 61614;
    var clientId = "clientId";
    destination = "/test";
    
    client = new Messaging.Client(host, Number(port), clientId);

    client.onConnect = onConnect;
  
    client.onMessageArrived = onMessageArrived;
    client.onConnectionLost = onConnectionLost;            

    client.connect({onSuccess:onConnect, onFailure:onFailure}); 
    return false;
  });  

    // the client is notified when it is connected to the server.
    var onConnect = function(frame) {
      debug("connected to MQTT");
      $('#connect').fadeOut({ duration: 'fast' });
      $('#disconnect').fadeIn();
      $('#send_form_input').removeAttr('disabled');
      client.subscribe(destination);
    };  

    // this allows to display debug logs directly on the web page
    var debug = function(str) {
      $("#debug").append(document.createTextNode(str + "\n"));
    };  

  $('#disconnect_form').submit(function() {
    client.disconnect();
    $('#disconnect').fadeOut({ duration: 'fast' });
    $('#connect').fadeIn();
    $('#send_form_input').addAttr('disabled');
    return false;
  });

  $('#send_form').submit(function() {
    var text = $('#send_form_input').val();
    if (text) {
      message = new Messaging.Message(text);
      message.destinationName = destination;
      client.send(message);
      $('#send_form_input').val("");
    }
    return false;
  });

  function onFailure(failure) {
    debug("failure");
    debug(failure.errorMessage);
  }  

  function onMessageArrived(message) {
    var p = document.createElement("p");
    var t = document.createTextNode(message.payloadString);
    p.appendChild(t);
    $("#messages").append(p);
  }

  function onConnectionLost(responseObject) {
    if (responseObject.errorCode !== 0) {
      debug(client.clientId + ": " + responseObject.errorCode + "\n");
    }
  }

});