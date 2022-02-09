#ifndef PACKET_H
#define PACKET_H

#include <QtGlobal>
#include "config.h"
#include "options.h"
#include "tdist.h"
#include "timestamp.h"

// Kind of packet
struct flags
{
    uchar mode: 3;                      // Mode: 3 is client
    uchar version: 3;                   // Version number of the protocol: 3 or 4
    uchar leap: 2;                      // Alarm condition (clock not synchronized)
};                                      // 8 bits or 1 byte

#pragma pack(push, 1)
// Mandatory structure
struct simple_packet
{
    flags packetFlags;                  // 8 bits: leap indicator(2 bits), version number(3 bits), mode4(3 bits)
    quint8 stratum;                     // 8 bits: Stratum level of local clock
    qint8 poll;                         // 8 bits: Max interval between succesive messages
    qint8 precision;                    // 8 bits: Precision of the local clock

    tdist rootDelay;                    // 32 bits: Total round trip delay time
    tdist rootDispersion;               // 32 bits: Max error from primary clock source
    qint8 referenceIdentifier[4];       // 32 bits: Reference clock identifier

    timestamp referenceTimestamp;       // 64 bits: Reference timestamp seconds + fraction
    timestamp originateTimestamp;       // 64 bits: Originate timestamp seconds + fraction
    timestamp receiveTimestamp;         // 64 bits: Receive timestamp seconds + fraction
    timestamp transmitTimestamp;        // 64 bits: Transmit timestamp seconds + fraction
};                                      // 384 bits or 48 bytes

// Optional security
struct auth
{
    quint32 keyId;                      // 32 bits: Key identifier
    quint8 messageDigest[16];           // 128 bits: Message Digest
};                                      // 160 bits or 20 bytes

// Packet + security
struct full_packet
{
    simple_packet basic;                // 48 bytes basic NTP packet
    auth secure;                        // plus 20 bytes security fields
};                                      // 68 bytes: Full packet size
#pragma pack(pop)

#endif // PACKET_H
