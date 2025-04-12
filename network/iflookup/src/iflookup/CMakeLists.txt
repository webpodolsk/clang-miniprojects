cmake_minimum_required(VERSION 3.13)

set(IFLOOKUP_TARGET iflookup)
set(IFLOOKUP_SRC    iflookup.c)

add_library(${IFLOOKUP_TARGET} STATIC ${IFLOOKUP_SRC})
