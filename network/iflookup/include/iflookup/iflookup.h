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
#include <stdlib.h>
#include <stdint.h>

/*
    Структура,   представляющая  сетевой  интерфейс  с
    данными о ней
*/
typedef struct netifdata
{
    char     devname[32];
    uint8_t  mac[6];
    uint8_t  ipv4[4];
    uint8_t  ipv6[16];
    bool     isActive;
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
bool getInterfacesAddr(netifdata* ifaddrs, size_t amount);
