# Changelog

## v1.0 — 2026-02-21

First official release of MPLIB documentation and demos.

### Features

- **Hardware SAES encryption** — SD card data-at-rest encryption using STM32 SAES peripheral
- **Hardware AES encryption** — Data-in-transit encryption for network communications
- **ECC KEM key exchange** — Elliptic Curve Cryptography with auto-announce, session initialization, and key refresh
- **ECIES data encryption** — Integrated Encryption Scheme over ECC for secure payloads
- **PQC ML-KEM** — Post-Quantum Cryptography migration with ML-KEM key encapsulation (lab prototype completed)
- **TouchGFX UI** — MVP-pattern frontend with daylight/night screen modes, log views, login screen
- **RTOS multitasking** — FreeRTOS and Azure RTOS/ThreadX support via conditional compilation (`FREERTOS`/`AZRTOS`)
- **Singleton thread architecture** — Default, GUI, Data, System, Display, Secure, SDCard, Network threads with visual heartbeat LEDs
- **SD card persistence** — Configuration save/load and factory reset
- **Network services** — NX Duo networking with UDP broadcast announce, ECC/PQC key exchange, and encrypted data transfer
- **UART logging** — Printf diagnostics to UART and TouchGFX log views

### Hardware Targets

- STM32H573I-DK (H5) — primary development board
- STM32H743I-EVAL (H7) — secondary target with FreeRTOS
- STM32N6570-DK (N6) — next-generation target (setup guide included)

### Documentation

- `README.md` — Project architecture, thread model, IDE configuration, RTOS comparison
- `key-exchange.md` — ECC KEM and PQC ML-KEM protocol sequences, UDP/TCP payload layouts
- `pqc-sizes.md` — ML-KEM and ML-DSA key size reference tables
- `STM32N6570-dk.md` — N6 Discovery Kit project setup and flashing guide
- `STM32N6570-dk-specs.md` — N6 MCU specifications

### Infrastructure

- GitHub Pages site at `packetqc.github.io/MPLIB` with Jekyll (minimal theme)
- English and French publication hubs
- Demo GIFs and videos in `Images/`
- CubeIDE symbol/include XML exports for H5 FreeRTOS and ThreadX configurations
