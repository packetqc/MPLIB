---
layout: default
title: "MPLIB — Publications techniques"
description: "Publications techniques sur la bibliothèque de systèmes embarqués MPLIB : chiffrement matériel SAES/AES, échange de clés ECC/PQC, interface TouchGFX, multitâche RTOS sur MCU STM32. Par Martin Paquet & Claude."
permalink: /fr/publications/
---

# Publications techniques

Documentation et articles techniques du **projet MPLIB** — une bibliothèque de systèmes embarqués pour MCU STM32 (H5, H7, N6) avec chiffrement matériel, échange de clés ECC/PQC, interface TouchGFX et multitâche RTOS.

*Par Martin Paquet & Claude (Anthropic, Opus 4.6)*

**Languages / Langues**: [English]({{ '/publications/' | relative_url }}) | Français (cette page)

---

## À propos

MPLIB est une bibliothèque de systèmes embarqués de qualité production ciblant les microcontrôleurs STM32. Le projet couvre :

- **Chiffrement matériel** : SAES pour la persistance sur carte SD, AES pour les données en transit
- **Échange de clés** : ECC KEM avec annonce automatique et rafraîchissement de clés, migration PQC ML-KEM en cours
- **Interface** : Frontend TouchGFX avec séparation backend via files d'attente et mutex
- **RTOS** : FreeRTOS et Azure RTOS/ThreadX avec architecture de threads singleton
- **Réseau** : Pile réseau NXDuo avec UDP, NTP et services de gestion
- **Persistance** : Stockage chiffré sur carte SD avec réinitialisation usine

---

| | |
|---|---|
| **Base de connaissances** | [packetqc/knowledge](https://github.com/packetqc/knowledge) |
| **Dépôt** | [packetqc/MPLIB](https://github.com/packetqc/MPLIB) |
| **Contact** | packetqcca@gmail.com |
| **Plateforme** | STM32H573I-DK, STM32H743, STM32N6570-DK |
| **Pile technologique** | FreeRTOS / ThreadX RTOS, TouchGFX, SAES/AES, ECC/PQC |

---

*Toutes les publications sont versionnées. Les versions précédentes sont préservées lors des mises à jour.*
*© 2026 Martin Paquet & Claude (Anthropic)*
