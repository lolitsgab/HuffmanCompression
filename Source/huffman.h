//huffman.h provided by DDEL accesed 5/23/17
# ifndef _HUFFMAN_H
# define _HUFFMAN_H
# include <stdint.h>
# include <stdbool.h>
# include <stdlib.h>
# include "code.h"
# ifndef NIL
# define NIL ( void *) 0
# endif

typedef struct DAH treeNode;

struct DAH
{
    uint8_t symbol ;
    uint64_t count ;
    bool leaf ;
    treeNode *left , * right ;
};

// Check if current node is leaf
static inline bool isLeaf(treeNode *t)
{
    return ((t->left) || (t->right)) ? true : false;
}

void dumpTreeHelper(treeNode *t, uint8_t *buf);

// New node , with symbols , leaf or not , a count associated with it
treeNode * newNode ( uint8_t s, bool l, uint64_t c) ;

// Delete a single node
static inline void delNode(treeNode *h)
{
    free(h);
    return;
}

// Delete a tree
void delTree ( treeNode *t) ;

// Dump a Huffman tree onto a file
void dumpTree ( treeNode *t, int file ) ;

// Build a tree from the saved tree
treeNode * loadTree (uint8_t savedTree [], int arrSize) ;


// Step through a tree following the code
int32_t * stepTree ( treeNode *root , treeNode *t, uint32_t code ) ;

void buildCode ( treeNode *t, code s, code table [256]);

//static inline void delNode(treeNode *h)
//{
//    free(h);
//    return;
//}

static inline bool compare ( treeNode *l, treeNode *r)
{
    return l-> count - r-> count ; // l < r if negative , l = r if 0 , ...
}

treeNode * join ( treeNode *l, treeNode *r) ; // Join two subtrees
# endif
