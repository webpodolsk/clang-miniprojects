#pragma once

#include <stdint.h>

#define STACKLIB_MALLOC_ERROR -1

typedef struct _StackNode
{
    int64_t value;
    struct _StackNode* next;
} _StackNode;

typedef _StackNode* Stack;

Stack createStack();

int isEmptyStack(Stack* s);

int pushStack(Stack* s, int64_t value);

int64_t popStack(Stack* s);

void freeStack(Stack* s);