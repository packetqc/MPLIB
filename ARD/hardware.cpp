#include "hardware.h"
#include "debug.h"

#include <WiFiNINA.h>
#include <utility/wifi_drv.h>


// extern int __heap_start,*__brkval;
#ifdef __arm__
extern "C" char* sbrk(int incr);
#else
extern char *__brkval;
#endif


// Hardware::Hardware(Debug* LOGGER,HardwareSerial* serial) {
//   this->LOGGER = LOGGER;
//   this->Serial = serial;

// }

// Hardware::~Hardware() {  
// }

uint8_t Hardware::begin() {
  // this->device_id = device_id;
  started = true;

  LOGGER.log("HARDWARE", "Begins...");

  // starttime = millis();              //get the current time;
  // lastupdatetime = starttime;
  // lastupdatenetworktime = starttime;
  lastcheck = millis();

  LOGGER.log("HARDWARE", "Has begun", "\n");
  
  return( started );
}

uint8_t Hardware::end() {
  started = false;

  LOGGER.log("HARDWARE", "Has ended");

  return( started );
}

void Hardware::check(bool now) {
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
    LOGGER.log("HARDWARE","Checks...");

    checkSerial();
    checkMemory();

    LOGGER.log("HARDWARE","Checked", "\n");
    // if( ! checkrequested ) {
    //   LOGGER.logEmptyLine();
    // }
  }
  checkrequested = false;
}

void Hardware::checkSerial() {
  // LOGGER.log("HARDWARE","Checking Serial...");
  // if (!(*Serial)) {
  //   LOGGER.log("HARDWARE","No Serial present! Restarting Serial...");
  //   Serial->end();
  //   Serial->begin(115200);
  // }

  // if (!(*Serial)) {
  //   LOGGER.log("HARDWARE","No Serial still present! Ending program!");
  //   while (1);
  // }
  // LOGGER.log("HARDWARE","Serial checked");
}

void Hardware::checkMemory() {
  // LOGGER.log("HARDWARE","Checking Memory...");
  checkMemoryFree();
  // LOGGER.log("HARDWARE","Memory checked");
}

void Hardware::checkMemoryFree() {
  // LOGGER.log("HARDWARE","Checking Memory...");
  char top;

#ifdef __arm__
  freememory = &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  freememory = &top - __brkval;
#else
  freememory = __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif

  // if( ! checkrequested ) {
    LOGGER.log("HARDWARE","Memory free: ", freememory);

    if(freememory < 100) {
      LOGGER.log("HARDWARE","Memory free is too low: ", "will initiate reset sequence...");
      LOGGER.reset("Memory free is too low");
    }
  // }
  // LOGGER.log("HARDWARE","Memory checked");
}

void Hardware::ledFlash(uint8_t color, uint8_t manytimes) {
  uint8_t c1;
  uint8_t c2;
  uint8_t c3;
  
  switch(color) {
    case BLUE:
    c1 = 0; c2 = 0; c3 = 255;
    break;
    case RED:
    c1 = 255; c2 = 0; c3 = 0;
    break;
    case WHITE:
    c1 = 255; c2 = 255; c3 = 255;
    break;
    case GREEN:
    c1 = 0; c2 = 255; c3 = 0;
    break;
    case YELLOW:
    c1 = 255; c2 = 255; c3 = 0;
    break;
    case PURPLE:
    c1 = 0; c2 = 255; c3 = 255;
    break;
    case CYAN:
    c1 = 255; c2 = 0; c3 = 255;
    break;
    case OFF:
    c1 = 0; c2 = 0; c3 = 0;
    default:
    c1 = 0; c2 = 0; c3 = 0;
    break;
  }

  for(uint8_t i=0; i<manytimes; i++) {
    ledOff();
    delay(200);
    WiFiDrv::analogWrite(25, c1);
    WiFiDrv::analogWrite(26, c2);
    WiFiDrv::analogWrite(27, c3);
    delay(200);
  }
  ledOff();
}

void Hardware::ledBlue() {
  WiFiDrv::analogWrite(25, 0);
  WiFiDrv::analogWrite(26, 0);
  WiFiDrv::analogWrite(27, 255);
}

void Hardware::ledRed() {
  WiFiDrv::analogWrite(25, 255);
  WiFiDrv::analogWrite(26, 0);
  WiFiDrv::analogWrite(27, 0);
}

void Hardware::ledGreen() {
  WiFiDrv::analogWrite(25, 0);
  WiFiDrv::analogWrite(26, 255);
  WiFiDrv::analogWrite(27, 0);
}

void Hardware::ledWhite() {
  WiFiDrv::analogWrite(25, 255);
  WiFiDrv::analogWrite(26, 255);
  WiFiDrv::analogWrite(27, 255);
}

void Hardware::ledOff() {
  WiFiDrv::analogWrite(25, 0);
  WiFiDrv::analogWrite(26, 0);
  WiFiDrv::analogWrite(27, 0);
}

void Hardware::ledYellow() {
  WiFiDrv::analogWrite(25, 255);
  WiFiDrv::analogWrite(26, 255);
  WiFiDrv::analogWrite(27, 0);  
}

void Hardware::ledPurple() {
  WiFiDrv::analogWrite(25, 0);
  WiFiDrv::analogWrite(26, 255);
  WiFiDrv::analogWrite(27, 255);
}

void Hardware::ledCyan() {
  WiFiDrv::analogWrite(25, 255);
  WiFiDrv::analogWrite(26, 0);
  WiFiDrv::analogWrite(27, 255);
}



// void Hardware::check() {
  // LOGGER.log("HARDWARE","Checking...");
  // LOGGER.log("HARDWARE","checked");
// }

// void Hardware::check() {
  // LOGGER.log("HARDWARE","Checking...");
  // LOGGER.log("HARDWARE","checked");
// }


// bool Hardware::readyPublishData() {
//   bool retour = false;

//   // if( lots2go < 1) {
//   //   retour = true;
//   // }

//   return( retour );
// }

// String Hardware::publishData() {
//   char message[255];

//   String time_stamp(LOGGER.timeClient->getEpochTime());
//   sprintf(message, "{ \"time_stamp\": \"%s\", \"device_id\": \"%s\", \"air_status\": \"%s\", \"air_quality\": %d, \"dust_concentration\": %f, \"temperature\": %f, \"pressure\": %f }", time_stamp.c_str(), device_id.c_str(), air_quality_desc.c_str(), air_quality, dust_concentration, temperature, pressure);
  
//   // LOGGER.log(message,"\n");

//   String messageStr(message);

//   return(messageStr);
// }


Hardware &Hardware::getInstance() {
  static Hardware instance;
  return instance;
}

Hardware &HW = HW.getInstance();