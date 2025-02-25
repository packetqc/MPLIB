#pragma once
// #ifndef NETWORKS_H
// #define NETWORKS_H

#include <Arduino.h>

#include <SPI.h>
#include <WiFiNINA.h> 
#include <NTPClient.h>

#include <ArduinoECCX08.h>
#include <ArduinoBearSSL.h>
#include <ArduinoMqttClient.h>

#include <ArduinoJson.h>

#include "main.h"

#include "arduino_secrets.h"

const char ssid[]        = SECRET_SSID;
const char pass[]        = SECRET_PASS;
// const char* certificate  = SECRET_CERTIFICATE;
const uint8_t device_id        = DEVICE_ID;

#define SIZEOFJSON 200

#define MQTT_TIMEOUT 15000
#define MQTT_CONNECT_RETRY 5
#define WIFI_CONNECT_RETRY 5

    // unsigned long lastcheck;
    // unsigned long lastcheckrequest;

    // unsigned long lastcheck_ms = 60000*5;


class Networks {
  private:
    Networks() = default;
    void checkSE();
    String getStringByteArray( byte anarray, int sizeArray);

  public:
    static Networks &getInstance();
    Networks(const Networks &) = delete;
    Networks &operator=(const Networks &) = delete;

  public:
    uint8_t begin(NTPClient* timeClient, BearSSLClient* sslClient, MqttClient* mqttClient);
    uint8_t end();
    
    uint8_t check(bool now=false);

    void checkMessages();
    bool connectMQTT();
    int  getConnectError();
    void setPowerMode();
    bool connectWiFi();
    void setupConnection();
    void closeConnection();    

    void onMessageReceived(int messageSize);
    void publishOutgoing(String outgoing);
    void publishMessage(String deviceId, String message);
    void publishMessage(String deviceId, char* message);

    void printWiFiData();
    void printCurrentNet();
    void printMacAddress();
    void printBSSID();    

    int pingServices();

  public:  
    String device_str;

    // Debug* LOGGER;
    NTPClient* timeClient;
    // Devices* HW;
    BearSSLClient* sslClient;
    MqttClient* mqttClient;
    String mqttBroker = SECRET_BROKER;

    unsigned long lastcheck;
    unsigned long lastcheck_ms = 60000*2;

    bool started = false;
    char topic[15];                  //toxic/+/air
    StaticJsonDocument<200> incomingJson;

    bool noLowPowerMode = true;
    byte mac[6];
    IPAddress ip;
    IPAddress mask;
    IPAddress gateway;
    byte bssid[6];
    long rssi;
    byte encryptionType;
};


extern Networks &NET;

// #endif