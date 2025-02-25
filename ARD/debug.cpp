#include "debug.h"
#include <Arduino.h>

Debug &Debug::getInstance() {
  static Debug instance;
  return instance;
}

// Debug::Debug(HardwareSerial* serial, NTPClient* timeClient) {
//   this->timeClient = timeClient;
//   this->stream = serial;
//   this->serial = serial;
// }

// Debug::Debug() {
  
// }

// Debug::~Debug() {
  
// }

void Debug::haveSerial(bool haveit) {
  have_serial = haveit;
}

// void Debug::updateSerial(HardwareSerial* serial) {
//   this->stream = serial;
//   this->serial = serial;
//   have_serial = true;
// }

uint8_t Debug::begin(NTPClient *timeClient) {
  started = true;
  this->timeClient = timeClient;

  Serial.begin(115200);
  delay(3000);

  if(!Serial) {
    haveSerial(false);
  }
  else {      
    haveSerial(true);
    log("LOGGER", "Has begun");
  }

  digitalWrite(resetPin, HIGH);
  delay(200);
  pinMode(resetPin, OUTPUT);

  return( started );
}

uint8_t Debug::end() {
  started = false;

  // stream->println("HW as ended");
  if(have_serial) {
    log("LOGGER", "Has ended");
  }

  return( !started );
}

void Debug::debug(String header, String alog, String tail) {
  if(DEBUG) {
    if(have_serial) {    
      Serial.print(F(timeClient->getFormattedTime().c_str()));
      Serial.print(F("\t"));
      Serial.print(F(header.c_str()));
      Serial.print(F("\t"));
      Serial.print(F(alog.c_str()));
      Serial.print(F(" "));
      Serial.println(F(tail.c_str()));
    }
  }
  // else {
  //   lostserial = true;
  // }
}

void Debug::logEmptyLine() {
  // if(stream->available()) {
    // stream->println("");
  if(have_serial) {     
    Serial.println(F(""));
  }
}
// void Debug::updateNetworks(Networks* net) {
//   this->NET = net;
// }

void Debug::log(String header, String alog, String tail) {
  if(have_serial) {    
    Serial.print(F(timeClient->getFormattedTime().c_str()));
    Serial.print(F("\t"));
    Serial.print(F(header.c_str()));
    Serial.print(F("\t"));
    Serial.print(F(alog.c_str()));
    Serial.print(F(" "));
    Serial.println(F(tail.c_str()));
  }
}

void Debug::reset(String reason) {
  log("MONITORING", "Sending reset status monitoring");
  // NET->publishOutgoing(reason);

  log("MONITORING", "Resetting now !");
  digitalWrite(resetPin, LOW);
}

void Debug::check() {
  // if(!stream->available()) {
  //   lostserial = true;
  // }
  // else {
  //   lostserial = false;
  // }
}

void Debug::publishMessage(String messageStr) {

}

void Debug::log(String header, String alog, int anumber, String tail) {
  String snumber(anumber);
  log(header,alog+snumber,tail);
}

void Debug::log(String header, String alog, float anumber, String tail) {
  String snumber(anumber);
  log(header,alog+snumber,tail);  
}

void Debug::log(String header, String alog, unsigned long anumber, String tail) {
  String snumber(anumber);
  log(header,alog+snumber,tail);  
}

void Debug::log(String header, String alog, long anumber, String tail) {
  String snumber(anumber);
  log(header,alog+snumber,tail);  
}

void Debug::log(String header, int anumber, String tail) {
  String snumber(anumber);
  log(header,snumber,tail);
}

void Debug::log(String header, float anumber, String tail) {
  String snumber(anumber);
  log(header,snumber,tail);
}


Debug &LOGGER = LOGGER.getInstance();
