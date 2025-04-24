// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ifaddrs.h>           // Функция получения адресов NIC getifaddrs()
#include <net/if.h>            // Содержит константы со статусами NIC из вызова ioctl()
#include <netinet/in.h>        // Описывает производные структуры sockaddr_in, sockaddr_in6
#include <netpacket/packet.h>  // Описывает производную структуру sockaddr_sll

#include <iflookup/iflookup.h>

uint8_t getNICAmount()
{
    char* statusCode;
    char buffer[256];
    uint8_t amount = -1; // Значение -1 учитывает считываемый EOF
    FILE *fd = fopen("/proc/net/dev", "r");

    if (fd == NULL)
    {
        return -1;
    }

    // Перемещаем указатель на две строки ниже, чтобы миновать заголовки файла
    statusCode = fgets(buffer, sizeof(buffer) / sizeof(buffer[0]), fd);

    if (statusCode == NULL)
    {
        fclose(fd);
        return -1;
    }

    statusCode = fgets(buffer, sizeof(buffer) / sizeof(buffer[0]), fd);

    if (statusCode == NULL)
    {
        fclose(fd);
        return -1;
    }

    // Считаем строки, следующие после заголовков. Их кол-во равно кол-ву интерфейсов
    while (1)
    {
        amount++;
        statusCode = fgets(buffer, sizeof(buffer) / sizeof(buffer[0]), fd);

        if (statusCode == NULL)
        {
            fclose(fd);
            break;
        }
    }

    return amount;
}

void getInterfacesAddr(netifdata *ifaddrs, size_t amount)
{
    struct ifaddrs *ifap, *ifa;
    uint8_t lastfilledstruct;
    size_t devnameStringSize;
    netifdata *ptr;

    getifaddrs(&ifap);

    devnameStringSize = sizeof(ifaddrs[0].devname) / sizeof(ifaddrs[0].devname[0]);
    lastfilledstruct = 0;
    ptr = NULL;
    ifa = ifap;

    // Очистка структуры от произвольных данных из RAM
    memset(ifaddrs, 0, amount * sizeof(netifdata));

    // Обход связанного списка ifaddrs до конца
    while (ifa->ifa_next != NULL)
    {
        // Здесь находим нужный интерфейс из массива структур netifdata,
        // присваиваем  адрес  на  нее указателю ptr и работаем с ним в
        // последующих  проверках.  Это  связано  с особенностью работы
        // функции getifaddrs
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

        // Дальше идет обработка полученных из getifaddr данных и заполнение структуры

        // Заполнение полей адресов (mac, ipv4, ipv6) структуры netifdata
        switch (ifa->ifa_addr->sa_family)
        {
        case AF_PACKET:
            struct sockaddr_ll *af_packet_addr = (struct sockaddr_ll *)ifa->ifa_addr;
            ptr->devid = af_packet_addr->sll_ifindex;
            memcpy(&ptr->mac, &af_packet_addr->sll_addr, 6);
            break;

        case AF_INET:
            struct sockaddr_in *af_inet_addr = (struct sockaddr_in *)ifa->ifa_addr;
            memcpy(&ptr->ipv4, &af_inet_addr->sin_addr.s_addr, 4);
            break;

        case AF_INET6:
            struct sockaddr_in6 *af_inet6_addr = (struct sockaddr_in6 *)ifa->ifa_addr;
            memcpy(&ptr->ipv6, &af_inet6_addr->sin6_addr.s6_addr, 16);
            break;
        }

        ptr = NULL;
        ifa = ifa->ifa_next;
    }

    freeifaddrs(ifap);
}
