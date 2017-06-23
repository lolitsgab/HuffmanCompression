// bv.h — Bit Vector interface
// Author: Prof. Darrell Long

# ifndef _BVector
# define _BVector
# include <stdint.h>

typedef struct bitV {
	uint8_t *v;
	uint64_t l;
	uint64_t index;
} bitV;

bitV *newVec(uint64_t); //Create new vector of specified length

void delVec(bitV *); // Deletes a vector

void oneVec(bitV *); //Creates a vector of all 1

void setBit(bitV *, uint32_t); //Sets a speified bit

void clrBit(bitV *, uint32_t); //Clears a specified bit

uint8_t valBit(bitV *, uint32_t); //Return the value of a specified bit

uint32_t lenVec(bitV *); //Returns the length of a specified bit

void appendBit(bitV *e, uint32_t k); //Append a bit to a bitvector

# endif
