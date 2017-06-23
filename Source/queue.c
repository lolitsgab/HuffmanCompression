//queue.c code provided by DDEL accesed 5/23/17
# include <stdint.h>
# include <stdbool.h>
# include <stdlib.h>
# include <stdio.h>
# include "queue.h"
# include "huffman.h"

queue *newQueue(uint32_t size) // Constructor
{
	queue *q = (queue *)calloc(1, sizeof(queue));
	q -> size = size;
	q -> max = 0;
	q -> Q = (item *)calloc(size, sizeof(item));
	return q;
}

void   delQueue(queue *q)      // Destructor
{
	free(q -> Q);
	free(q);
	return;
}

bool emptyq(queue *q)           // Is it empty?
{
	return (q -> max) == 0;
}

bool full (queue *q)           // Is it full?
{
	return (q -> max) == q -> size;
}

bool enqueue(queue *q, item  i) // Add an item
{
	if(full(q)) //can't queue if full
	{
		return false;
	}
	else
	{
		if(q -> max == 0) //0 case just sets 0
		{
			q -> Q[0] = i;
		}
                for(uint32_t a = (q -> max); a > 0; a--) //start at current value and decrement
		{
                                if((i -> count > q -> Q[a-1] -> count)) //if the count of the new input is greater than 
                                {       
                                        q -> Q[a] = q -> Q[a-1]; //move number forward to give it space
					if((a-1) == 0 )
					{
						q -> Q[0] = i; //if we move forward the Q[0]. set the Q[0] to i and break
						break;
					}
                                }
				else 
				{
					q -> Q[a] = i; //if it finds a spot where it is between two numbers, set it
					break;
				}
                }
		q -> max += 1; //increase the max number
	}
        return true;
}	

bool dequeue(queue *q, item *i) //Remove from the rear
{
	if(emptyq(q)) //can't dequeue if empty
	{
		return false;
	}
	else
	{
		q -> max -= 1; //dequeue max number
		*i = q -> Q[q -> max]; //the max becomes one less than the previous
		return true;
	}
}

