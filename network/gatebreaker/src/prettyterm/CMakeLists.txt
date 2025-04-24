cmake_minimum_required(VERSION 3.13)

set(PRETTYTERM_TARGET prettyterm)
set(PRETTYTERM_SOURCE logo.c)

add_library(${PRETTYTERM_TARGET} STATIC ${PRETTYTERM_SOURCE})