# ifndef _TREESTACK_H
# define _TREESTACK_H
# include <stdint.h>
# include <stdbool.h>
# include "huffman.h"

typedef struct stackTree
{
        uint32_t size;
        uint32_t top;
        treeNode **entries;
} stackTree;

# define INVALID 0xDeadD00d

struct stackTree *newStackT();

void delStackT(struct stackTree *);

treeNode *popT(struct stackTree *);

void pushT(struct stackTree *, treeNode *);

bool emptyT(struct stackTree *);
# endif
