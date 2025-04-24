// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <netlib/packet.h>

PACKET_D createPacket()
{
    PACKET_D packet; 
    packet.size = 0;
    packet.ptr = (uint8_t*)malloc(NETLIB_ETH_MTU);

    // Поскольку функция возвращает структуру PACKET_D,
    // проверка на NULL делегируется вызывающей функции
    if (packet.ptr == NULL)
    {
        return packet;
    }

    memset(packet.ptr, 0, NETLIB_ETH_MTU);

    return packet;
}

void clearPacket(PACKET_D* packet)
{
    free(packet->ptr);
    packet->ptr = NULL;
}