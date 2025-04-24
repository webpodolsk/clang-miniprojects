#!/usr/bin/zsh

# 1. Константы проекта
PROJECT_NAME=gatebreaker  #  <-- Ввести здесь название проекта
BUILD_NAME=build
PROJECT_ROOT_DIR=$(pwd)
BUILD_TMP_DIR=${PROJECT_ROOT_DIR}/tmp
SOURCES_DIR=${PROJECT_ROOT_DIR}/src

# 2. Состояния выполнения скрипта
isDebug=0
isRun=0

# 3. Парсинг ключей для изменения состояний скрипта
#                          (смотреть пункт №2)
while [ -n "$1" ]; do
    case "$1" in
        -h)   echo "-d --> Включение сообщений отладки\n-r --> Запуск после успешной сборки\n-pvs --> создание PVS Studio fullHTML отчета"; exit ;;
        -d)   isDebug=1 ;;
        -r)   isRun=1 ;;
        -pvs) isPVS=1 ;;
    esac
    shift
done

# 3. Компиляция
if [ $isDebug -eq 1 ]; then
    cmake -S $SOURCES_DIR -B $BUILD_TMP_DIR
    pvs-studio-analyzer trace -- make --directory=$BUILD_TMP_DIR
else
    cmake -S $SOURCES_DIR -B $BUILD_TMP_DIR >/dev/null
    pvs-studio-analyzer trace -- make --directory=$BUILD_TMP_DIR >/dev/null
fi

if [ $isPVS -eq 1 ]; then
    rm -r report
    pvs-studio-analyzer analyze -o strace_out
    plog-converter -a GA:1,2 -t fullhtml strace_out -o report
fi

# 4. Переименование и очистка
mv $BUILD_TMP_DIR/$BUILD_NAME $PROJECT_ROOT_DIR/$PROJECT_NAME
rm -r $BUILD_TMP_DIR

if [ $isRun -eq 1 ]; then
    ./$PROJECT_NAME
fi
