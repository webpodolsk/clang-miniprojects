// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#pragma once

#include <stdint.h>
#include <stdlib.h>

#define NETLIB_ETH_MTU 1500

typedef struct PACKET_D
{
    uint8_t* ptr;
    size_t size;
} PACKET_D;

PACKET_D createPacket();

void clearPacket(PACKET_D*);