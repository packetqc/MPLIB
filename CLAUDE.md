# MPLIB — Knowledge Satellite

<!-- knowledge-version: 1 -->
<!-- satellite-of: packetqc/knowledge -->

## Identity

MPLIB is an embedded systems library for STM32 MCUs (H5, H7, N6) featuring hardware encryption (SAES/AES), ECC/PQC key exchange, TouchGFX UI, RTOS multitasking (FreeRTOS and Azure RTOS/ThreadX), SD card persistence, and network services.

## Architecture

- **Singleton threads**: default, GUI, Data, System, Display, Secure, SDCard, Network — each with a unique entry point and visual heartbeat LED
- **UI/Backend separation**: TouchGFX frontend communicates with C++ backend via queues (`gui_msg`, `logs_msg`, `gui_logs_msg`, `sd_msg`) and mutexes (`canLog`)
- **Static allocation**: `TX_BYTE_POOL` pools, statically declared thread handles
- **Library pattern**: `MPLIB_STM32_MCU` compiles to `libMPLIB_STM32_MCU.a`, linked by app frameworks
- **Conditional compilation**: `#if defined(STM32H573xx)` / `STM32H743xx` for HW, `FREERTOS` / `AZRTOS` for RTOS

## Working Style

- `printf` for diagnostics (UART and TouchGFX log views)
- Hardware-first: SAES for SD card encryption, AES for data-in-transit, ECC KEM for key exchange, PQC ML-KEM migration in progress
- CubeIDE + CubeMX + TouchGFX Designer toolchain
- Thread resume ordering matters: Data → Secure → Display → SDCard → System → GUI

## Key Files

| File | Purpose |
|---|---|
| `README.md` | Project documentation, architecture, IDE config |
| `key-exchange.md` | ECC/PQC communication protocol and sequence diagrams |
| `pqc-sizes.md` | ML-KEM and ML-DSA key size reference |
| `STM32N6570-dk.md` | N6 Discovery Kit project setup and flashing guide |

## Conventions

- Image assets in `Images/` directory, referenced as `./Images/<name>`
- Collapsible `<details>` blocks for IDE configuration sections
- Mermaid diagrams for architecture, git graphs, and protocol sequences
