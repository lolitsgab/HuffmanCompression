// Code from DDEL 4/20/17 (valBit, setBit, clrBit)
#include <stdlib.h>
#include <stdio.h>
#include "bv.h"

bitV *newVec(uint64_t k) //Create new vector of specified length
{
	bitV *t;
	t = (bitV *)calloc(1, sizeof(bitV));
	t -> l = k;
	t -> index = 0;
	t -> v = (uint8_t *)calloc(k/8+1, sizeof(uint8_t));
	return t;
}

uint8_t valBit(bitV *a, uint32_t k) //Return the value of a specified bit
{	
	return((a->v)[k >> 3] & (0x1 << (k & 0x7))) >> (k & 0x7);
}

void delVec(bitV *e) // Deletes a vector
{
	free(e -> v);
	free(e);
	return;
}

void oneVec(bitV *e) //Creates a vector of all 1
{
	uint32_t t = 0;
	while(t < (e->l))
	{
		(e->v)[t] = ~(0x0);
		t++;
	}	
	return ;	
}

void setBit(bitV *e, uint32_t k) //Sets a speified bit
{	
        (e->v)[k >> 3] |=  (0x1 << (k & 0x7));
	return ;
}

void clrBit(bitV *e, uint32_t k) //Clears a specified bit
{
        (e->v)[k >> 3] &= ~(0x1 << (k & 0x7));
	return;
}

uint32_t lenVec(bitV *e) //Return the length if the vector
{
	uint32_t t;
	t = (e->l);
	return t;
}

void appendBit(bitV *e, uint32_t k)
{
	if(e -> index < e -> l) //check if index is reached
	{
		if(k == 0) //if 0, set bit to 0
		{
			clrBit(e, e -> index);
		}
		else //if 1, set bit to 1
		{
			setBit(e, e -> index);
		}
	}
	else //if index is reaced, full BV = error
	{
		return;
	}
	e -> index += 1; //increase the index
	return;
}
