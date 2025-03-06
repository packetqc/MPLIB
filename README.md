# MPLIB
 MP Library stm32 and arduino

# STM32 LIB

## CONFIGS

### SYMBOLS

- FREERTOS
- AZRTOS

### LIBS

#### FREERTOS

- /MPLIB_APP_FRAMEWORK_FREERTOS/Core/Lib/libMPLIB_STM32_MCU.a
    
    MPLIB_STM32_MCU

#### AZURE RTOS

- /MPLIB_APP_FRAMEWORK_AZRTOS/Core/Lib/libMPLIB_STM32_MCU.a

### INCLUDE

1. /MPLIB_STM32_MCU/Core/Inc

    ![alt text](image-1.png)

## USAGE

### FREERTOS

#### app_freertos.c
#include "MPSystem.h"

/* Definitions for SystemServiceTask */
osThreadId_t SystemServiceTaskHandle;
const osThreadAttr_t SystemServiceTask_attributes = {
  .name = "SystemServiceTask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

void MX_FREERTOS_Init(void) {
    // ...
    SystemServiceTaskHandle = osThreadNew(StartSystemServices, NULL, &SystemServiceTask_attributes);
    // ...
}