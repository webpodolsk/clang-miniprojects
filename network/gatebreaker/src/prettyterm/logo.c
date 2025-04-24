// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include <stdio.h>

void prettytermPrintLogo()
{
    printf(" ▄▄ •  ▄▄▄· ▄▄▄▄▄▄▄▄ .▄▄▄▄· ▄▄▄  ▄▄▄ . ▄▄▄· ▄ •▄ ▄▄▄ .▄▄▄\n");
    printf("▐█ ▀ ▪▐█ ▀█ •██  ▀▄.▀·▐█ ▀█▪▀▄ █·▀▄.▀·▐█ ▀█ █▌▄▌▪▀▄.▀·▀▄ █·\n");
    printf("▄█ ▀█▄▄█▀▀█  ▐█.▪▐▀▀▪▄▐█▀▀█▄▐▀▀▄ ▐▀▀▪▄▄█▀▀█ ▐▀▀▄·▐▀▀▪▄▐▀▀▄ \n");
    printf("▐█▄▪▐█▐█ ▪▐▌ ▐█▌·▐█▄▄▌██▄▪▐█▐█•█▌▐█▄▄▌▐█ ▪▐▌▐█.█▌▐█▄▄▌▐█•█▌\n");
    printf("·▀▀▀▀  ▀  ▀  ▀▀▀  ▀▀▀ ·▀▀▀▀ .▀  ▀ ▀▀▀  ▀  ▀ ·▀  ▀ ▀▀▀ .▀  ▀\n");
    printf("        https://github.com/webpodolsk/gatebreaker\n\n");
}

void debug(char* message)
{
    printf("[\033[1m\033[33mDEBUG\033[0m] %s", message);
}

void error(char* message)
{
    printf("[\033[1m\033[31mERROR\033[0m] %s", message);
}

void success(char* message)
{
    printf("[\033[1m\033[32mSUCCESS\033[0m] %s", message);
}

void bold(char* message)
{
    printf("\033[1m%s\033[0m", message);
}

void clearOutput()
{
    printf("\033c");
}