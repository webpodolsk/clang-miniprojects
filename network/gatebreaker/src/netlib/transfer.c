// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <unistd.h>
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <arpa/inet.h>

#include <iflookup/iflookup.h>
#include <netlib/packet.h>

int createAFPacketRawSocket(netifdata* interface, int protocol)
{
    int errcode;
    int sockfd = socket(AF_PACKET, SOCK_RAW, htons(protocol));

    if (sockfd == -1)
    {
        perror("Socket creating error");
        return -1;
    }

    struct sockaddr_ll bindingData;
    bindingData.sll_family = AF_PACKET;
    bindingData.sll_protocol = htons(protocol);
    bindingData.sll_ifindex = interface->devid;
    bindingData.sll_hatype = 0;
    bindingData.sll_pkttype = 0;
    bindingData.sll_halen = 6;
    memcpy(bindingData.sll_addr, interface->mac, 6);

    errcode = bind(sockfd, (struct sockaddr*)&bindingData, sizeof(bindingData)); // -V641

    if (errcode == -1)
    {
        perror("Bind error");
        close(sockfd);
        return -2;
    }

    return sockfd;
}

int sendFrame(int sockfd, PACKET_D* packetd)
{
    send(sockfd, packetd->ptr, packetd->size, 0);

    return 0;
}

// TODO: Реализовать проверку ошибки при получении
PACKET_D recvFrame(int sockfd)
{
    PACKET_D recvPacketd = createPacket();
    recvPacketd.ptr = (uint8_t *)malloc(NETLIB_ETH_MTU);

    // Поскольку функция возвращает структуру PACKET_D,
    // проверка на NULL делегируется вызывающей функции
    if (recvPacketd.ptr == NULL)
    {
        return recvPacketd;
    }

    ssize_t recvBytesAmount = recv(sockfd, recvPacketd.ptr, NETLIB_ETH_MTU, 0);

    recvPacketd.size = recvBytesAmount;

    return recvPacketd;
}