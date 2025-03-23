#pragma once

#include <stdint.h>
#include <stdbool.h>

/*
    Представляет  собой  ноду односвязного списка, хранящуюся
    в динамической памяти. Она никак не связана с реализацией
    стека  -  это  просто односвязный список, поверх которого
    пишется  контейнер: будь это стек, очередь с ООП или без.
*/
struct _StackNode
{
    int64_t value;
    struct _StackNode* next;
};

typedef struct _StackNode* StackNode;

/*
    Контейнер  для  односвязного  списка,  реализующий  стек с
    приминением  псевдо  ООП . Данные и методы инкапсулируются
    в  объект  при  помощи  self,  как  в Python. По сути этот
    контейнер  -  просто  ООП  обертка  для указателя на самый
    верхний элемент стека, что и без ООП.
*/
struct _StackObject
{
    // || СВОЙСТВА ||
    StackNode _top;
    size_t _size;

    // ||  МЕТОДЫ  ||
    size_t (*size)(struct _StackObject*);
    bool (*isEmpty)(struct _StackObject*);
    bool (*push)(struct _StackObject*, int64_t);
    int64_t (*pop)(struct _StackObject*);
    void (*delete)(struct _StackObject**);
};

typedef struct _StackObject* Stack;

// Методы стека
size_t _getStackSize(Stack self);

bool _isStackEmpty(Stack self);

bool _pushStack(Stack self, int64_t value);

int64_t _popStack(Stack self);

void _deleteStack(Stack* self);

// Метод создания объекта стека
Stack createStack();