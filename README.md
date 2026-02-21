---
title: "MPLIB — Embedded Systems Library for STM32"
description: "Hardware SAES/AES encryption, ECC/PQC key exchange, TouchGFX UI, RTOS multitasking, SD card persistence, and network services for STM32 MCUs"
author: "Martin Paquet"
---

# MPLIB

**v1.0** — Embedded Systems Library for STM32 MCUs (H5, H7, N6)

Hardware encryption, ECC/PQC key exchange, TouchGFX UI, RTOS multitasking, SD card persistence, and network services.

| | |
|---|---|
| **Source code** | Private repo ([MPLIB-CODE](https://github.com/packetqc/MPLIB-CODE)) |
| **Documentation** | This repo + [GitHub Pages](https://packetqc.github.io/MPLIB/) |
| **Portfolio** | [YouTube](https://youtube.com/@packet-yi9sq?feature=shared) |

---

## Features

- **Hardware SAES encryption** — SD card data-at-rest encryption using STM32 SAES peripheral
- **Hardware AES encryption** — Data-in-transit encryption for network communications
- **ECC KEM key exchange** — Auto-announce, session initialization, key refresh — [protocol details](key-exchange.md)
- **PQC ML-KEM** — Post-quantum cryptography migration — [key sizes](pqc-sizes.md) · [LinkedIn update](https://www.linkedin.com/posts/martypacket_primer-from-quebec-city-more-details-to-activity-7373330968012562433-jv8H?utm_source=share&utm_medium=member_desktop&rcm=ACoAAD1vQloBCFsX_wTEDglktk5vKN4V8UQPqGk)
- **TouchGFX UI** — MVP-pattern frontend with daylight/night modes, log views, login screen
- **RTOS multitasking** — FreeRTOS and Azure RTOS/ThreadX via conditional compilation
- **SD card persistence** — Configuration save/load and factory reset
- **Network services** — NX Duo with UDP, NTP, management monitoring
- **UART logging** — Printf diagnostics to UART and TouchGFX log views

---

## Demo

![MPLIB demo](./Images/mplibsreadyforcodemigration-1.gif)

<video autoplay mute controls src="./Images/MPLIB-demo-all-devices-screen-and-camera-hr-1-1.mp4"></video>

![MPLIB all devices demo](./Images/MPLIB-demo-all-devices-screen-and-camera-hr-2x-1-1.gif)

### Hardware Encryption

#### SD Card and Screen Encryption

| Released | Previous dev |
|----------|--------------|
| ![SAES encryption demo](./Images/MPLIB-SAES-hw-encryption-demo-3-1-1.gif) | ![On-screen encryption demo](./Images/MPLIB-encrypt-on-screen-daylight-3.gif) |

#### Data-in-Transit Encryption

| ECC / ECIES | ECC Key Generation Auto Refresh |
|-------------|--------------------------------|
| ![ECC crypto network demo](./Images/STM32H573i-Crypto-Network-Udp-Ntp-Mgmt-2.gif) | ![ECC crypto demo](./Images/ecc-crypto-demo.gif) |

### Logs

![MPLIB H7 logs preview](./Images/MPLIB-H7-preview-1-1.gif)

### Login

![MPLIB login demo](./Images/MPLIB-demo-login-1-1.gif)

### Config Save on SD Card

![Config save demo](./Images/mplibs-config-sdcard.gif)

### Factory Reset

![Factory reset demo](./Images/MPLIB-demo-tgfx-1-1.gif)

### Screen Modes (Daylight / Night)

![Screen modes demo](./Images/mplibs-freertos-screen-lite-2x-1.gif)

---

## Hardware Targets

| MCU | Board | Role |
|-----|-------|------|
| STM32H573 (H5) | STM32H573I-DK | Primary development board |
| STM32H743 (H7) | STM32H743I-EVAL | Secondary target (FreeRTOS) |
| STM32N657 (N6) | STM32N6570-DK | Next-generation target |

- N6 setup guide: [STM32N6570-dk.md](STM32N6570-dk.md)
- N6 hardware specs: [STM32N6570-dk-specs.md](STM32N6570-dk-specs.md)

---

## Architecture

### Project Structure

| Project | Description |
|---------|-------------|
| `MPLIB_STM32_MCU` | Core library — compiles to `libMPLIB_STM32_MCU.a` |
| `MPLIB_APP_FRAMEWORK_FREERTOS` | Application framework (FreeRTOS, H5) |
| `MPLIB_APP_FRAMEWORK_AZRTOS` | Application framework (Azure RTOS, H5) |
| `MPLIB_APP_FRAMEWORK_H7_FREERTOS` | Application framework (FreeRTOS, H7) |

```mermaid
flowchart LR
  MPLIB_STM32_MCU ==> lib1([libMPLIB_STM32_MCU.a])
  lib1([libMPLIB_STM32_MCU.a]) -.- MPLIB_APP_FRAMEWORK_FREERTOS
  lib1([libMPLIB_STM32_MCU.a]) -.- MPLIB_APP_FRAMEWORK_AZRTOS
  lib1([libMPLIB_STM32_MCU.a]) -.- MPLIB_APP_FRAMEWORK_H7_FREERTOS
```

### Branch Strategy (Source Code)

```mermaid
---
title: MPLIB_STM32_MCU
---
gitGraph
commit id: "Init main FreeRTOS"
branch H5
branch H7
commit id: "Init H7"
checkout H5
commit id: "Init H5"
branch FREERTOS
branch AZRTOS
commit id: "Init AZURE RTOS"
commit id: "MX and BSP ok"
checkout FREERTOS
commit id: "Init FREERTOS"
checkout H5
merge AZRTOS
commit id: "Ready to merge in FreeRTOS"
checkout AZRTOS
commit id: "Init ongoing dev"
checkout H5
branch FREERTOS-MX-MERGING
commit id: "Merging MX and BSP"
checkout AZRTOS
commit id: "Adapt from FreeRTOS MX merge"
checkout H5
merge AZRTOS
checkout main
merge H5
checkout AZRTOS
commit id: "Network dev udp sender and receiver"
checkout H5
merge AZRTOS
checkout main
merge H5
checkout AZRTOS
commit id: "Cryptography dev"
commit id: "NTP, MGMT monitor Tx and memory pool, ECC session & crypto token"
checkout H5
merge AZRTOS
commit id: "Post Quantum Crypto dev"
branch WOLFSSL
commit id: "Post Quantum Crypto WolfCrypt"
checkout H5
branch CMOX
commit id: "STM32 Post Quantum Crypto"
```

### Threads (Singletons)

| Thread | Visual Heartbeat | Description |
|--------|-----------------|-------------|
| Default | Green LED | Entry point, resumes other threads |
| GUI | Green screen border | TouchGFX task |
| Data | Orange LED | Network data services |
| System | Red LED | System monitoring |
| Display | Blue LED | Display services |
| Secure | Red LED | Cryptographic services |
| SDCard | Blue LED | SD card persistence |
| Network | Green LED | NX Duo networking |

Resume ordering: Data → Secure → Display → SDCard → System → GUI

### Communications

#### Queues and Mutexes

| Asset | Queue | Mutex |
|-------|-------|-------|
| canLog | | x |
| gui_msg | x | |
| logs_msg | x | |
| gui_logs_msg | x | |
| logsmon_msg | x | |
| ConnectionEvent | x | |
| sd_msg | x | |

#### UI / Backend Flow

```mermaid
sequenceDiagram
C ->> Cplusplus: start thread
Cplusplus ->> Model: queues
Model ->> Presenter: modelListener
Presenter ->> View: bind
C ->> Cplusplus: queues
```

#### Application State, Interactions and Persistence

```mermaid
sequenceDiagram
SDServices ->> SDCard: read
SDServices ->> SystemServices: set config
Model ->> SystemServices: get config
Model ->> Presenter: get
Presenter ->> Model: set
Presenter ->> Model: call functions
Model ->> Presenter: call functions
Model ->> SystemServices: set config
View ->> Presenter: call functions
Presenter ->> View: call functions
Screen TouchGFX ->> View: interactions
View ->> Presenter: event / interactions
View ->> SecureServices: encrypt
View ->> SecureServices: decrypt
View ->> Screen TouchGFX: get
View ->> SystemServices: set config
View ->> SDServices: set sd config
View ->> SDServices: reset factory
SDServices ->> SDServices: sd msg queue (encrypt screen)
SDServices ->> SDServices: sd msg queue (encrypt sdcard)
SDServices ->> SDServices: sd msg queue (reset factory)
SDServices ->> SystemServices: get config
SDServices ->> SecureServices: encrypt
SDServices ->> SecureServices: decrypt
SDServices ->> SDCard: write
SDServices ->> Model: gui msg queue (update config)
Model ->> Presenter: update config
Presenter ->> View: update config
View ->> Screen TouchGFX: set & invalidate
```

### Application Framework

*Application framework documentation to be added.*

---

## Documentation

| Document | Description |
|----------|-------------|
| [Key Exchange Protocol](key-exchange.md) | ECC KEM and PQC ML-KEM sequences, UDP/TCP payload layouts |
| [PQC Key Sizes](pqc-sizes.md) | ML-KEM and ML-DSA key size reference tables |
| [STM32N6570-DK Setup](STM32N6570-dk.md) | N6 Discovery Kit project setup and flashing guide |
| [N6 Hardware Specs](STM32N6570-dk-specs.md) | STM32N657X0H3Q MCU specifications |
| [IDE Configuration](docs/ide-config.md) | CubeIDE symbols, includes, libraries, and source exclusions |
| [RTOS Comparison](docs/rtos-comparison.md) | FreeRTOS / ThreadX API equivalence table |
| [Hardware Reference](docs/hardware-reference.md) | H5 security architecture, UID access, USB BSP, UART tips |
| [Changelog](CHANGELOG.md) | Release history |

---

## Links

- [GitHub Pages](https://packetqc.github.io/MPLIB/) — Publications and documentation site
- [Knowledge Base](https://packetqc.github.io/knowledge/) — Cross-project knowledge
- [YouTube Portfolio](https://youtube.com/@packet-yi9sq?feature=shared) — Video demos
