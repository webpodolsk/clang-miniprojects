// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>       // --> close()
#include <net/ethernet.h> // --> ETH_P_ARP
#include <netinet/in.h>   // --> in_addr_t
#include <arpa/inet.h>    // --> htons()
#include <sys/time.h>     // --> timeval for setsockopt()

#include <netlib/transfer.h>
#include <netlib/packet.h>
#include <netlib/frame.h>
#include <netlib/arp.h>

/*
    Эта функция зависит от функции netlib/transfer.h, потому что в ней находится
    статическая  переменная  сокета. Чтобы  при каждом стандартом ARP запросе не
    создавать новый сокет, разумно использовать уже готовое решение.
*/
int makeNormalARPRequest(netifdata* interface, uint8_t mac[6], in_addr_t ip)
{
    uint8_t broadcastMAC[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

    // TODO: ПРОВЕРКА РАБОТЫ С ПАМЯТЬЮ!!
    ARPPacket arp;
    memcpy(arp.senderMAC, interface->mac, 6);
    memcpy(&arp.senderIP, interface->ipv4.array, 4);
    memcpy(arp.targetMAC, broadcastMAC, 6);
    memcpy(&arp.targetIP, &ip, 4);
    arp.isReply = false;

    // TODO: ПРОВЕРКА РАБОТЫ С ПАМЯТЬЮ!! x2
    EthernetFrame frame;
    memcpy(frame.destination, broadcastMAC, 6);
    memcpy(frame.source, interface->mac, 6);
    frame.etherType = htons(0x0806);

    PACKET_D packetd = createPacket();

    if (packetd.ptr == NULL)
    {
        return -1;
    }

    int sockfd = createAFPacketRawSocket(interface, ETH_P_ARP);

    //https://stackoverflow.com/questions/2876024/linux-is-there-a-read-or-recv-from-socket-with-timeout
    struct timeval val;
    val.tv_sec = 0;
    val.tv_usec = 450000;

    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (void*)&val, sizeof(val));

    packARP(&packetd, &arp);
    packFrame(&packetd, &frame);
    sendFrame(sockfd, &packetd);

    PACKET_D recvPacketd = recvFrame(sockfd);

    if (errno == EAGAIN || errno == EWOULDBLOCK)
    {
        return -1;
    }

    ARPPacket unpackedARP;

    unpackARP(&recvPacketd, &unpackedARP);

    memcpy(mac, unpackedARP.senderMAC, 6);

    clearPacket(&recvPacketd);
    clearPacket(&packetd);
    close(sockfd);

    return 0;
}

int packARP(PACKET_D* packetd, ARPPacket* arp)
{
    // ARP пакеты ничего в себя не могут инкапсулировать.
    // packetd->size должен быть пуст.
    if (packetd->size != 0)
    {
        return -1;
    }

    uint8_t constHeader[8] = {
        0x00, 0x01,  // Hardware type
        0x08, 0x00,  // Protocol type
        0x06,        // Hardware size
        0x04,        // Protocol size
        0x00, 0x00   // Opcode
    };

    if (arp->isReply == true)
    {
        constHeader[7] = 0x02;
    }
    else
    {
        constHeader[7] = 0x01;
    }

    memcpy(packetd->ptr + packetd->size, constHeader, sizeof(constHeader) / sizeof(constHeader[0]));
    packetd->size += sizeof(constHeader) / sizeof(constHeader[0]);

    memcpy(packetd->ptr + packetd->size, arp->senderMAC, 6);
    packetd->size += 6;

    memcpy(packetd->ptr + packetd->size, &arp->senderIP, 4);
    packetd->size += 4;

    memcpy(packetd->ptr + packetd->size, arp->targetMAC, 6);
    packetd->size += 6;

    memcpy(packetd->ptr + packetd->size, &arp->targetIP, 4);
    packetd->size += 4;

    return 0;
};

int unpackARP(PACKET_D* packetd, ARPPacket* arppacket)
{
    if (packetd->ptr[12] != 0x08 && packetd->ptr[13] != 0x06)
    {
        printf("Ethertype error: this is not the ARP protocol\n");
        return -1;
    }

    // TODO: Проверки функций работы с памятью
    memcpy(arppacket->senderMAC, packetd->ptr + 22, 6);
    memcpy(&arppacket->senderIP, packetd->ptr + 28, 4);
    memcpy(arppacket->targetMAC, packetd->ptr + 32, 6);
    memcpy(&arppacket->targetIP, packetd->ptr + 38, 4);

    return 0;
}