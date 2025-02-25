#include "networks.h"
#include "hardware.h"
#include "debug.h"

// void Networks::callback_incoming(int messageSize) {
//   onMessageReceived(messageSize);
// }

// unsigned long callback_gettime() {
//   // return WiFi.getTime()+TIME_ZONE;
//   return WiFi.getTime();
//   // return timeClient.getEpochTime();
// }

// Networks::Networks(Debug* LOGGER, NTPClient* timeClient, BearSSLClient* sslClient, MqttClient* mqttClient) {
uint8_t Networks::begin(NTPClient* atimeClient, BearSSLClient* asslClient, MqttClient* amqttClient) {
  started = true;

  // this->LOGGER = &LOGGER;
  // this->HW = HW;
  timeClient = atimeClient;
  sslClient = asslClient;
  mqttClient = amqttClient;

  LOGGER.log("NETWORKS","Has begun");

  return( started );
}

// Networks::Networks() {

// }

// Networks::~Networks() {
  
// }

uint8_t Networks::check(bool now) {
  uint8_t retour = 1;

  if((millis() - lastcheck > lastcheck_ms) || now ) {
    lastcheck = millis();
    LOGGER.log("NETWORKS","Networks checks...");

    checkSE();

    setPowerMode();

    if (!mqttClient->connected()) {
      LOGGER.log("NETWORKS","Not connected with the MQTT server");
      if (WiFi.status() != WL_CONNECTED) {
        LOGGER.log("NETWORKS","Not connected with the WiFi network");
        LOGGER.log("NETWORKS","WiFi not connected reason code:", WiFi.reasonCode());
        closeConnection();
        setupConnection();
      }
      else {
        LOGGER.log("NETWORKS","But connected with the WiFi network");
        LOGGER.log("NETWORKS","Still investigating...");
        
        rssi = WiFi.RSSI();
        LOGGER.log("NETWORKS","WiFi signal strength(rssi):", rssi);

        pingServices();

        LOGGER.log("NETWORKS","Restarting WiFi as well...");
        closeConnection();
        setupConnection();
        connectMQTT();
      }
    }

    LOGGER.log("NETWORKS","Networks checks completed", "\n");
  }
  return(retour);
}

void Networks::checkSE() {
  LOGGER.log("NETWORKS","Checking ECCX08 status...");

  if (!ECCX08.begin()) {
    ECCX08.end();
    LOGGER.log("NETWORKS","No ECCX08 present! Restarting ECCX08...");
  }

  if (!ECCX08.begin()) {
    LOGGER.log("NETWORKS","No ECCX08 still present! Ending program!");
    while (1);
  }
  
  LOGGER.log("NETWORKS","ECCX08 status checked");
}

// uint8_t Networks::begin(Debug* LOGGER, NTPClient* timeClient) {
//   started = true;

//   this->LOGGER = LOGGER;
//   // this->HW = HW;
//   this->timeClient = timeClient;

//   LOGGER.log("NETWORKS","Has begun");

//   return( started );
// }

// uint8_t Networks::begin() {
//   started = true;

//   LOGGER.log("NETWORKS","Has begun");

//   return( started );
// }

uint8_t Networks::end() {
  started = false;

  LOGGER.log("NETWORKS","Has ended");

  return( started );
}

// void Networks::getSetData() {
//   check(true);
//   // if (WiFi.status() != WL_CONNECTED) {
//   //   closeConnection();
//   //   setupConnection();
//   // }

//   publishMessage();
// }

void Networks::closeConnection() {
  // LOGGER.log("NETWORKS","Closing connections...");

  WiFi.end();
  
  // ECCX08.end();

  // sslClient->flush();
  // sslClient->stop();
  
  // mqttClient->flush();
  // mqttClient->stop();

  LOGGER.log("NETWORKS","Connections closed");
}

void Networks::setupConnection() {
  bool status = true;

  LOGGER.log("NETWORKS","Starting connections...");

  String fv = WiFi.firmwareVersion();
  LOGGER.log("NETWORKS", "WiFi Firmware version is:", fv.c_str());

  checkSE();

  // Set a callback to get the current time
  // used to validate the servers certificate
  //MP ArduinoBearSSL.onGetTime(getTime);
  //MP ArduinoBearSSL.onGetTime(timeClient->getEpochTime);
  

  // Set the ECCX08 slot to use for the private key
  // and the accompanying public certificate for it
  // sslClient->setEccSlot(1, certificate);  

  // Optional, set the client id used for MQTT,
  // each device that is connected to the broker
  // must have a unique client id. The MQTTClient will generate
  // a client id for you based on the millis() value if not set
  //
  // mqttClient->setId("clientId");
  mqttClient->setId(String(DEVICE_ID).c_str());

  // Set the message callback, this function is
  // called when the MQTTClient receives a message
  //MP mqttClient->onMessage(onMessageReceived);  


  if (WiFi.status() != WL_CONNECTED) {
    status = connectWiFi();
    if(status) {
      LOGGER.log("NETWORKS","WiFi connected");
      timeClient->begin();      
      LOGGER.log("NETWORKS","NTP updating time...");
      timeClient->update();
      LOGGER.log("NETWORKS","NTP up to date");      
      LOGGER.log("NETWORKS","Network time protocol connected");
    }
    else {
      LOGGER.log("NETWORKS","WiFi **NOT** connected");
    }
  }

  // delay(3000);

  if (!mqttClient->connected() && status ) {
    // sslClient->setEccSlot(1, SECRET_CERTIFICATE);
    // MQTT client is disconnected, connect
    if(connectMQTT()) {
      LOGGER.log("NETWORKS","MQTT connection started", "\n");
    }
    else {
      LOGGER.log("NETWORKS","MQTT connection failed", "on hold networking until next checks\n");
    }
  }
}

void Networks::setPowerMode() {
    if(noLowPowerMode) {
    LOGGER.log("NETWORKS","Disabeling WiFi Low Power Mode");
    WiFi.noLowPowerMode();
  }
  else {
    LOGGER.log("NETWORKS","Enabeling WiFi Low Power Mode");
    WiFi.lowPowerMode();
  }
}

bool Networks::connectWiFi() {
  bool retour = true;

  setPowerMode();

  LOGGER.log("NETWORKS","Attempting to connect to SSID: ", ssid);

  int retryWiFi = 0;
  while (WiFi.begin(ssid, pass) != WL_CONNECTED ) {
    // WiFi.end();    
    delay(3000);
    retryWiFi++;
    LOGGER.log("NETWORKS","Connection retry with the WiFi network: ", retryWiFi);
    if(retryWiFi > WIFI_CONNECT_RETRY) {
      retour = false;
      break;
    }
  }
  
  if(!retour) {
    LOGGER.log("NETWORKS","You are NOT connected to the network: ", "will retry at next network check round, will focus to retreive data then...");
  }
  else {
    LOGGER.log("NETWORKS","You're connected to the network\n");
    HW.ledFlash(BLUE, 5);
    // ip = WiFi.localIP();
    // WiFi.macAddress(mac);
    // WiFi.BSSID(bssid);
    // rssi = WiFi.RSSI();
    // encryptionType = WiFi.encryptionType();
    // LOGGER.log("NETWORKS","WiFi signal strength(rssi):", rssi);

    printMacAddress();
    printWiFiData();
    printCurrentNet();

    pingServices();
    // retour = true;
  }
  return(retour);
}

void Networks::checkMessages() {
  // poll for new MQTT messages and send keep alives
  // Serial.print(timeClient->getFormattedTime());
  // Serial.println("\tPolling mqtt messages...");
  
  mqttClient->poll();  
  
  // Serial.print(timeClient->getFormattedTime());
  // Serial.println("\tMqtt messages polled");
}

int Networks::getConnectError() {
  int connectError = mqttClient->connectError();

  switch(connectError) {
    case MQTT_CONNECTION_REFUSED:
      LOGGER.log("NETWORKS","Connection refused by the MQTT broker");
      break;
    case MQTT_CONNECTION_TIMEOUT:
      LOGGER.log("NETWORKS","Connection timeout with the MQTT broker");
      break;
    case MQTT_SUCCESS:
      LOGGER.log("NETWORKS","Connection success with the MQTT broker, from connectError code...");
      break;
    case MQTT_UNACCEPTABLE_PROTOCOL_VERSION:
      LOGGER.log("NETWORKS","Connection with unacceptable protocol version with the MQTT broker");
      break;
    case MQTT_IDENTIFIER_REJECTED:
      LOGGER.log("NETWORKS","Connection identifier rejected by the MQTT broker");
      break;
    case MQTT_SERVER_UNAVAILABLE:
      LOGGER.log("NETWORKS","Connection incompleted: ", "MQTT broker server unavailable");
      break;
    case MQTT_BAD_USER_NAME_OR_PASSWORD:
      LOGGER.log("NETWORKS","Connection refused: ", "bad user or password");
      break;
    case MQTT_NOT_AUTHORIZED:
      LOGGER.log("NETWORKS","Connection not authorized by the MQTT broker");
      break;
    default:
      LOGGER.log("NETWORKS","Undefined connect error status with the MQTT broker ****");
      break;
  }

  return(connectError);
}

bool Networks::connectMQTT() {
  bool retour = false;
  int retry = 0;

  LOGGER.log("MQTT CLIENT","Attempting to MQTT broker: ", mqttBroker);  

  // mqttClient->setConnectionTimeout(MQTT_TIMEOUT);
  mqttClient->setConnectionTimeout(10000);

  while(retry < MQTT_CONNECT_RETRY) {
    // LOGGER.log("MQTT CLIENT","Connecting...");  
    // HW.ledFlash(YELLOW, 2);

    try {
      HW.ledFlash(YELLOW, 2);
      if(!mqttClient->connect(mqttBroker.c_str(), 8883)) {
        LOGGER.log("MQTT CLIENT","Connection error with the MQTT broker"); 
        HW.ledFlash(YELLOW, 3);
        mqttClient->stop();
        // getConnectError();
      }
      else {      
        if(retry == 0) {
          LOGGER.log("MQTT CLIENT","Connection success at once with the MQTT broker");
        }
        else {
          LOGGER.log("MQTT CLIENT","Connection success with the MQTT broker on ", retry, " retries");
        }
        retour = true;
        break;
      }
    } catch( String error ) {
      LOGGER.log("MQTT CLIENT", "Catched Error on connect");
      HW.ledFlash(YELLOW, 5);
      // getConnectError();
    }

    delay(5000);
    retry = retry + 1;
    LOGGER.log("MQTT CLIENT","Connection retry with the MQTT broker: ", retry);
  }

  if(!mqttClient->connected()) {
    retour = false;
    LOGGER.log("MQTT CLIENT","You are NOT connected to the MQTT broker: ", "will retry at next network check round, will focus to retreive data then...");
  }
  else {
    LOGGER.log("MQTT CLIENT","You're connected to the MQTT broker");
    HW.ledFlash(WHITE, 5);

    mqttClient->setTxPayloadSize(MQTT_MESSAGE_BUFFER);
    LOGGER.log("MQTT CLIENT","MQTT tx payload is set to:", (int)MQTT_MESSAGE_BUFFER);

    LOGGER.log("MQTT CLIENT","Subscribing to incoming messages...");

    String atopic;
    atopic = "toxicair/incoming";
    mqttClient->subscribe(atopic);
    LOGGER.log("MQTT CLIENT","Subscribed to: ", atopic);
    
    atopic = "toxicair/"+device_str+"/incoming";
    mqttClient->subscribe(atopic);
    LOGGER.log("MQTT CLIENT","Subscribed to: ", atopic);
    LOGGER.log("MQTT CLIENT","Subscribed to incoming messages","\n");
    LOGGER.log("MQTT CLIENT","MQTT connection completed with success", "\n");
  }
  
  return(retour);
}

void Networks::publishMessage(String deviceId, char* messageToSend) {
  String atopic; 

  LOGGER.log("Publish Message v1: ","starting");

  atopic = "toxic/"+deviceId+"/air";
  
  LOGGER.log("MQTT message size is:", (int)strlen(messageToSend), " octets");

  if(mqttClient->connected()) {
    LOGGER.log("Publishing message to: ", atopic);
    mqttClient->beginMessage(atopic);
    mqttClient->print(messageToSend);
    mqttClient->endMessage();
    LOGGER.log("Published message to: ", atopic);
    // LOGGER.log(String(messageToSend),"\n");
    HW.ledFlash(RED, 3);
  }
  else {
    LOGGER.log("MQTT CLIENT", "Not connected with the MQTT broker: ", "Skip publish message for now...");
  }
  LOGGER.log("Publish Message v1: ","ended\n");
}

void Networks::publishMessage(String deviceId, String message) {
  String atopic; 

  LOGGER.log("Publish Message v2: ","starting");

  atopic = "toxic/"+deviceId+"/air";

  if(mqttClient->connected()) {
    LOGGER.log("Publishing message to: ", atopic);
    // LOGGER.log(message,"\n");
    LOGGER.log("MQTT message size is:", (int)message.length(), " octets");
    mqttClient->beginMessage(atopic);
    mqttClient->print(message.c_str());
    mqttClient->endMessage();
    LOGGER.log("Published message to: ", atopic);
    HW.ledFlash(RED, 3);
  }
  else {
    LOGGER.log("MQTT CLIENT", "Not connected with the MQTT broker: ", "Skip publish message for now...");
  }
  LOGGER.log("Publish Message v2: ","ended\n");
}

void Networks::publishOutgoing(String outgoing) {
  char message[MQTT_MESSAGE_BUFFER] = {'\0'};

  LOGGER.log("Publish Outgoing: ","starting");

  sprintf(message, "{ \"time_stamp\": \"%s\", \"message\": \"%s\" }", timeClient->getEpochTime(), outgoing.c_str() );

  String atopic; 
  atopic = "toxicair/"+device_str+"/outgoing";

  if(mqttClient->connected()) {
    LOGGER.log("Publishing message to: ", atopic);
    // LOGGER.log(message,"\n");

    mqttClient->beginMessage(atopic);
    mqttClient->print(message);
    mqttClient->endMessage();
    LOGGER.log("Published message to: ", atopic);
    HW.ledFlash(RED, 4);
  }
  else {
    LOGGER.log("MQTT CLIENT", "Not connected with the MQTT broker: ", "Skip publish outgoing for now...");
  }
  LOGGER.log("Publish Outgoing: ","ended\n");
}

void Networks::onMessageReceived(int messageSize) {
  // we received a message, print out the topic and contents
  LOGGER.log("Received a message with topic: ", mqttClient->messageTopic());
  HW.ledFlash(RED, 5);

  int i = 0;
  char message[messageSize];
  // use the Stream interface to print the contents
  while (mqttClient->available()) {
    // Serial.print((char)mqttClient->read());
    message[i] = (char)mqttClient->read();
    i++;
  }

  LOGGER.log(message,"\n");

  if( messageSize > SIZEOFJSON ) {
    LOGGER.log("MQTT CLIENT","Message received is bigger than allowed size: ", "cannot process message");
  }
  else {
    DeserializationError error = deserializeJson(incomingJson,message);

    if(error) {
      LOGGER.log("MQTT CLIENT", "deserializeJson:", "failed");
      LOGGER.log("MQTT CLIENT", "error message:", error.c_str());
    }

    if(incomingJson.containsKey("reset")) {      
      if(incomingJson["reset"] == true) {
        LOGGER.log("MQTT CLIENT", "Received reset command");
        LOGGER.log("MQTT CLIENT", "Reseting...");
        LOGGER.reset("Processing reset command received");
        // reset();
      }
    }
  }
}

void Networks::printWiFiData() {
  ip = WiFi.localIP();
  WiFi.macAddress(mac);
  mask = (IPAddress)WiFi.subnetMask();
  gateway = (IPAddress)WiFi.gatewayIP();

  LOGGER.log("NETWORKS","IP Address:", ip );
  LOGGER.log("NETWORKS","Network mask:", mask );
  LOGGER.log("NETWORKS","Network gateway:", gateway );

  printMacAddress();

  // // print your board's IP address:
  // IPAddress ip = WiFi.localIP();
  // Serial.print("IP address : ");
  // Serial.println(ip);

  // Serial.print("Subnet mask: ");
  // Serial.println((IPAddress)WiFi.subnetMask());

  // Serial.print("Gateway IP : ");
  // Serial.println((IPAddress)WiFi.gatewayIP());

  // // print your MAC address:
  // byte mac[6];
  // WiFi.macAddress(mac);
  // Serial.print("MAC address: ");
  // printMacAddress(mac);
}

void Networks::printCurrentNet() {
  WiFi.BSSID(bssid);
  rssi = WiFi.RSSI();
  encryptionType = WiFi.encryptionType();
  
  LOGGER.log("NETWORKS","WiFi SSID:", WiFi.SSID());
  // // print the SSID of the network you're attached to:
  // Serial.print("SSID: ");
  // Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  // byte bssid[6];
  // WiFi.BSSID(bssid);
  // Serial.print("BSSID: ");
  printBSSID();
  // // print the received signal strength:
  // long rssi = WiFi.RSSI();
  // Serial.print("signal strength (RSSI): ");
  // Serial.println(rssi);
  LOGGER.log("NETWORKS","WiFi signal strength(rssi):", rssi);

  // print the encryption type:
  // byte encryption = WiFi.encryptionType();
  // Serial.print("Encryption Type: ");
  // Serial.println(encryption, HEX);
  // Serial.println();
  LOGGER.log("NETWORKS","WiFi encryption type:", (int)encryptionType);
}

void Networks::printMacAddress() {
  // for (int i = 5; i >= 0; i--) {
  //   if (mac[i] < 16) {
  //     Serial.print("0");
  //   }
  //   Serial.print(mac[i], HEX);
  //   if (i > 0) {
  //     Serial.print(":");
  //   }
  // }
  // Serial.println();  
}


void Networks::printBSSID() {

}

String Networks::getStringByteArray( byte anarray, int sizeArray) {
  String retour;
  // for (int i = 5; i >= 0; i--) {
  //   if (mac[i] < 16) {
  //     Serial.print("0");
  //   }
  //   Serial.print(mac[i], HEX);
  //   if (i > 0) {
  //     Serial.print(":");
  //   }
  // }
  // Serial.println();
  return(retour);
}

int Networks::pingServices() {
  int retour = WL_PING_ERROR;

  LOGGER.log("NETWORKS", "Network check to reach services:", mqttBroker.c_str());
  // Serial.print("Pinging ");
  // Serial.print(mqttBroker.c_str());
  // Serial.print(": ");

  retour = WiFi.ping(mqttBroker.c_str());

  if (retour >= 0) {
    LOGGER.log("NETWORKS", "Reached in ", retour, " ms");
    HW.ledFlash(BLUE, 3);

    // Serial.print("SUCCESS! RTT = ");
    // Serial.print(retour);
    // Serial.println(" ms");
    // // retour = true;
  } else {
    LOGGER.log("NETWORKS", "**** Failed to reach services with code:", retour);
    HW.ledFlash(BLUE, 5);
    switch(retour) {
      case WL_PING_DEST_UNREACHABLE:
      LOGGER.log("NETWORKS", "Network destination unreachable");
      break;
      case WL_PING_TIMEOUT:
      LOGGER.log("NETWORKS", "Network timeout to reach services");
      break;
      case WL_PING_UNKNOWN_HOST:
      LOGGER.log("NETWORKS", "Unknown host to reach by network");
      break;
      case WL_PING_ERROR:
      LOGGER.log("NETWORKS", "Network error to reach services");
      break;
      default:
      LOGGER.log("NETWORKS", "Cannot reach services for unknown reason");
      break;
    }
    // Serial.print("FAILED! Error code: ");
    // Serial.println(retour);
  }

  return(retour);
}

Networks &Networks::getInstance() {
  static Networks instance;
  return instance;
}

Networks &NET = NET.getInstance();
