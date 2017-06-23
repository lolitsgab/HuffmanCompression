# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <errno.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include "queue.h"
# include "huffman.h"
# include "code.h"

# define SIZE 512
# define HISTLEN 256

int histogram (void)
{
        uint32_t histogram[HISTLEN];//make histogram of 256
	uint8_t buffer[SIZE]; //buffer for reading
        int length; //length for reading
        
	for(int h = 0; h < 256; h++)
        {
                histogram[h] = 0; //set it all to 0
        }
	
	histogram[0] = 1;
	histogram[255] = 1;
        
        int doc = open("testing.txt", O_RDONLY); //open file
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
                        printf("%d ", histogram[j]);
                }
        }
	return 0;
}
