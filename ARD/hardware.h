#pragma once
// #ifndef HARDWARE_H
// #define HARDWARE_H

#include "Arduino.h"

#include "main.h"
// #include "debug.h"

// #define HW_RESTART_ON_FAILURE 0

enum {
  BLUE,
  RED,
  WHITE,
  GREEN,
  YELLOW,
  PURPLE,
  CYAN,
  OFF
};

class Hardware {
  private:
    Hardware() = default;

  public:
    static Hardware &getInstance();
    Hardware(const Hardware &) = delete;
    Hardware &operator=(const Hardware &) = delete;

  public:
    // Hardware(Debug* logger,HardwareSerial* serial);
    // ~Hardware();

    uint8_t begin();
    uint8_t end();

    void check(bool now=false);
    void checkSerial();
    void checkMemory();
    void checkMemoryFree();

    void ledBlue();           //NETWORKS ACTIVITIES
    void ledRed();            //NOT CONNECTED TO SERVICES
    void ledWhite();          //CONNECTED TO SERVICES, STATUS OK, LOGGER OK, LOGGER ACTIVITIES
    void ledGreen();          //DEVICES DATA CAPTURE 
    void ledYellow();         //NETWORKS CHECKS
    void ledPurple();         //DEVICES ACTIVITIES
    void ledCyan();           //HARDWARE CHECKS ACTIVITIES
    void ledOff();            //OFF FOR NEXT STATUS COLOR
    void ledFlash(uint8_t color, uint8_t manytimes);

    // bool readyPublishData();
    // String  publishData();
  
  protected:
    // String device_id="0";
    // Debug* LOGGER;
    // HardwareSerial* Serial;
    uint8_t started = false;
    bool checkrequested = false;
    unsigned long lastcheck;
    unsigned long lastcheckrequest;

    int freememory = 0;
};

extern Hardware &HW;

// #endif