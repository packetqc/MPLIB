---
title: "MPLIB — IDE Project Configuration"
description: "CubeIDE project configuration reference: MX code generation, symbols, includes, libraries, and source exclusions for STM32 H5/H7 with FreeRTOS and Azure RTOS"
author: "Martin Paquet"
---

# IDE Project Configuration

Reference for CubeIDE project setup across MPLIB and application framework projects.

## MX Code Generation (ThreadX Compilation)

Notes and details on CubeMX configuration to support BSP modules. Example: SD card — activate, no code generation, no init and no visibility (see in advanced project code generation).

*Detailed MX configuration steps to be documented.*

## MCU/MPU Settings

### H5

![H5 MCU/MPU Settings](../Images/image.png)

### H7

![H7 MCU/MPU Settings](../Images/image-2.png)

## Symbols

Common symbols for all targets:

- `TOUCHGFX`
- `STM32H573xx` (H5 target)
- `STM32H743xx` (H7 target)
- `USE_HAL_DRIVER`

### FreeRTOS

- `FREERTOS`

### Azure RTOS

- `AZRTOS`
- `TX_INCLUDE_USER_DEFINE_FILE`
- `TX_SINGLE_MODE_NON_SECURE`
- `NX_INCLUDE_USER_DEFINE_FILE`
- `NX_SECURE_INCLUDE_USER_DEFINE_FILE`

## Config Files

### FreeRTOS

- `FreeRTOSConfig.h`

### Azure RTOS

*Config files to be documented.*

### H5

- `stm32h573i_discovery_conf.h`
- `stm32h5xx_hal_conf.h`

### H7

- `stm32h743i_eval_conf.h` (in `Core/Inc`)
- `stm32h7xx_hal_conf.h`

## Conditional Includes

### Hardware-Defined

```c
#if defined(STM32H743xx)

#include "cmsis_os.h"
#include "stm32h743i_eval.h"
#include "stm32h7xx_hal_rng.h"
#include "stm32h7xx_hal_sd.h"
#include "stm32h7xx_hal.h"
#include "stm32h743i_eval_sdram.h"
#include "stm32h743i_eval_qspi.h"

#elif defined(STM32H573xx)

#include "stm32h5xx_hal.h"
#include "stm32h5xx_hal_eth.h"
#include "stm32h573i_discovery.h"
#include "stm32h5xx_hal_rng.h"
#include "stm32h5xx_hal_cryp.h"

#endif
```

### RTOS-Defined

```c
#if defined(FREERTOS)
// FreeRTOS headers included via FreeRTOSConfig.h

#elif defined(AZRTOS)

#include <malloc.h>
#include "tx_api.h"

#endif
```

### Time Slice Configuration

```c
#define TX_APP_THREAD_TIME_SLICE  1  // TX_NO_TIME_SLICE
```

## Include Paths

### Common (All Targets)

```
(workspace_path)/MPLIB_STM32_MCU/Core/Inc
Core/Inc
TouchGFX/App
TouchGFX/target/generated
TouchGFX/target
Drivers/CMSIS/Include
Middlewares/ST/touchgfx/framework/include
TouchGFX/generated/fonts/include
TouchGFX/generated/gui_generated/include
TouchGFX/generated/images/include
TouchGFX/generated/texts/include
TouchGFX/generated/videos/include
TouchGFX/gui/include
```

### H5

```
Drivers/STM32H5xx_HAL_Driver/Inc
Drivers/STM32H5xx_HAL_Driver/Inc/Legacy
Drivers/BSP/STM32H573I-DK
Drivers/CMSIS/Device/ST/STM32H5xx/Include
```

#### H5 — Azure RTOS (Eclipse ThreadX)

```
Middlewares/ST/threadx/ports/cortex_m33/gnu/inc
Middlewares/ST/threadx/common/inc
```

#### H5 — FreeRTOS

```
Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM33_NTZ/non_secure/
Middlewares/Third_Party/FreeRTOS/Source/include
Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
Middlewares/Third_Party/CMSIS/RTOS2/Include
```

### H7

```
Drivers/STM32H7xx_HAL_Driver/Inc
Drivers/STM32H7xx_HAL_Driver/Inc/Legacy
Drivers/BSP/STM32H743I-EVAL
Drivers/BSP/Components/Common
Drivers/BSP/Components/exc7200
Drivers/BSP/Components/is42s32800g
Drivers/BSP/Components/mt25tl01g
Drivers/BSP/Components/ts3510
Drivers/BSP/Components/stmpe811
Drivers/CMSIS/Device/ST/STM32H7xx/Include
Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
```

### MPLIB Project Reference

```
(as workspace)/MPLIB_STM32_MCU/Core/Inc
```

![Include path configuration](../Images/image-1.png)

## Library Paths

### FreeRTOS

```
/MPLIB_APP_FRAMEWORK_FREERTOS/Core/Lib/
:libMPLIB_STM32_MCU.a
```

### Azure RTOS

```
/MPLIB_APP_FRAMEWORK_AZRTOS/Core/Lib/
:libMPLIB_STM32_MCU.a
```

## Source Exclusions

### H5

Exclude from compilation:

```
Drivers/STM32H7xx_HAL_Driver
Drivers/CMSIS/Device/ST/STM32H7xx
...
Drivers/BSP/Components/mx25lm51245g
```

### H7

Exclude from compilation:

```
Drivers/BSP/STM32H573I-DK
Drivers/BSP/STM32H743I-EVAL/stm32h743i_eval_sd.c
Drivers/STM32H5xx_HAL_Driver
Drivers/CMSIS/Device/ST/STM32H5xx
Middlewares/ST/threadx/ports/cortex_m33
Middlewares/ST/touchgfx/framework/source
Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM33_NTZ
Drivers/BSP/Components/mx25lm51245g
```

### FreeRTOS

```
Middlewares/ST/threadx
```

### Azure RTOS

*Excludes to be documented.*

---

## IDE Project Usage

### Azure RTOS

#### app_azure_rtos_config.h

```c
/* Using static memory allocation via ThreadX byte memory pools */

#define USE_STATIC_ALLOCATION          1
#define TX_APP_MEM_POOL_SIZE           1024*10
#define TOUCHGFX_APP_MEM_POOL_SIZE     8192

#define TX_MEM_DEFAULT_THREAD   256
#define TX_MEM_DATA_THREAD      1024
#define TX_MEM_SYSTEM_THREAD    256
#define TX_MEM_DISPLAY_THREAD   256
#define TX_MEM_SECURE_THREAD    256
#define TX_MEM_SDCARD_THREAD    512
```

#### app_threadx.c

```c
TX_THREAD defaultTaskHandle;
TX_THREAD GUI_TaskHandle;
TX_THREAD DataServicesHandle;
TX_THREAD SystemServiceTaHandle;
TX_THREAD DisplayServiceHandle;
TX_THREAD SecureServiceHandle;
TX_THREAD SDServiceHandle;

extern void StartDataServices(ULONG thread_input);
extern void StartSystemServices(ULONG thread_input);
extern void StartDisplayServices(ULONG thread_input);
extern void StartSecureServices(ULONG thread_input);
extern void StartSDServices(ULONG thread_input);

// App_ThreadX_Init
TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;
CHAR *pointer;

if (tx_byte_allocate(byte_pool, (VOID**) &pointer,
    TX_APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
{
    return TX_POOL_ERROR;
}

/* Create default start thread */
if (tx_thread_create(&defaultTaskHandle, "Default start thread",
    StartDefaultTask, 0, pointer,
    TX_APP_STACK_SIZE, TX_MPLIB_THREAD_PRIO,
    TX_MPLIB_THREAD_PREEMPTION_THRESHOLD,
    TX_NO_TIME_SLICE, TX_APP_THREAD_AUTO_START) != TX_SUCCESS)
{
    return TX_THREAD_ERROR;
}
```

#### Thread Resume Ordering (Default Task)

```c
void StartDefaultTask(ULONG thread_input)
{
    tx_thread_resume(DataServicesHandle);
    tx_thread_resume(SecureServiceHandle);
    tx_thread_sleep(30);
    tx_thread_resume(DisplayServiceHandle);
    tx_thread_sleep(30);
    tx_thread_resume(SDServiceHandle);
    tx_thread_resume(SystemServiceTaHandle);
    tx_thread_resume(GUI_TaskHandle);

    while(1) {
        tx_thread_sleep(30);
    }
}
```

### FreeRTOS

#### app_freertos.c

```c
#include "MPSystem.h"

osThreadId_t SystemServiceTaskHandle;
const osThreadAttr_t SystemServiceTask_attributes = {
    .name = "SystemServiceTask",
    .stack_size = 1024 * 4,
    .priority = (osPriority_t) osPriorityNormal,
};

void MX_FREERTOS_Init(void) {
    SystemServiceTaskHandle = osThreadNew(
        StartSystemServices, NULL,
        &SystemServiceTask_attributes);
}
```

---

## IDE Project Files

### Core Files

*Project core file reference to be documented.*

### MPLIB Files

Linked via project library configurations — see [Library Paths](#library-paths) above.

### TouchGFX Files

Project file: `TouchGFX/MPLIB_APP_FRAMEWORK_FREERTOS.touchgfx`

Includes all project files integrated with the MPLIB library:

- `ModelListener.hpp`
- `Model.cpp` / `Model.hpp`
- Base presenter and view files for containers and screens (TouchGFX generated)
- Custom presenter and view files for containers and screens
- Texts and images

### Drivers

- BSP
- `Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F`

### Middlewares

*Middleware reference to be documented.*
