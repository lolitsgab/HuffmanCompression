# include <stdio.h>
# include <stdlib.h>
# include "huffman.h"
# include "stack.h"
# include "treestack.h"
# include <unistd.h>
# include <fcntl.h>
# include <string.h> /* memset */


int main(int argc, char *argv[])
{

    uint32_t magicNum = 0xdeadd00d;
    uint32_t readNum;
    uint64_t fileLength;
    uint16_t  treeLength;
    int option;
    bool toSTD = true;
    bool verbose = false;
    char *outName = NULL;
    char *inName = NULL;
    opterr = 0;
    /*************** GETOPT *******************************/
    while ((option = getopt(argc , argv , " i:o:v") ) != -1)
    {
        switch (option)
        {
            case 'i':
            {
                inName = optarg;
                break;
            }
            case 'o':
            {
                toSTD = false;
                outName = optarg;
                break;
            }
            case 'v':
            {
                verbose = true;
                break;
            }
            default:
	    {
                fprintf(stderr, "Improper arguments. Make sure that you specify input with -i and make sure correct flags are give.");
                return 2;
       	    }
	 }
        
    }
    /*************** GETOPT *******************************/

	if(argc == 1)
	{
		printf("Error: No inputs given. Terminating program\n");
		return -1;
	} 
    int readMe = open(inName, O_RDONLY);
    int write2Me  = open(outName, O_CREAT|O_WRONLY, 0777);
    
    (void) read(readMe, &readNum, 4); //load first 4 bits (maginc num)
    
    if(!(readNum == magicNum)) //check if compressed 
    {
        printf("Not a compressed file");
        return -1;
    }
    
    (void) read(readMe, &fileLength, 8);
    (void) read(readMe, &treeLength, 2); // read length into buffer
    
    uint8_t treeBuf[treeLength]; // Create a buffer to read the tree chars into
    (void)read(readMe, treeBuf, treeLength); // read tree into treeBuff
    treeNode *tree, *temp;
    tree = loadTree(treeBuf, treeLength); // Load the tree into a huffman structure (binary tree)
    temp = tree; //make copy of tree
    uint8_t codeBuf[1]; //read byte by byte
    if(read(readMe, codeBuf, 1) < 0) // Check if it exists
    {
        printf("Nothing was read, check your file");
    }

    if(verbose) //print verbose if true
    {
        printf("Original %llu bits: tree (%llu)\n", (unsigned long long)fileLength * 8, (unsigned long long)treeLength);
    }
    
    uint32_t count = 0; // char count tracker
    
    
    //search time for binary tree is log(n)
    int slide = 1;
    for(uint32_t i = 0; count < fileLength; slide++, i++) // itterate through the buffer
    {
        if( ((codeBuf[i/8] & (0x1 << i%8)) != 0 ) ) // bitmask check if bit is 0 or 1
        {
            temp = temp->right; //go to right of tree
        }
        else
        {
            temp = temp->left; //go to left of tree
        }
        if(temp->leaf) // is it a leaf?
        {
            if(toSTD) //print to stdout?
            {
                printf("%c", temp->symbol);
                fflush(stdout);
            }
            else //write to file
            {
               if(write(write2Me, &temp->symbol, 1) == -1) // check if write error
               {
                   perror("Write() error");
               }
            }
            temp = tree; // reset tree to home node
            count++;
        }
        if(slide % 8 == 0) //sliding window
        {
            (void) read(readMe, codeBuf, 1);
            i = -1;
        }
    }
    delTree(tree); //Delete tree
    return 0;
}
