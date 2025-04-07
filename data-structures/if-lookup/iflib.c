#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include <netpacket/packet.h>  // Описывает производную структуру sockaddr_sll
#include <netinet/in.h>        // Описывает производные структуры sockaddr_in, sockaddr_in6
#include <ifaddrs.h>           // Функция получения адресов NIC getifaddrs() 
#include <net/if.h>            // Содержит константы со статусами NIC вызова ioctl()

#include "iflib.h"

/*
    Возвращает количество сетевых интерфейсов
*/
uint8_t getNICAmount()
{
    char buffer[256];
    uint8_t amount = -1;  // Значение -1 учитывает считываемый EOF
    FILE* fd = fopen("/proc/net/dev", "r");
    
    if (fd == NULL) 
    {
        return -1;
    }

    fgets(buffer, sizeof(buffer) / sizeof(buffer[0]), fd);
    fgets(buffer, sizeof(buffer) / sizeof(buffer[0]), fd);

    if (buffer == NULL) 
    {
        return -1;
    }
    
    while (feof(fd) == 0) 
    {
        amount++;
        fgets(buffer, sizeof(buffer) / sizeof(buffer[0]), fd);
    }

    fclose(fd);

    return amount;
}

bool getInterfacesAddr(netifdata* ifaddrs, size_t amount)
{
    struct ifaddrs *ifap, *ifa;
    uint8_t lastfilledstruct;
    size_t devnameStringSize;
    netifdata* ptr;

    getifaddrs(&ifap);

    devnameStringSize = sizeof(ifaddrs[0].devname) / sizeof(ifaddrs[0].devname[0]);
    lastfilledstruct = 0;
    ptr = NULL;
    ifa = ifap;
    

    memset(ifaddrs, 0, amount * sizeof(netifdata));

    while (ifa->ifa_next != NULL) 
    {
        // Ассоциирую текущий [f]ifa с нужной структурой в 
        // массиве и заполняю данными об адресе
        for (int i = 0; i < amount; i++) 
        {
            if (strcmp(ifaddrs[i].devname, ifa->ifa_name) == 0) 
            {
                ptr = &ifaddrs[i];
                break;
            }
        }

        // Условие выполняется, когда интерфейс появляется впервые
        if (ptr == NULL) 
        {
            strncpy(ifaddrs[lastfilledstruct].devname, ifa->ifa_name, devnameStringSize);
            ptr = &ifaddrs[lastfilledstruct];
            lastfilledstruct++;

            // https://man7.org/linux/man-pages/man7/netdevice.7.html
            if (ifa->ifa_flags & IFF_RUNNING)
            {
                ptr->isActive = true;
            }
            else
            {
                ptr->isActive = false;
            }
        }

        // Заполнение полей адресов (mac, ipv4, ipv6) структуры netifdata
        switch (ifa->ifa_addr->sa_family) 
        {
            case AF_PACKET:
                struct sockaddr_ll* af_packet_addr = (struct sockaddr_ll*)ifa->ifa_addr;
                memcpy(&ptr->mac, &af_packet_addr->sll_addr, 6);
                break;

            case AF_INET:
                struct sockaddr_in* af_inet_addr = (struct sockaddr_in*)ifa->ifa_addr;
                memcpy(&ptr->ipv4, &af_inet_addr->sin_addr.s_addr, 4);
                break;

            case AF_INET6:
                struct sockaddr_in6* af_inet6_addr = (struct sockaddr_in6*)ifa->ifa_addr;
                memcpy(&ptr->ipv6, &af_inet6_addr->sin6_addr.s6_addr, 16);
                break;
        }

        ptr = NULL;
        ifa = ifa->ifa_next;
    }

    freeifaddrs(ifap);
}