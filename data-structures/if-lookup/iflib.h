#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/*
    Во время сбора информации о девайсах в цикле
    постепенно  будут  заполняться  структуры  с
    необходимой информации о них
*/
typedef struct netifdata
{
    char     devname[32];
    uint8_t  mac[6];
    uint8_t  ipv4[4];
    uint8_t  ipv6[16];
    bool     isActive;
} netifdata;

uint8_t getNICAmount();

bool getInterfacesAddr(netifdata* ifaddrs, size_t amount);