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
    
    :libMPLIB_STM32_MCU.a

#### AZURE RTOS

- /MPLIB_APP_FRAMEWORK_AZRTOS/Core/Lib/libMPLIB_STM32_MCU.a

### INCLUDE

1. /MPLIB_STM32_MCU/Core/Inc

    ![alt text](image-1.png)

## USAGE

### AZRTOS

#### app_threadx.c

/* USER CODE BEGIN PV */
uint8_t *MPSystemThreadStack;
TX_THREAD MPSystemThreadHandle;
/* USER CODE END PV */


### FREERTOS

#### app_freertos.c

/* USER CODE BEGIN Includes */

#include "MPSystem.h"


/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN Variables */

/* Definitions for SystemServiceTask */

osThreadId_t SystemServiceTaskHandle;
const osThreadAttr_t SystemServiceTask_attributes = {
  .name = "SystemServiceTask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* USER CODE END Variables */


void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN RTOS_THREADS */

  /* add threads, ... */

  SystemServiceTaskHandle = osThreadNew(StartSystemServices, NULL, &SystemServiceTask_attributes);

  /* USER CODE END RTOS_THREADS */
}