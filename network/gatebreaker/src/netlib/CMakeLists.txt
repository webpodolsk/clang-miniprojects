cmake_minimum_required(VERSION 3.13)

set(NETLIB_TARGET netlib)
set(NETLIB_SOURCE arp.c packet.c frame.c transfer.c)

add_library(${NETLIB_TARGET} STATIC ${NETLIB_SOURCE})