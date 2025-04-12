cmake_minimum_required(VERSION 3.13)
project(iflookup)

# Эти константы будут наследоваться в поддиректориях
set(MAIN_TARGET build)
set(MAIN_SRC    main.c)

include_directories(${PROJECT_SOURCE_DIR}/../include)
add_executable(${MAIN_TARGET} ${MAIN_SRC})

# Подключение библиотек
add_subdirectory(iflookup)
target_link_libraries(${MAIN_TARGET} PRIVATE iflookup)
