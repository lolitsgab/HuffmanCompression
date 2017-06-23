//Code based and outlined from Darrell Long's outline of the stack structure.
# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>
# include <stdbool.h>
# include "treestack.h"

# define MIN_STACKT 10

stackTree *newStackT()
{
    stackTree *s = (stackTree *) calloc(MIN_STACKT, sizeof(stackTree)); // Allocate space for array
    s->size = MIN_STACKT; //set the min
    s->top  = 0; //pew
    s->entries = (treeNode **) calloc(MIN_STACKT, sizeof(treeNode*)); // Make the array
    return s;
}

// delete stack by freeing it
void delStackT(stackTree *s)
{
    free(s->entries);
    free(s);
    return;
}

// pop the stack
treeNode *popT(stackTree *s)
{
    if (s->top > 0) //return popped thing
    {
        s->top -= 1;
        return (s->entries[s->top]);
    }
    else // return an invalid node
    {
        treeNode *pew = malloc(sizeof(treeNode));
        return pew;
    }
}

// push a treenode onto stack
void pushT(stackTree *s, treeNode *i)
{
    if (s->top == s->size) // resize if too small
    {
        s->size *= 2;
        s->entries = (treeNode **) realloc(s->entries, s->size * sizeof(treeNode*)); // push onto stack
    }
    s->entries[s->top] = i;
    s->top += 1;
    return;
    
}

bool emptyT(stackTree *s)
{
    return s->top == 0;
}
