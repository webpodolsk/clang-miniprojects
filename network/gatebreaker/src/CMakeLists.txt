cmake_minimum_required(VERSION 3.13)
project(gatebreaker)

# Установка главного таргета
set(MAIN_TARGET build)  # !! Для работоспособности скрипта ./makeit значение нельзя менять 
set(MAIN_SOURCE main.c)

add_executable(${MAIN_TARGET} ${MAIN_SOURCE})

# Наследуемое добавление пути к заголовкам
include_directories(${PROJECT_SOURCE_DIR}/../include)

#               Подключение библиотек
#   -> add_subdirectory(...)
#   -> target_link_libraries(... PRIVATE ${MAIN_TARGET})
add_subdirectory(iflookup)
target_link_libraries(${MAIN_TARGET} iflookup)

add_subdirectory(netlib)
target_link_libraries(${MAIN_TARGET} netlib)

add_subdirectory(prettyterm)
target_link_libraries(${MAIN_TARGET} prettyterm)