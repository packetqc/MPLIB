#pragma once
// #ifndef Debug_H
// #define Debug_H

#include "Arduino.h"

#include "NTPClient.h"
// WiFiUDP       ntpUDP;
// // NTPClient timeClient(ntpUDP);
// // NTPClient     timeClient(ntpUDP, "ca.pool.ntp.org");
// NTPClient     timeClient(ntpUDP, "ca.pool.ntp.org", -(4*3600), 60000);
// ////// NTPClient  timeClient(ntpUDP, NTP_SERVER, TIME_ZONE, 60000);


#define DEBUG 1

class Debug {  
  private:
    // static Debug* pInstance;
    // Debug(HardwareSerial* serial, NTPClient* ntpclient);
    Debug() = default;
    // ~Debug();

  public:
    static Debug &getInstance();
    Debug(const Debug &) = delete;
    Debug &operator=(const Debug &) = delete;

  public:
    void reset(String reason);

    uint8_t begin(NTPClient *timeClient);
    uint8_t end();
    
    void  check();
    // void  updateSerial(HardwareSerial* serial);
    void  haveSerial(bool haveit);
    // void  updateNetworks(Networks* net);
    void  publishMessage(String message);

    void  debug(String header, String alog, String tail="");
    void  log(String header, String alog, String tail="");
    void  log(String header, int anumber, String tail="");
    void  log(String header, String alog, int anumber, String tail="");
    void  log(String header, float anumber, String tail="");
    void  log(String header, String alog, float anumber, String tail="");
    void  log(String header, String alog, unsigned long anumber, String tail="");
    void  log(String header, String alog, long anumber, String tail="");
    void  logEmptyLine();

  protected:
    HardwareSerial* serial;
    Stream* stream;
    uint8_t started = false;
    uint8_t resetPin = 7;
    bool have_serial = false;
    bool lostserial = false;

  public:
    // Networks* NET;
    NTPClient* timeClient;
};

extern Debug &LOGGER;
// extern NTPClient timeClient(ntpUDP, "ca.pool.ntp.org", -(4*3600), 60000);
// #endif