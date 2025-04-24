// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#pragma once

#include <netlib/packet.h>

#define ETH_FRAME_SIZE 14

typedef struct EthernetFrame
{
    uint8_t source[6];
    uint8_t destination[6];
    uint16_t etherType;
} EthernetFrame;

int packFrame(PACKET_D*, EthernetFrame*);

EthernetFrame unpackFrame(PACKET_D*);