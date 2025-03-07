# MPLIB
 MP Library stm32 and arduino

# STM32 LIB

![demo](mplibsreadyforcodemigration.gif)

## MPLIB STRUCTURE AND WORKFLOW

### STRUCTURE

### WORKFLOW


```mermaid
flowchart LR
  Start --> Stop
```

```mermaid
flowchart LR
  Start --> Stop
```

## IDE PROJECT CONFIGS

<details>
<summary>Click to view details</summary>

### SYMBOLS

- FREERTOS
- AZRTOS

### LIBS

#### FREERTOS

/MPLIB_APP_FRAMEWORK_FREERTOS/Core/Lib/  
:libMPLIB_STM32_MCU.a

#### AZURE RTOS

/MPLIB_APP_FRAMEWORK_AZRTOS/Core/Lib/  
:libMPLIB_STM32_MCU.a

### INCLUDE

1. /MPLIB_STM32_MCU/Core/Inc

    ![alt text](image-1.png)

</details>

## IDE PROJECT USAGE

<details>
<summary>Click to view details</summary>

### AZRTOS

#### app_threadx.c

/* USER CODE BEGIN PV */

TX_THREAD MPSystemThreadHandler;

/* USER CODE END PV */

//App_ThreadX_Init

/* USER CODE BEGIN App_ThreadX_Init */

  /* Allocate the stack for MPSystem thread  */

  if (tx_byte_allocate(byte_pool, (VOID**) &pointer,
                         TX_APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
    {
      return TX_POOL_ERROR;
    }

  /* Create MPSystem thread.  */

  if (tx_thread_create(&MPSystemThreadHandler, "MPSystem", StartSystemServices, 0, pointer,
	TX_APP_STACK_SIZE, TX_MPLIB_THREAD_PRIO, TX_MPLIB_THREAD_PREEMPTION_THRESHOLD,
	TX_NO_TIME_SLICE, TX_APP_THREAD_AUTO_START) != TX_SUCCESS)
  {
	  return TX_THREAD_ERROR;
  }

  /* USER CODE END App_ThreadX_Init */


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

</details>
