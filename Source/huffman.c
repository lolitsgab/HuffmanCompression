# include "huffman.h"
# include "treestack.h"
# include "stack.h"
# include <stdio.h>
# include <unistd.h>

treeNode * newNode ( uint8_t s, bool l, uint64_t c)
{
    treeNode *nNode = calloc(1, sizeof(treeNode));
    nNode -> leaf = l;
    nNode -> count = c;
    nNode -> symbol = s;
    return nNode;
}

// Delete a tree
void delTree (treeNode *t)
{
    if(!t->leaf)
    {
        delTree(t->left);
        delTree(t->right);
    }
    delNode(t);
    return;
}

// Dump a Huffman tree onto a file
static uint16_t len = 0; // keep track of buffer pos
static uint16_t leaves = 0; // keep track of leaf count
void dumpTree ( treeNode *t,int file)
{
    uint8_t buf[768]; // buffer
    len = 0;
    dumpTreeHelper(t, buf); //start recursion
    leaves = (leaves*3)-1; //write the number of leaves *3 -1
    write(file, &leaves, 2); // same as above
    write(file, buf, len); // write length of tree
	
    return;
}

void dumpTreeHelper(treeNode *t, uint8_t *buf)
{
    if(t->leaf) // if it is a leaf, put leaf in buffer
    {
        buf[len++] = 'L'; //put leaf in buffer
        buf[len++] = t->symbol; // put symbol next to L in buffer
	leaves++; // increment amount of leaves
        return;
    }
    dumpTreeHelper(t->left, buf); // go left
    dumpTreeHelper(t->right, buf); // go right
    buf[len++] = 'I';
    return;
}

// Build a tree from the saved tree
treeNode * loadTree (uint8_t savedTree [], int size)
{
    //size = (size-1) / 3;
    struct stackTree *tree = newStackT();   // Stack of treeNodes
    int i;
    for(i = 0; i < size; i++)            //run until max possible (because ascii limit)
    {
        if(savedTree[i] == 'L')       //check when there is a leaf
        {
            treeNode *tempNode = newNode(savedTree[++i], true, 0);//  make a new node with symbol next to L
            pushT(tree, tempNode); //push onto stack
        }
        else if(savedTree[i] == 'I')   //check if internal node
        {
            treeNode *tempPop2 = popT(tree);   //pop tree here
            treeNode *tempPop1 = popT(tree);   //and here
            treeNode *tempJoin = join(tempPop1, tempPop2);   //and join them two
            pushT(tree, tempJoin);//push them back onto the stac
            if(i == 16)
            {
                i = 16;
            }
        }
    }
    treeNode * retTree = popT(tree); //return the main tree
    delStackT(tree);
    return retTree;
}
// Join trees
treeNode * join ( treeNode *l, treeNode *r) // Join two subtrees
{
    treeNode *nNode = newNode('$', false, (l->count + r->count));
    nNode->left = l;
    nNode->right = r;
    return nNode;
}

 // Parse a Huffman tree to build codes
void buildCode(treeNode *t, code s, code codeTable[256])
{
	if(t && t -> leaf) //check if leaf
	{
		codeTable[t -> symbol] = s; //set spot in code table, for that symbol
		return;
	}	
	else
	{
		uint32_t tmp; //temp code for popping
		if(t -> left)
		{
			pushCode(&s, 0); //push 0 for left
			buildCode(t -> left, s, codeTable); //go left
			popCode(&s, &tmp); //pop excess code
		}
		
		if(t -> right)
		{
			pushCode(&s, 1); //push 1 for right
			buildCode(t -> right, s, codeTable); //go right
			popCode(&s, &tmp); //pop excess code
		}
		return;
	}	    
}
