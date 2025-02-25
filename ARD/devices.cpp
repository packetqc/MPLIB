#include "devices.h"
#include "debug.h"
#include "hardware.h"

AirQualitySensor airSensor(A1);
AirQualitySensor airSensor2(A2);


// DEVICES ::DEVICES (Debug* LOGGER) {
//   this->LOGGER = LOGGER;

//   if(HAVE_HUMIDITY) {
//     have_humidity = true;
//   }

//   if(HAVE_MOVEMENT) {
//     have_movement = true;
//   }

//   if(HAVE_DUST) {
//     pinMode(dust_sensor_pin, INPUT);
//     have_dust = true;
//   }
  
//   //NEED TO DEFINE WHAT IS heaterSelPin ...
//   if(HAVE_ALCOHOL) {
//     // #define heaterSelPin = 15;
//     // pinMode(heaterSelPin, OUTPUT);
//     // digitalWrite(heaterSelPin,LOW); //start preheat sensor

//     // // pinMode(air_alcohol_pin, OUTPUT);
//     // // digitalWrite(air_alcohol_pin,LOW); //start preheat sensor
//       /*For wio link!*/
//     #if defined(ESP8266)
//     pinMode(heaterSelPin, OUTPUT);
//     digitalWrite(heaterSelPin, 1);
//     Serial.println("Set wio link power!");
//     delay(500);
//     #else
//     pinMode(heaterSelPin,OUTPUT);   // set the heaterSelPin as digital output.
//     digitalWrite(heaterSelPin,LOW); // Start to heat the sensor
//     #endif
//     have_alcohol = true;
//   }

//   if(HAVE_DPS) { have_dps = true; }
//   if(HAVE_AQ) { have_aq = true; }
//   if(HAVE_COVCO2) { have_vocco2 = true; }
//   if(HAVE_LCD) { have_lcd = true; }
//   if(HAVE_TEMP) { have_temperature = true; }
//   if(HAVE_BARO) { have_baro = true; }
// }

  // if(HAVE_DUST) {}
  // if(HAVE_ALCOHOL) {}
  // if(HAVE_DPS) {}
  // if(HAVE_AQ) {}
  // if(HAVE_COVCO2) {}
  // if(HAVE_LCD) {}
  // if(HAVE_TEMP) {}
  // if(HAVE_BARO) {}
  // if(HAVE_HUMIDITY) {}
  // if(HAVE_MOVEMENT) {}  

  // if(have_dust) {}
  // if(have_alcohol) {}
  // if(have_dps) {}
  // if(have_aq) {}
  // if(have_vocco2) {}
  // if(have_lcd) {}
  // if(have_temperature) {}
  // if(have_baro) {}
  // if(have_humidity) {}
  // if(have_movement) {}

// Devices::~DEVICES () {
  
// }

uint8_t Devices::begin(String device_id) {
  this->started = true;
  this->device_id = device_id;

  message = new char[MQTT_MESSAGE_BUFFER];

  if(HAVE_HUMIDITY) {
    have_humidity = true;
  }

  if(HAVE_MOVEMENT) {
    have_movement = true;
  }

  if(HAVE_DUST) {
    pinMode(dust_sensor_pin, INPUT);
    have_dust = true;
  }
  
  //NEED TO DEFINE WHAT IS heaterSelPin ...
  if(HAVE_ALCOHOL) {
      /*For wio link!*/
    #if defined(ESP8266)
    pinMode(heaterSelPin, OUTPUT);
    digitalWrite(heaterSelPin, 1);
    // Serial.println("Set wio link power!");
    delay(500);
    #else
    pinMode(heaterSelPin,OUTPUT);   // set the heaterSelPin as digital output.
    digitalWrite(heaterSelPin,LOW); // Start to heat the sensor
    #endif
    have_alcohol = true;

    if(have_alcohol) {
      LOGGER.log("DEVICES ", "Configured to have: ", "ALCOHOL");
      LOGGER.log("DEVICES ", "Waiting ALCOHOL sensor to init and get rs_air...");
      initAlcoholSensor();
    }
  }

  if(HAVE_DPS) { 
    LOGGER.log("DEVICES ", "Configured to have: ", "DPS");
    if (! dps.begin_I2C()) {    
      LOGGER.log("DEVICES ", "*ERROR*","Failed to find DPS");
      have_dps = false;
    }
    else {
      have_dps = true;
      LOGGER.log("DEVICES ", "DPS found and ok");
    }

    if( have_dps ) {
      dps_temp = dps.getTemperatureSensor();
      dps_pressure = dps.getPressureSensor();

      dps.configurePressure(DPS310_64HZ, DPS310_64SAMPLES);
      dps.configureTemperature(DPS310_64HZ, DPS310_64SAMPLES);
    }
  }

  if(HAVE_AQ) { 
    have_aq = true; 
    LOGGER.log("DEVICES ", "Configured to have: ", "AIR QUALITY");
    LOGGER.log("DEVICES ", "Waiting AIR sensors to init...(20 sec.)");
    // delay(20000);

    if (airSensor.init()) {
        LOGGER.log("DEVICES ", "AQ Sensor 1 ready.");
    } else {
        LOGGER.log("DEVICES ", "*ERROR*", "AQ Sensor 1 failed to init");        
    }

    if(HAVE_AQ >1) {
      if (airSensor2.init()) {
        LOGGER.log("DEVICES ", "AQ Sensor 2 ready.");
      } else {
        LOGGER.log("DEVICES ", "*ERROR*", "AQ Sensor 2 failed to init");        
      }
    }
  }

  if(HAVE_COVCO2) { 
    have_vocco2 = true; 
    LOGGER.log("DEVICES ", "Configured to have: ", "VOC eC02");
    /*  Init module,Reset all baseline,The initialization takes up to around 15 seconds, during which
    all APIs measuring IAQ(Indoor air quality ) output will not change.Default value is 400(ppm) for co2,0(ppb) for tvoc*/
    LOGGER.log("DEVICES ", "SGP30 ADDR: ", sgp_get_configured_address());
    while (sgp_probe() != STATUS_OK) {
        LOGGER.log("DEVICES ", "*ERROR*", "SGP failed");
        // while (1);
        have_vocco2 = false;
        break;
    }

    /*Read H2 and Ethanol signal in the way of blocking*/
    if(have_vocco2) {
      err = sgp_measure_signals_blocking_read(&scaled_ethanol_signal, &scaled_h2_signal);
      if (err == STATUS_OK) {
          LOGGER.log("DEVICES ", "VOCeC02", "get ram signal! Good.");
      } else {
          LOGGER.log("DEVICES ", "*ERROR*", "VOCeC02 reading signals");
      }
      err = sgp_iaq_init();
      if (err == STATUS_OK) {
          LOGGER.log("DEVICES ", "VOCeC02", "iaq init completed");
      } else {
          LOGGER.log("DEVICES ", "*ERROR*", "VOCeC02 iaq init not completed");
      }
    }
    else {
      LOGGER.log("DEVICES ", "WARNING", "no sgp30 VOCeC02 available");
    }
  }
  
  if(HAVE_LCD) { 
    have_lcd = true; 
    LOGGER.log("DEVICES ", "Configured to have: ", "LCD");
  }

  if(HAVE_TEMP) { 
    have_temperature = true;
    LOGGER.log("DEVICES ", "Configured to have: ", "TEMPERATURE"); 
  }
  
  if(HAVE_BARO) { 
    have_baro = true; 
    LOGGER.log("DEVICES ", "Configured to have: ", "BAROMETER");
  }


  //ORIGINAL BEGIN...
  LOGGER.log("DEVICES ", "Begins...");

  starttime = millis();              //get the current time;
  lastupdatetime = starttime;
  lastupdatenetworktime = starttime;
  lastcheck = millis();

  LOGGER.log("DEVICES ", "Have begun", "\n");
  
  return( started );
}

void Devices::initAlcoholSensor() {
    /*--- Get a average data by testing 100 times ---*/
    // for(int x = 0 ; x < 100 ; x++)
    // {
    //     sensorValue = sensorValue + analogRead(air_alcohol_pin);
    //     delay(100);
    // }
    // sensorValue = sensorValue/100.0;
    // /*-----------------------------------------------*/

    // sensor_volt = sensorValue/1024*5.0;
    // RS_air = sensor_volt/(5.0-sensor_volt); // omit *R16
    // LOGGER.log("DEVICES ", "sensor_volt = ", sensor_volt, "V");
    LOGGER.log("DEVICES ", "RS_air = ", RS_air);
}

uint8_t Devices::end() {
  started = false;

  LOGGER.log("DEVICES ", "Has ended");

  return( started );
}

void Devices::check(bool now) {
  if(now) {
    checkrequested = true;
  }
  else if(checkrequested) {
    if(millis() - lastcheck < lastcheck_ms) {
      checkrequested = false;
    }
  }

  if((millis() - lastcheck > lastcheck_ms) || checkrequested) {
    lastcheck = millis();
    LOGGER.log("DEVICES ","Checks...");

    checkDPS();
    checkDUST();
    checkAQ();

    LOGGER.log("DEVICES ","Checked", "\n");
    // if( ! checkrequested ) {
    //   LOGGER.logEmptyLine();
    // }
  }
  checkrequested = false;
}

void Devices::checkDPS() {
  LOGGER.log("DEVICES ","Checking DPS...");
  
  float pressureCheck = getMaxPressure();

  if(pressureCheck > 20000) {
    LOGGER.log("DEVICES ","Pressure check value is abnormal: ", "will initiate reset sequence of the DPS...");
    dps.reset();
    LOGGER.log("DEVICES ","DPS reset completed");
    // LOGGER.log("DEVICES ","Pressure check value is abnormal: ", "will initiate reset sequence...");
    // LOGGER.reset("Pressure check value is abnormal");
  }

  if(HAVE_DPS && !have_dps) {
      LOGGER.log("DEVICES ","DPS should be present and operational: ", "will initiate reset sequence of the DPS...");
      dps.reset();
      LOGGER.log("DEVICES ","DPS reset completed");
      LOGGER.log("DEVICES ","DPS begin I2C...");
      if (! dps.begin_I2C()) {    
        if(!HW_RESTART_ON_FAILURE) {
          LOGGER.log("DEVICES ", "*ERROR*","Failed to find DPS: will not initiate reset sequence of the DPS.");
        }
        else {
          LOGGER.log("DEVICES ", "*ERROR*","Failed to find DPS: will initiate reset sequence of the DPS...");
          LOGGER.reset("Failed to find DPS");
      }
      }
      else {
        have_dps = true;
        LOGGER.log("DEVICES ", "DPS found and ok");
      }

      if( have_dps ) {
        dps_temp = dps.getTemperatureSensor();
        dps_pressure = dps.getPressureSensor();

        dps.configurePressure(DPS310_64HZ, DPS310_64SAMPLES);
        dps.configureTemperature(DPS310_64HZ, DPS310_64SAMPLES);

        // dps_temp->printSensorDetails();
        // dps_pressure->printSensorDetails();
      }
  }

  LOGGER.log("DEVICES ","DPS checked");
}

void Devices::checkDUST() {
  // LOGGER.log("DEVICES ","Checking DUST...");
  // LOGGER.log("DEVICES ","DUST checked");
}

void Devices::checkAQ() {
  // LOGGER.log("DEVICES ","Checking AQ...");
  // LOGGER.log("DEVICES ","AQ checked");
}

void Devices::checkALCOHOL() {
  // LOGGER.log("DEVICES ","Checking Alcohol...");
  // LOGGER.log("DEVICES ","Alcohol checked");
}

void Devices::checkVOCCO2() {
  // LOGGER.log("DEVICES ","Checking VOCCO2...");
  // LOGGER.log("DEVICES ","VOCCO2 checked");
}

// void DEVICES ::check() {
  // LOGGER.log("DEVICES ","Checking...");
  // LOGGER.log("DEVICES ","checked");
// }

// void DEVICES ::check() {
  // LOGGER.log("DEVICES ","Checking...");
  // LOGGER.log("DEVICES ","checked");
// }

String Devices::publishDataString() {
  // char message[MQTT_MESSAGE_BUFFER] = {'\0'};

  if(have_dust) {
    dust_concentration = getMaxDust();
  }
  else
    dust_concentration = 0.0;

  if(have_alcohol) {
    air_alcohol = getMaxAlcohol();
  }
  else
    air_alcohol = 0;

  if(have_dps) {}
  
  if(have_aq) {
    air_quality = getMaxAir();
    if(HAVE_AQ>1) {
      air_quality2 = getMaxAir2();
    }
    else
      air_quality2 = -1;    
  }
  else
    air_quality = -1;

  if(have_vocco2) {
    air_voc = getMaxVOC();
    air_co2 = getMaxCO2();
  }
  else {
    air_voc = 0.0;
    air_co2 = 0.0;
  }

  if(have_lcd) {}
  
  if(have_temperature) {
    temperature = getMaxTemperature();
  }
  else
    temperature = 0;

  if(have_baro) {
    pressure = getMaxPressure();
  }
  else
    pressure = 0.0;

  if(have_humidity) {
    humidity = getMaxHumidity();
  }
  else
    humidity = 0;

  if(have_movement) {
    movement = getMovement();
  }
  else
    movement = 0;
      

  if(HAVE_AQ>0) {
    if (air_quality == AirQualitySensor::FORCE_SIGNAL) {
      air_quality_desc = "HAZARDOUS - ALERT";
    } else if (air_quality == AirQualitySensor::HIGH_POLLUTION) {
      air_quality_desc = "HIGH POLLUTION";
    } else if (air_quality == AirQualitySensor::LOW_POLLUTION) {
      air_quality_desc = "LOW POLLUTION";
    } else if (air_quality == AirQualitySensor::FRESH_AIR) {
      air_quality_desc = "FRESH - GOOD";
    }
  }
  else
    air_quality_desc = "NO SENSOR";

  if(HAVE_AQ>1) {
    if (air_quality2 == AirQualitySensor::FORCE_SIGNAL) {
      air_quality_desc2 = "HAZARDOUS - ALERT";
    } else if (air_quality2 == AirQualitySensor::HIGH_POLLUTION) {
      air_quality_desc2 = "HIGH POLLUTION";
    } else if (air_quality2 == AirQualitySensor::LOW_POLLUTION) {
      air_quality_desc2 = "LOW POLLUTION";
    } else if (air_quality2 == AirQualitySensor::FRESH_AIR) {
      air_quality_desc2 = "FRESH - GOOD";
    }
  }
  else
    air_quality_desc2 = "NO SENSOR";
    

  String time_stamp(LOGGER.timeClient->getEpochTime());
  sprintf(message, "{ \"data_time_stamp\":%s, \"device_id\":\"%s\", \"air_status\":\"%s\", \"air_quality\":%d, \"air_status2\":\"%s\", \"air_quality2\":%d, \"air_alcohol\":%f, \"dust_concentration\":%f, \"temperature\":%f, \"pressure\":%f, \"humidity\":%d, \"movement\":%d, \"VOC\":%d, \"eCO2\":%d }", time_stamp.c_str(), device_id.c_str(), air_quality_desc.c_str(), air_quality, air_quality_desc2.c_str(), air_quality2, air_alcohol, dust_concentration, temperature, pressure, humidity, movement, air_voc, air_co2);
  String messageStr(message);

  return(messageStr);
}

char* Devices::publishData() {
  // char message[MQTT_MESSAGE_BUFFER] = {'\0'};

  if(have_dust) {
    dust_concentration = getMaxDust();
  }
  else
    dust_concentration = 0.0;

  if(have_alcohol) {
    air_alcohol = getMaxAlcohol();
  }
  else
    air_alcohol = 0;

  if(have_dps) {}
  
  if(have_aq) {
    air_quality = getMaxAir();
    air_value = getMaxAirValue();
    if(HAVE_AQ>1) {
      air_quality2 = getMaxAir2();
      air_value2 = getMaxAirValue2();
    }
    else
      air_quality2 = -1;    
  }
  else
    air_quality = -1;

  if(have_vocco2) {
    air_voc = getMaxVOC();
    air_co2 = getMaxCO2();
  }
  else {
    air_voc = 0.0;
    air_co2 = 0.0;
  }

  if(have_lcd) {}
  
  if(have_temperature) {
    temperature = getMaxTemperature();
  }
  else
    temperature = 0;

  if(have_baro) {
    pressure = getMaxPressure();
  }
  else
    pressure = 0.0;

  if(have_humidity) {
    humidity = getMaxHumidity();
  }
  else
    humidity = 0;

  if(have_movement) {
    movement = getMovement();
  }
  else
    movement = 0;
      

  if(HAVE_AQ>0) {
    if (air_quality == AirQualitySensor::FORCE_SIGNAL) {
      air_quality_desc = "HAZARDOUS - ALERT";
    } else if (air_quality == AirQualitySensor::HIGH_POLLUTION) {
      air_quality_desc = "HIGH POLLUTION";
    } else if (air_quality == AirQualitySensor::LOW_POLLUTION) {
      air_quality_desc = "LOW POLLUTION";
    } else if (air_quality == AirQualitySensor::FRESH_AIR) {
      air_quality_desc = "FRESH - GOOD";
    }
  }
  else
    air_quality_desc = "NO SENSOR";

  if(HAVE_AQ>1) {
    if (air_quality2 == AirQualitySensor::FORCE_SIGNAL) {
      air_quality_desc2 = "HAZARDOUS - ALERT";
    } else if (air_quality2 == AirQualitySensor::HIGH_POLLUTION) {
      air_quality_desc2 = "HIGH POLLUTION";
    } else if (air_quality2 == AirQualitySensor::LOW_POLLUTION) {
      air_quality_desc2 = "LOW POLLUTION";
    } else if (air_quality2 == AirQualitySensor::FRESH_AIR) {
      air_quality_desc2 = "FRESH - GOOD";
    }
  }
  else
    air_quality_desc2 = "NO SENSOR";
    

  String time_stamp(LOGGER.timeClient->getEpochTime());
  memset(message,'\0',MQTT_MESSAGE_BUFFER);
  sprintf(message, "{ \"data_time_stamp\":%s, \"device_id\":\"%s\", \"air_status\":\"%s\", \"air_quality\":%d, \"air_value\":%d, \"air_status2\":\"%s\", \"air_quality2\":%d, \"air_value2\":%d, \"air_alcohol\":%f, \"dust_concentration\":%f, \"temperature\":%f, \"pressure\":%f, \"humidity\":%d, \"movement\":%d, \"VOC\":%d, \"eCO2\":%d }", time_stamp.c_str(), device_id.c_str(), air_quality_desc.c_str(), air_quality, air_value, air_quality_desc2.c_str(), air_quality2, air_value2, air_alcohol, dust_concentration, temperature, pressure, humidity, movement, air_voc, air_co2);

  return(message);
}

bool Devices::getData() {
  bool retour = true;

  // if ((millis() - starttime) > sampletime_ms) 
  if(lots2go >= 1 && samples2go <1)
  {
    // checkMessages();
    lots2go--;
    samples2go = NUMBER_SAMPLES;
    starttime = millis();
    maxairdatareached = false;

    if(have_humidity) {
      humidity = getMaxHumidity();
      LOGGER.log("Humidity:", humidity );
    }

    if(have_movement) {
      movement = getMovement();
      if(movement) {
        LOGGER.log("Movement:", "detection of movement");
        HW.ledWhite();
      }
      else {
        LOGGER.log("Movement:", "negative, no movement");
        HW.ledOff();
      }
    }

    if(have_dust) {
      dust_concentration = getMaxDust();
      LOGGER.log("Dust 0.01cf:", dust_concentration);
    }

    if(have_aq) {
      air_quality = getMaxAir();
      air_value = getMaxAirValue();
      // if (air_quality > 300) {
      //   air_quality_desc = "HAZARDOUS";
      // } else if (air_quality > 200) {
      //   air_quality_desc = "VERY UNHEALTHY";
      // } else if (air_quality > 150) {
      //   air_quality_desc = "UNHEALTHY";
      // } else if (air_quality > 100) {
      //   air_quality_desc = "UNHEALTHY FOR SENSITIVE GROUPS";
      // } else if (air_quality > 50) {
      //   air_quality_desc = "MODERATE";
      // } else {
      //   air_quality_desc = "FRESH / GOOD";
      // }
    
      if (air_quality == AirQualitySensor::FORCE_SIGNAL) {
        air_quality_desc = "HAZARDOUS / ALERT";
      } else if (air_quality == AirQualitySensor::HIGH_POLLUTION) {
        air_quality_desc = "HIGH POLLUTION";
      } else if (air_quality == AirQualitySensor::LOW_POLLUTION) {
        air_quality_desc = "LOW POLLUTION";
      } else if (air_quality == AirQualitySensor::FRESH_AIR) {
        air_quality_desc = "FRESH / GOOD";
      }

      LOGGER.log("Air status:", air_quality_desc);
      LOGGER.log("Air quality:", air_quality);

      if(HAVE_AQ>1) {
        air_quality2 = getMaxAir2();
        air_value2 = getMaxAirValue2();

        if (air_quality2 == AirQualitySensor::FORCE_SIGNAL) {
          air_quality_desc2 = "HAZARDOUS / ALERT";
        } else if (air_quality2 == AirQualitySensor::HIGH_POLLUTION) {
          air_quality_desc2 = "HIGH POLLUTION";
        } else if (air_quality2 == AirQualitySensor::LOW_POLLUTION) {
          air_quality_desc2 = "LOW POLLUTION";
        } else if (air_quality2 == AirQualitySensor::FRESH_AIR) {
          air_quality_desc2 = "FRESH / GOOD";
        }
      
        LOGGER.log("Air status:", air_quality_desc2);
        LOGGER.log("Air quality:", air_quality2);
      }
    }
    if(have_alcohol) {
      air_alcohol = getMaxAlcohol();
      LOGGER.log("Air alcohol:", air_alcohol, " ppm");
    }
    if(have_dps) {
      if(have_temperature) {
        temperature = getMaxTemperature();
        LOGGER.log("Temperature:", temperature, " *C");
      }
      if(have_baro) {
        pressure = getMaxPressure();
        LOGGER.log("Pressure:", pressure, " hPa");
      }
    }    
    if(have_vocco2) {
      air_voc = getMaxVOC();
      LOGGER.log("Air VOC:", air_voc, " ppb");

      air_co2 = getMaxCO2();
      LOGGER.log("Air eCO2:", air_co2, " ppm");
    }
    if(have_lcd) {

    }

    // lastairreadingcount += (airreadingcount-1);
    lastairreadingcount += (airreadingcount-1);

    // air_quality_desc.toCharArray(oled_text, 17);
    LOGGER.logEmptyLine();
  }
  else if(samples2go >= 1 && ((millis() - lasttime) > sample_ms)) {
    // checkMessages();
    samples2go--;

    lasttime = millis();
    // Serial.println("reading air data at 1 second cylcle");

    if(airreadingcount >= MAXAIR_DATA) {
      if(!maxairdatareached) {
        LOGGER.log("\n*WARNING*","max air reading reached, stoping read until next cylcle", "\n");
        // Serial.print(timeClient.getFormattedTime());
        // Serial.println("\tmax air reading reached, stoping read until next cylcle");
        // Serial.println();
        maxairdatareached = true;
        retour = false;
        HW.ledOff();
      }
    }    
    else {
      if(!maxairdatareached) {
        if(have_humidity) {
          HUMIDITY[airreadingcount] = analogRead(humidity_pin);
        }

        if(have_movement) {
          int value = digitalRead(movement_pin);
          MOVEMENT[airreadingcount] = value == HIGH ? true : false;
        }
        
        if(have_dust) {
          duration = pulseIn(dust_sensor_pin, LOW);
          lowpulseoccupancy = lowpulseoccupancy + duration;
          //dust sensor
          // ratio = lowpulseoccupancy / (sampletime_ms * 10.0); // Integer percentage 0=>100
          ratio = lowpulseoccupancy / (sample_ms * 10.0); // Integer percentage 0=>100
          dust_concentration = 1.1 * pow(ratio, 3) - 3.8 * pow(ratio, 2) + 520 * ratio + 0.62; // using spec sheet curve
          DUSTDATA[airreadingcount] = dust_concentration;
          lowpulseoccupancy = 0;
          
          if( dust_concentration < 1 || dust_concentration > 10000) {
            checkrequested = true;
          }
        }

        if(have_alcohol) {
          alcohol_data_struct air_alcohol;
          air_alcohol.sensorValue = analogRead(air_alcohol_pin);

          air_alcohol.sensor_volt=(float)air_alcohol.sensorValue/1024*5.0;
          // RS_air = sensor_volt/(5.0-sensor_volt);
          air_alcohol.RS_air = RS_air;
          air_alcohol.RS_gas = air_alcohol.sensor_volt/(5.0-air_alcohol.sensor_volt); // omit *R16
          air_alcohol.ratio = air_alcohol.RS_gas/air_alcohol.RS_air;  // ratio = RS/R0

          // LOGGER.log("ALCOHOL Sensor volt = ", air_alcohol.sensor_volt);
          // LOGGER.log("ALCOHOL RS_gas = ", air_alcohol.RS_gas);
          // LOGGER.log("ALCOHOL RS_air = ", air_alcohol.RS_air);
          // LOGGER.log("ALCOHOL ratio RS_gas/RS_air = ", air_alcohol.ratio);

          // //air alcohol sensor
          // #ifdef HAVE_ALCOHOL
          // air_alcohol.sensorValue = analogRead(air_alcohol_pin);
          // air_alcohol.sensor_volt=(float)air_alcohol.sensorValue/1024*5.0;
          // // air_alcohol.RS_air = air_alcohol.sensor_volt/(5.0-air_alcohol.sensor_volt); // omit *R16
          // air_alcohol.RS_air = getAvgAlcohol();
          // air_alcohol.RS_gas = air_alcohol.sensor_volt/(5.0-air_alcohol.sensor_volt); // omit *R16
          // air_alcohol.ratio = air_alcohol.RS_gas/air_alcohol.RS_air;  // ratio = RS/R0 ppm

          ALCOHOLDATA[airreadingcount].ratio = air_alcohol.ratio;
          ALCOHOLDATA[airreadingcount].RS_gas = air_alcohol.RS_gas;   //******MP getAvgAlcohol(); //...
          ALCOHOLDATA[airreadingcount].RS_air = air_alcohol.RS_air;
          ALCOHOLDATA[airreadingcount].sensor_volt = air_alcohol.sensor_volt;
          ALCOHOLDATA[airreadingcount].sensorValue = air_alcohol.sensorValue;
          // #endif
        }

        if(have_aq) {
          //air quality sensor
          last_air_quality = air_quality;
          air_quality = airSensor.slope();
          air_value = airSensor.getValue();
          AIRVALUE[airreadingcount] = air_value;
          // int quality2;
          // #ifdef HAVE_AQ
          // air_quality = analogRead(air_quality_pin);
          AIRDATA[airreadingcount] = air_quality;
          
          // #endif
          if( HAVE_AQ > 1) {
            last_air_quality2 = air_quality2;
            air_quality2 = airSensor2.slope();
            // #ifdef HAVE_AQ
            // air_quality = analogRead(air_quality_pin);
            AIRDATA2[airreadingcount] = air_quality2;

            air_value2 = airSensor2.getValue();
            AIRVALUE2[airreadingcount] = air_value2;
          }
          if( air_quality < 0 || air_quality > 1000) {
            checkrequested = true;
          }
          if( air_quality2 < 0 || air_quality2 > 1000) {
            checkrequested = true;
          }
        }

        if(have_vocco2) {
          vocec02_data_struct air_vocec02;
          s16 err = 0;
          u16 tvoc_ppb, co2_eq_ppm;
          err = sgp_measure_iaq_blocking_read(&tvoc_ppb, &co2_eq_ppm);
          if (err == STATUS_OK) {
              // LOGGER.log("Air VOC eCO2:", "tVOC  Concentration:", tvoc_ppb, "ppb");
              // LOGGER.log("Air VOC eCO2:", "CO2eq Concentration:", co2_eq_ppm, "ppm");

              air_vocec02.tvoc_ppb = tvoc_ppb;
              air_vocec02.co2_eq_ppm = co2_eq_ppm;

              air_vocec02.tvoc_ppb = air_vocec02.tvoc_ppb > 60000 ? 0 : air_vocec02.tvoc_ppb;
              air_vocec02.co2_eq_ppm = air_vocec02.co2_eq_ppm > 60000 ? 0 : air_vocec02.co2_eq_ppm;

              VOCCO2DATA[airreadingcount].tvoc_ppb = air_vocec02.tvoc_ppb;
              VOCCO2DATA[airreadingcount].co2_eq_ppm = air_vocec02.co2_eq_ppm;
          } else {
              // have_vocco2 = false;
              LOGGER.log("Air VOC eCO2:", "error reading IAQ values");
          }
        }

        if(have_lcd) {
        }

        if(have_dps) {
          if(have_temperature) {          
            if (dps.temperatureAvailable()) {
              dps_temp->getEvent(&temp_event);
              temperature = temp_event.temperature;
              temperature = temperature == 0.0 ? 0.010001 : temperature;
              lasttemperature = temperature;
              TEMPDATA[airreadingcount] = temperature;
            }
            else {
              TEMPDATA[airreadingcount] = lasttemperature;
              // if(airreadingcount > 0) {
              //   TEMPDATA[airreadingcount] = TEMPDATA[airreadingcount-1];              
              // }
            }
            if( temperature < -50 || temperature > 50) {
              checkrequested = true;
            }
          }
          if(have_baro) {
            // Reading pressure also reads temp so don't check pressure
            // before temp!
            if (dps.pressureAvailable()) {
              dps_pressure->getEvent(&pressure_event);
              pressure = pressure_event.pressure;
              pressure = pressure == 0.0 ? 0.010001 : pressure;
              lastpressure = pressure;
              PRESSUREDATA[airreadingcount] = pressure;
            }
            else {
              PRESSUREDATA[airreadingcount] = lastpressure;
            // if(airreadingcount > 0) {
            //   PRESSUREDATA[airreadingcount] = PRESSUREDATA[airreadingcount-1];              
            // }   
            }       
          }
          else {
            TEMPDATA[airreadingcount] = lasttemperature;
            PRESSUREDATA[airreadingcount] = lastpressure;
          }
          
          if( pressure < 0 || pressure > 20000) {
            checkrequested = true;
          }
        }     
      // }       
        // struct {
        //   float sensor_volt;
        //   float RS_air;            //  Get the value of RS via in a clear air
        //   float sensorValue;
        //   float RS_gas;            // Get value of RS in a GAS
        //   float ratio;             // Get ratio RS_GAS/RS_air
        // } alcohol_data_struct;

        // starttime = millis();
 
        airreadingcount += 1;
      }    
      else {
        retour = false;
        HW.ledOff();
      }     
    }
  }

  return(retour);
}

int Devices::getMaxAirValue() {
  int retour = 0;

  for(int i=lastairreadingcount; i<airreadingcount; i++) {
    if(AIRVALUE[i] > retour) {
      retour = AIRVALUE[i];
    }
  }

  return (retour);
}

int Devices::getMaxAirValue2() {
  int retour = 0;

  for(int i=lastairreadingcount; i<airreadingcount; i++) {
    if(AIRVALUE2[i] > retour) {
      retour = AIRVALUE2[i];
    }
  }

  return (retour);
}

int Devices::getMaxAir() {
  int retour = 0;

  for(int i=lastairreadingcount; i<airreadingcount; i++) {
    if(AIRDATA[i] > retour) {
      retour = AIRDATA[i];
    }
  }

  return (retour);
}

int Devices::getMaxAir2() {
  int retour = 0;

  for(int i=lastairreadingcount; i<airreadingcount; i++) {
    if(AIRDATA2[i] > retour) {
      retour = AIRDATA2[i];
    }
  }

  return (retour);
}

float Devices::getMaxAlcohol() {
  float retour = 0;

  for(int i=lastairreadingcount; i<airreadingcount; i++) {
    if(ALCOHOLDATA[i].ratio > retour) {
      retour = ALCOHOLDATA[i].ratio;
    // if(ALCOHOLDATA[i].RS_gas > retour) {
    //   retour = ALCOHOLDATA[i].RS_gas;
    }
  }

  return (retour);
}

float Devices::getAvgAlcohol() {
  float RS_air;
  float sensorValue = 1;
  float sensor_volt;
  int count;

  for(int i=lastairreadingcount; i<airreadingcount; i++) {
    // if(ALCOHOLDATA[i].RS_gas > retour) {
      sensorValue = sensorValue + ALCOHOLDATA[i].sensorValue;
    // }
  }

  count = airreadingcount > 0 ? airreadingcount : 1;
  sensorValue = sensorValue / (float)count;
  sensor_volt = sensorValue/1024*5.0;

  RS_air = sensor_volt/(5.0-sensor_volt); // omit *R16

  return (RS_air);
}

unsigned short Devices::getMaxVOC() {
  unsigned short retour = 0;

  for(int i=lastairreadingcount; i<airreadingcount; i++) {
    if(VOCCO2DATA[i].tvoc_ppb > retour) {
      retour = VOCCO2DATA[i].tvoc_ppb;
    }
  }

  return (retour);
}

unsigned short Devices::getMaxCO2() {
  unsigned short retour = 0;

  for(int i=lastairreadingcount; i<airreadingcount; i++) {
    if(VOCCO2DATA[i].co2_eq_ppm > retour) {
      retour = VOCCO2DATA[i].co2_eq_ppm;
    }
  }

  return (retour);
}

float Devices::getMaxDust() {
  float retour = 0;

  for(int i=lastairreadingcount; i<airreadingcount; i++) {
    if(DUSTDATA[i] > retour) {
      retour = DUSTDATA[i];
    }
  }

  return (retour);
}

float Devices::getMaxTemperature() {
  float retour = 0;

  for(int i=lastairreadingcount; i<airreadingcount; i++) {
    if(TEMPDATA[i] > retour) {
      retour = TEMPDATA[i];
    }
  }

  return (retour);
}

float Devices::getMaxPressure() {
  float retour = 0;

  for(int i=lastairreadingcount; i<airreadingcount; i++) {
    if(PRESSUREDATA[i] > retour) {
      retour = PRESSUREDATA[i];
    }
  }

  return (retour);
}

int Devices::getMaxHumidity() {
  int retour = 0;

  for(int i=lastairreadingcount; i<airreadingcount; i++) {
    if(HUMIDITY[i] > retour) {
      retour = HUMIDITY[i];
    }
  }

  return (retour);
}

bool Devices::getMovement() {
  bool retour = false;
  
  for(int i=lastairreadingcount; i<airreadingcount; i++) {
    if(MOVEMENT[i]) {
      retour = MOVEMENT[i];
    }
    if(retour)
      break;
  }

  return(retour);
}

bool Devices::readyPublishData() {
  bool retour = false;

  if( lots2go < 1) {
    retour = true;
  }

  return( retour );
}

Devices &Devices::getInstance() {
  static Devices instance;
  return instance;
}

Devices &DEVICES = DEVICES.getInstance();