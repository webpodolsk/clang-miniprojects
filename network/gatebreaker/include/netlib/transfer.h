// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#pragma once

#include <iflookup/iflookup.h>
#include <netlib/packet.h>

int createAFPacketRawSocket(netifdata*, int protocol);

int sendFrame(int sockfd, PACKET_D*);

PACKET_D recvFrame(int sockfd);