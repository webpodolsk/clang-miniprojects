// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include <netlib/packet.h>
#include <netlib/frame.h>
#include <string.h>

int packFrame(PACKET_D* packetd, EthernetFrame* frame)
{
    // Ethernet фрейм обязан инкапсулировать данные сетевого уровня.
    // packetd должен быть чем-то заполнен и packetd->size = 0.
    if (packetd->size <= 0)
    {
        return -1;
    }

    // TODO: Риск вылезти за пределы буффера. Нужно сделать проверку
    // 1. Смещение заголовков и полезной нагрузки для вставки фрейма в начало
    memcpy(packetd->ptr + ETH_FRAME_SIZE, packetd->ptr, packetd->size);
    
    // Добавление Ethernet заголовков
    // TODO: Чтобы избежать повторения кода и магических чисел, нужно вынести логику 
    // добавления заголовков в начало пакета после смещения в отдельную функцию
    memcpy(packetd->ptr + 0, frame->destination, 6);
    memcpy(packetd->ptr + 6, frame->source, 6);
    memcpy(packetd->ptr + 12, &frame->etherType, 2);

    packetd->size += ETH_FRAME_SIZE;

    return 0;
}