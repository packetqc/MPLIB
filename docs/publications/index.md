---
layout: default
title: "MPLIB — Technical Publications"
description: "Technical publications on the MPLIB embedded systems library: hardware SAES/AES encryption, ECC/PQC key exchange, TouchGFX UI, RTOS multitasking on STM32 MCUs. By Martin Paquet & Claude."
permalink: /publications/
---

# Technical Publications

Documentation and technical papers from the **MPLIB Project** — an embedded systems library for STM32 MCUs (H5, H7, N6) featuring hardware encryption, ECC/PQC key exchange, TouchGFX UI, and RTOS multitasking.

*By Martin Paquet & Claude (Anthropic, Opus 4.6)*

**Languages / Langues**: English (this page) | [Français]({{ '/fr/publications/' | relative_url }})

---

## About

MPLIB is a production-grade embedded systems library targeting STM32 microcontrollers. The project covers:

- **Hardware encryption**: SAES for SD card persistence, AES for data-in-transit
- **Key exchange**: ECC KEM with auto-announce and key refresh, PQC ML-KEM migration in progress
- **UI**: TouchGFX frontend with backend separation via queues and mutexes
- **RTOS**: FreeRTOS and Azure RTOS/ThreadX with singleton thread architecture
- **Networking**: NXDuo networking stack with UDP, NTP, and management services
- **Persistence**: Encrypted SD card storage with factory reset capability

---

| | |
|---|---|
| **Knowledge Base** | [packetqc/knowledge](https://github.com/packetqc/knowledge) |
| **Repository** | [packetqc/MPLIB](https://github.com/packetqc/MPLIB) |
| **Contact** | packetqcca@gmail.com |
| **Platform** | STM32H573I-DK, STM32H743, STM32N6570-DK |
| **Stack** | FreeRTOS / ThreadX RTOS, TouchGFX, SAES/AES, ECC/PQC |

---

*All publications are versioned. Previous versions are preserved when updates are made.*
*© 2026 Martin Paquet & Claude (Anthropic)*
