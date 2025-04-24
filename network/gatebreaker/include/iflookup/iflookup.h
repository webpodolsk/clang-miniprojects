// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
/*
                    О проекте (◎ ◎)ゞ

    Задача:  разработать  метод,  возвращаюший  массив
    структур,  представляющих  сетевой  интерфейс в ОС

    Я  всегда  хотел  научиться использовать glibc для
    работы  с  сетью.  Эти  навыки  пригодятся проекту
    https://github.com/webpodolsk/gatebreaker,       а
    в качестве университетской практики решил написать
    это  на  чистом  С, читая документацию в man pages
*/
#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/*
    Структура,   представляющая  сетевой  интерфейс  с
    данными о ней
*/
typedef struct netifdata
{
    char devname[32];
    uint32_t devid;
    bool isActive;
    uint8_t mac[6];
    union {
        uint32_t value;
        uint8_t array[4];
    } ipv4;
    union {
        __uint128_t value;
        uint8_t array[16];
    } ipv6;
} netifdata;

/*
    Возвращает  количество  сетевых интерфейсов из файла
    /proc/net/dev. Нужен для определения размера массива
*/
uint8_t getNICAmount();

/*
    Заполняет массив структур, представляющих интерфейсы,
    данными  при  помощи системных вызовов POSIX. amount
    получается при помощи getNICAmount()
*/
void getInterfacesAddr(netifdata *ifaddrs, size_t amount);
