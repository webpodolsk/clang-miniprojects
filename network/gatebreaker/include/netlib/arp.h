// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#pragma once

#include <stdbool.h>

#include <netinet/in.h>
#include <netlib/packet.h>
#include <iflookup/iflookup.h>

#define ARP_PACKET_SIZE 42

typedef struct ARPPacket
{
    uint8_t senderMAC[6];
    uint32_t senderIP;
    uint8_t targetMAC[6];
    uint32_t targetIP;
    bool isReply;
} ARPPacket;

int makeNormalARPRequest(netifdata*, uint8_t mac[6], in_addr_t);

int packARP(PACKET_D*, ARPPacket*);

int unpackARP(PACKET_D*, ARPPacket*);