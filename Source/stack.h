//stack.h provided by DDEL accesed 5/23/17
# ifndef _STACK_H
# define _STACK_H
# include <stdint.h>
# include <stdbool.h>

typedef uint32_t item;

typedef struct stack
{
    uint32_t size;
    uint32_t top;
    item *entries;
} stack;

# define INVALID 0xDeadD00d

stack *newStack();

void delStack(stack *);

item pop(stack *);

void push(stack *, item);

bool empty(stack *);
# endif
