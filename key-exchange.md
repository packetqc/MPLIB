---
title: "MPLIB — Key Exchange Protocol"
description: "ECC KEM and PQC ML-KEM communication protocol, sequence diagrams, and UDP payload layouts for STM32 encrypted networking"
author: "Martin Paquet"
---

# COMMUNICATIONS

## Communication table documentation
    
|Communication|Type|Protocol|Port src/local|Port dst/remote|Description|
|--|--|--|--|--|--|
|Announce ECC|Client|UDP|13131|31313|Send announcement broadcast UDP datagram including ECC public key for session initialization with other nodes on the network lan|
|Proceed ECC|Server|UDP|31313|na|Receive announcement, coordinate ECC to initialize and engage PQC unicast announce to the node on the network UDP broadcasting|
|ECC sender initializer|Client|UDP|13133|31333|ECC Session Initialization, ECC_Session_Send|
|ECC receiver|Server,Client|UDP|13333|33333|ECC Session Establishment, ECC_Session_Establish|
|ECC establisher|Server|UDP|33333|na|ECC Session Establishment|
|Announce PQC|Client|TCP|13131|31313|Upon UDP broadcast received by Proceed service, send announcement TCP unicast including PQC public key and cypher for session initialization with other nodes on the network lan|
|Proceed PQC|Server|TCP|31313|na|Receive TCP unicast announcement and coordinate PQC to initialize|
|Data sender|Client|UDP|34331|34333|Send tokens data example: clear-text, cypher-ecc-aes and cypher-pqc-aes|
|Data receiver|Server|UDP|34333|na|Receive tokens data example: clear-text, cypher-ecc-aes and cypher-pqc-aes|

## Communication between nodes

### ECC KEM

The lab prototype have been designed initially to support ECC KEM. Next diagram shows the key exchange not transported and created on both sides.

```mermaid  
  sequenceDiagram  
  participant N1 as Node on LAN  
  participant N2 as Node on LAN  
  N1->>N2: UDP Broadcast Announce  
  N2->>N1: ECC initialization  
  N1->>N2: ECC establishment  
  N1->>N2: "Data communication clear text token, cypher ecc-aes token in UDP payload"  
  N2->>N1: "Data communication response clear text token, cypher ecc-aes token in UDP payload"  
```

### PQC ML-KEM

In the migration transition, the ECC key exchange mechanism is still active. Next diagram only shows the PQC key exchange mechanism been involved upon UDP broadcast from a node on the network.

```mermaid  
  sequenceDiagram  
  participant N1 as Node on LAN  
  participant N2 as Node on LAN  
  N1->>N2: UDP Broadcast Announce  
  N2->>N1: "TCP Unicast Announce includes public key and empty cypher text (reserved for later dev)"
  N1->>N1: "Encapsulate cypher text with received public key and create shared key to store in secret key"  
  N1->>N2: "PQC initialization includes public key and cypher text encoded with remote public key received"  
  N2->>N2: "Decapsulate cypher text with private key and create decrypted shared key to store in secret key"  
  N1->>N1: "PQC secret key registration"
  N2->>N2: "PQC secret key registration"
  N1->>N2: "Alternative data communication clear text token, cypher ecc-aes token, cypher pqc-aes token in unsecured UDP"  
  N2->>N1: "Alternative data communication response clear text token, cypher ecc-aes token, cypher pqc-aes token in unsecured UDP"  
```

## PQC Sizes to consider

The migration would requires to fragment UDP communications to support the new sizes of the PQC keys. The key exchange mechanism will be accomplished with the TCP protocol. For more details of the new sizes of the PQC keys see [PQC Sizes table](pqc-sizes.md)

*See [PQC Sizes table](pqc-sizes.md) for ML-KEM and ML-DSA key size reference.*

## UDP Payloads

```mermaid
packet
title UDP Packet
+16: "Source Port"
+16: "Destination Port"
32-47: "Length"
48-63: "Checksum"
64-95: "Data (variable length)"
```

<div align="center">
    
|Payload type|Position|Description|
|--|--|--|
|Broadcast UDP announcment|64-TBC|"PQC Public Key (N-TBC bytes)"|
|ECC initialization|64-TBC|"TBC (N bytes)"|
|ECC establishment|64-TBC|"TBC (N bytes)"|
|Data communication (and response)|64-65|"Cypher tag (1 byte)"|
|Data communication (and response)|65-TBC|"Data clear-text (N bytes)"|
|Data communication (and response)|65-TBC|"Data ecc-aes encrypted (N bytes)"|
|Data communication (and response)|65-TBC|"Data pqc-aes encrypted (N bytes)" once PQC established|

</div>

## TCP Payloads

```mermaid  
packet
title TCP Packet
    0-15: "Source Port"
    16-31: "Destination Port"
    32-63: "Sequence Number"
    64-95: "Acknowledgment Number"
    96-99: "Data Offset"
    100-105: "Reserved"
    106: "URG"
    107: "ACK"
    108: "PSH"
    109: "RST"
    110: "SYN"
    111: "FIN"
    112-127: "Window"
    128-143: "Checksum"
    144-159: "Urgent Pointer"
    160-191: "(Options and Padding)"
    192-255: "Data (variable length)"
```

<div align="center">
    
|Payload type|Position|Description|
|--|--|--|
|Unicast TCP announcment|192-TBC|"ECC Public Key (N-TBC bytes)"|
|PQC initialization|192-TBC|"TBC (N bytes)"|
|PQC establishment|192-TBC|"TBC (N bytes)"|
|Data communication (and response)|192-193|"Cypher tag (1 byte)"|
|Data communication (and response)|193-TBC|"Data clear-text (N bytes)"|
|Data communication (and response)|193-TBC|"Data ecc-aes encrypted (N bytes)" if ECC established|
|Data communication (and response)|193-TBC|"Data pqc-aes-encrypted (N bytes)"|

</div>
