#include <stdio.h>
#include <iflookup/iflookup.h>

int main(int argc, char *argv[])
{
    uint8_t ifamount = getNICAmount();
    netifdata ifaddrs[ifamount];
    getInterfacesAddr(ifaddrs, ifamount);

    for (int i = 0; i < ifamount; i++)
    {
        if (ifaddrs[i].isActive)
        {
            printf("%d: Interface (\033[32m\033[1m%s\033[0m):\n", ifaddrs[i].devid, ifaddrs[i].devname);
        }
        else
        {
            printf("%d: Interface (\033[31m\033[1m%s\033[0m):\n", ifaddrs[i].devid, ifaddrs[i].devname);
        }

        printf("    ");
        for (int j = 0; j < 6; j++)
        {
            printf("%02x", ifaddrs[i].mac[j]);
            if (j < 5)
            {
                printf(":");
            }
        }

        printf("\n    ");
        for (int j = 0; j < 4; j++)
        {
            printf("%d", ifaddrs[i].ipv4.array[j]);
            if (j < 3)
            {
                printf(".");
            }
        }

        printf("\n\n");
    }

    return 0;
}
