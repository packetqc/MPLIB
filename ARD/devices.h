#pragma once
// #ifndef DEVICES_H
// #define DEVICES_H

#include "Arduino.h"
#include <Adafruit_DPS310.h>
#include "Air_Quality_Sensor.h"

#include "sensirion_common.h"
#include "sgp30.h"

#include "main.h"
#include "config.h"

#define heaterSelPin 15

/* ALCOHOL AIR
According to the figure, we can see that the minimum concentration we can test is 20ppm and the maximum is 10000ppm, 
in a other word, we can get a concentration of gas between 0.002% and 1%. However, we can't provide a formula because 
the relation between ratio and concentration is nonlinear.

Notes

a. The value varies between 500 - 905. Hence any value above 650 indicates alcohol vapor in the vicinity.
b. Once exposed to alcohol vapor, it takes some time for the sensor value to decrease completely.
c. Yet, any new exposure will show instant increase in sensor value.

Caution

a. Alcohol sensor is very sensitive semiconductor device. Handle with care.
b. Do not expose to organic silicon steam, alkali or corrosive gases.
c. Do not use freeze or spill water.
d. Maintain proper working voltage.
*/

struct alcohol_data_struct {
  float sensor_volt;
  float RS_air;       //  Get the value of RS via in a clear air
  float sensorValue;
  float RS_gas;       // Get value of RS in a GAS
  float ratio;        // Get ratio RS_GAS/RS_air
};

struct vocec02_data_struct {
  unsigned short tvoc_ppb;
  unsigned short co2_eq_ppm;
};

class Devices {
  private:
    Devices() = default;
  // public:
  //   Devices(Debug* logger);
  //   ~Devices();

  public:
    static Devices &getInstance();
    Devices(const Devices &) = delete;
    Devices &operator=(const Devices &) = delete;

  public:
    uint8_t begin(String device_id);
    uint8_t end();

    void initAlcoholSensor();
    
    void check(bool now=false);
    void checkDPS();
    void checkDUST();
    void checkAQ();
    void checkALCOHOL();
    void checkVOCCO2();

    bool  getData();
    float getMaxPressure();
    float getMaxDust();
    float getMaxTemperature();
    int   getMaxAirValue();
    int   getMaxAirValue2();
    int   getMaxAir();
    int   getMaxAir2();
    float getMaxAlcohol();
    float getAvgAlcohol();
  
    // float getMaxVOCCO2();
    unsigned short getMaxVOC();
    unsigned short getMaxCO2();

    int getMaxHumidity();
    bool getMovement();

    bool readyPublishData();
    String  publishDataString();
    char* publishData();
  
  protected:
    // Debug* LOGGER;
    HardwareSerial* Serial;
    uint8_t started = false;
    bool checkrequested = false;
    unsigned long lastcheck;
    unsigned long lastcheckrequest;

    int freememory = 0;

  public:
    String device_id="0";
    const int humidity_pin = A3;
    const int air_quality_pin = A1;
    const int air_quality2_pin = A2;
    const int air_alcohol_pin = A0;

    const int dust_sensor_pin = 0;
    const int movement_pin = 1;

    bool have_dps = false;
    bool have_alcohol = false;
    bool have_vocco2 = false;
    bool have_aq = false;
    bool have_lcd = false;
    bool have_dust = false;
    bool have_temperature = false;
    bool have_baro = false;
    bool have_humidity = false;
    bool have_movement = false;
    // uint8_t nosgp30 = 0;
    // bool have_sgp30 = false;
  
    float ratio = 0;
    float RS_air = 13.30; //  Get the value of RS via in a clear air
    
    float sensor_volt;
    float sensorValue = 0;
  
    char* message;//[MQTT_MESSAGE_BUFFER];// = {'\0'};
   
    int HUMIDITY[MAXAIR_DATA];// = {-1};
    bool MOVEMENT[MAXAIR_DATA];// = {false};
    int AIRDATA[MAXAIR_DATA];// = {-1};
    int AIRDATA2[MAXAIR_DATA];// = {-1};
    int AIRVALUE[MAXAIR_DATA];// = {-1};
    int AIRVALUE2[MAXAIR_DATA];// = {-1};    
    alcohol_data_struct ALCOHOLDATA[MAXAIR_DATA];
    // ALCOHOLDATA[MAXAIR_DATA] = {0.010001};
    vocec02_data_struct VOCCO2DATA[MAXAIR_DATA];
    // float VOCCO2DATA[MAXAIR_DATA] = {0.010001};
    float DUSTDATA[MAXAIR_DATA];// = {0.010001};
    float TEMPDATA[MAXAIR_DATA];// = {0.010001};
    float PRESSUREDATA[MAXAIR_DATA];// = {0.010001};


    String        time_stamp          = "-1";
    String        air_quality_desc    = "INIT";
    String        air_quality_desc2    = "INIT";
    String        display_text        = "";


    bool maxairdatareached = false;
    bool displayair = false;
    int airreadingcount = 0;
    int lastairreadingcount = 0;


    unsigned long lastMillis          = 0;
    bool          movement            = false;
    int           humidity            = 0;
    int           air_quality         = 0;
    int           air_quality2          = 0;
    int           air_value          = 0;
    int           air_value2         = 0;
    float         air_alcohol         = 0;
    unsigned short air_voc          = 0;
    unsigned short air_co2          = 0;
    int           last_air_quality    = 0;
    int           last_air_quality2    = 0;
    float         dust_concentration  = 0.010001;
    float         pressure            = 0.010001;
    float         temperature         = 0.010001;
    float         lastpressure            = 0.010001;
    float         lasttemperature         = 0.010001;

    int lots2go = NUMBER_LOTS;
    int samples2go = NUMBER_SAMPLES;

    unsigned long duration;
    unsigned long starttime;
    unsigned long lasttime;
    unsigned long lastupdatetime;
    unsigned long lastupdatenetworktime;
    unsigned long lastcheckmessages;

    char oled_text[17];
   
    sensors_event_t temp_event, pressure_event;
    Adafruit_DPS310 dps;
    Adafruit_Sensor *dps_temp;// = dps.getTemperatureSensor();
    Adafruit_Sensor *dps_pressure; // = dps.getPressureSensor();

    unsigned long lowpulseoccupancy = 0;
    s16 err;
    u16 scaled_ethanol_signal, scaled_h2_signal;
};

extern Devices &DEVICES;
// #endif