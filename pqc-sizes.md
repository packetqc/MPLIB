---
title: "MPLIB — PQC Key Sizes Reference"
description: "ML-KEM and ML-DSA post-quantum cryptography key size reference tables"
author: "Martin Paquet"
---

# SIZES

Post Quantum Crypto key sizes

## ML-KEM

|GRADE|KEYGEN_SEED|ENC_SEED|PRIVKEY|PUBKEY|CIPHERTEXT|SHARED_SECRET|
|--|--|--|--|--|--|--|
|512|64|32|1632|800|768|32|
|768|64|32|2400|1184|1088|32|
|1024|64|32|3168|1568|1568|32|

## ML-DSA

|GRADE|HEADER_MAX|KEYGEN_SEED|SIGN_SEED|PRIVKEY|PUBKEY|SIG|
|--|--|--|--|--|--|--|
|44|268|32|32|2560|1312|2420|
|65|268|32|32|4032|1952|3309|
|87|268|32|32|4896|2592|4627|

