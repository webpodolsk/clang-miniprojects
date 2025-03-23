/*
    Задача: переписать стек с использованием псевдо объектно-ориентированного
    программирования.  Для  этого  нужно заменить указатель на последнюю ноду
    стека  на  объект структуры с метаданными о данном стеке и указателями на
    различные функции.
    
    В нем будет своеобразная инкапсуляция: данные и методы,
    манипулирующие данными, будут объединены в один объект при помощи указателей
    на функции. Сокрытие обеспечивается нотацией при помощи __, как в Python.
*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "stack.h"

size_t _getStackSize(Stack self)
{
    return self->_size;
}

bool _isStackEmpty(Stack self)
{
    return self->_top == NULL ? true : false;
}

bool _pushStack(Stack self, int64_t value)
{
    StackNode newNode = (StackNode)malloc(sizeof(struct _StackNode));

    if (newNode == NULL) {
        return false;
    }

    StackNode lastNode = self->_top;

    newNode->value = value;
    newNode->next = lastNode;
    self->_top = newNode;
    self->_size++;

    return true;
}

int64_t _popStack(Stack self)
{
    StackNode lastNode = self->_top;
    int64_t value = lastNode->value;
    free(lastNode);

    StackNode nextNode = self->_top->next;
    self->_top = nextNode;

    return value;
}

void _deleteStack(Stack* self)
{
    while ((*self)->_top != NULL) {
        printf("Node value:\t%d\n", (*self)->_top->value);
        StackNode nextNode = (*self)->_top->next;
        free((*self)->_top);
        (*self)->_top = nextNode;
    }

    free((*self));
    *self = NULL;
}

Stack createStack()
{
    /*
        Поскольку тип возвращаемых данных - это указатель на
        структуру,  проверку на NULL нужно выполнять в месте
        вызова, например, в main()
    */
    Stack stackObject = (Stack)malloc(sizeof(struct _StackObject));

    if (stackObject == NULL)
    {
        return NULL;
    }

    stackObject->_top = NULL;
    stackObject->_size = 0;
    
    stackObject->size = &_getStackSize;
    stackObject->isEmpty = &_isStackEmpty;
    stackObject->push = &_pushStack;
    stackObject->pop = &_popStack;
    stackObject->delete = &_deleteStack;

    return stackObject;
}