# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <errno.h>
# include <unistd.h>
# include <fcntl.h>
# include <ctype.h>
# include <float.h>
# include "queue.h"
# include "huffman.h"
# include "code.h"
# include "bv.h"
# define Q_SIZE 512 //linear queue must account for overflow. Overcompensating for safety
# define SIZE 512 //bugger size
# define HISTLEN 256 //histogram length


int main(int argc, char *argv[])
{
    uint64_t histogram[HISTLEN];//make histogram of 256
    uint8_t buffer[SIZE]; //buffer for reading
    int length; //length for reading
    queue *q = newQueue(Q_SIZE); //priority queue
    int doc; //document for open and read
    treeNode *root; //tree node for merging, will end up being root
    uint64_t charCount = 0; //number of literal characters in file
    uint16_t leafCount = 0; //number of active histogram nodes (* 3 + 1)
    
    int option; //getopt int
    bool toSTD = true; //getopt: print to stdout?
    bool verbose = false; //getopt: verbose?
    char *outName = NULL; //getopt: if there is output
    char *inName = NULL; //getopt: if there is input
    opterr = 0;
    
    while((option = getopt(argc, argv , "i:o:v")) != -1)
    {
        switch(option)
        {
            case'i':
            {
                inName = optarg;
                break;
            }
            case'o':
            {
                toSTD = false;
                outName = optarg;
                break;
            }
            case'v':
            {
                verbose = true;
                break;
            }
            default:
            {
                fprintf(stderr, "Unknown flag or argument");
                return -1;
            }
        }
    }
    
    if(inName == NULL)
    {
        fprintf(stderr, "No input file deteced");
        return -1;
    }
    
    /***            MAKE HISTOGRAM             ***/
    
    for(int h = 0; h < 256; h++)
    {
        histogram[h] = 0; //set it all to 0
    }
    
    histogram[0] = 1; //0 starts at 1
    histogram[255] = 1; //255 starts at 1
    
    doc = open(inName, O_RDONLY); //open file
    if(doc == -1)
    {
        printf("[ERROR] Cannot open input file");
        return -1;
    }
    do
    {
        length = read(doc, buffer, 512); //read
        for(int i = 0; i < length; i++)
        {
            int character = buffer[i]; //each read character has a value
            histogram[character] += 1; //and increment the histogram
        }
    }while(length == 512); //if we didn't reach eof
    
    for(int j = 0; j < HISTLEN; j++)  //print active histogram nodes
    {
        if(histogram[j] > 0)
        {
            charCount += histogram[j]; //total number of characters will be all histogram nodes added together
            leafCount++; //each histogram node means there is another leaf
        }
    }
    
    if(charCount > 2)
    {
        charCount = charCount-2; //darrells had a count that was 2 less than the actual
    }
    
    
    /***            FILL PRIORITY QUEUE		***/
    
    for(uint32_t j = 0; j < HISTLEN; j++)
    {
        if(histogram[j] != 0) //for all characters used in file
        {
            if((enqueue(q, newNode(j, true, histogram[j])) == false)) //enqueue a leaf node with the symbol and count
            {
                printf("[ERROR] Can't enqueue node with symbol value: %d and count: %ld.", j, histogram[j]);
            }
        }
    }
    
    
    
    
    /***		MERGE TREE TOGETHER		***/
    
    do
    {
        root = join((q -> Q[(q -> max)-2]), (q -> Q[(q -> max)-1])); //start forming tree using the two lowest values (max on queue)
        dequeue(q, &q->Q[(q -> max)]); // dequeue
        dequeue(q, &q->Q[(q -> max)]); // each of the minimum
        enqueue(q, root); // put the new interior node back in
    }
    while(q -> max > 1); //until there is one node left (the root)
    
    
    /***		BUILD CODE TABLE		***/
    
    code s = newCode(); //make new code for stack
    code codeTable[256];//code table for all ASCII
    for(uint32_t y = 0; y < 256; y++)
    {
        codeTable[y] = newCode(); //0 out the code table
    }
    buildCode(root, s, codeTable); //build code recursively assigns code
    
    
    
    /***		WRITE PRELIMS TO OFILE		***/
    int dump;
    uint32_t magic = 0xdeadd00d; //magic number
    if(toSTD == false)
    {
        dump = open(outName, O_WRONLY|O_CREAT , 0640); //what to write into
    }
    else
    {
        dump = 1; //stdout
    }
    if(dump == -1)
    {
        printf("[ERROR] Can't open or create file output file.");
    }
    (void) write(dump, &magic, 4); //write magic number 64bits
    (void) write(dump, &charCount, 8); //write file lenfth 32bits
    dumpTree(root, dump); //dump tree, writes tree length and tree to file
    
    
    
    
    /***		WRITE ENCODED FILE TO OFILE		***/
    
    int readLength; //length for reading file
    int maxCode = (leafCount/8)+8;
    bitV *v = newVec(charCount*maxCode); //make bit vector (testing bitvector length)
    
    int readFrom = open(inName, O_RDONLY); //open file
    do
    {
        readLength = read(readFrom, buffer, 512); //read in buffer load
        for(int i = 0; i < readLength; i++) //for every character
        {
            uint8_t symbol = buffer[i];
            for(uint32_t le = 0; le < codeTable[symbol].l; le++)         // append the code bits
            {						             // of that character
                
                if(((codeTable[symbol].bits[le/8] & (0x1 << le%8)) != 0))
                {
                    appendBit(v, 1);
                }
                else
                {
                    appendBit(v, 0);
                }
            }
        }
    }while(readLength == 512);
    (void) write(dump, v->v , (v->index/8)+1); //write the bit vector up to the index/8 for bytes
    
    
    
    
    /***		CLEAN IT UP		***/
    
    close(readFrom);
    close(dump);
    if(verbose)
    {
        long double num1 = v->index;
        long double num2 = charCount*8;;
        long double g = 100 * (num1/num2);//(charCount*8 / (v->index+1));
        printf("Original %ld bits: leaves %d (%d bytes) ", charCount*8, leafCount, (leafCount*3)-1);
        printf("encoding %ld bits (%.4Lf", v->index, g);
        char percent = '%';
        printf("%c).\n", percent);
    }
    delVec(v); //valgrind approved
    delQueue(q); //valgrind approved
    delTree(root); //valgrind approved
    return 0;
}

