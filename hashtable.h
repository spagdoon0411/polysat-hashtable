#include <stdlib.h>
#include <stddef.h>
#include <string.h> 
#include <stdint.h>
#include <stdio.h>

/* The type of integer used for the table size. 
Should be signed. Determines how large the 
hash table can grow. */
#define SIZEINT int32_t 

/* Load factor threshold for table expansion, 
always less than 1. 0.75 is used here, as that
leads to an expected probe count of 4 in the 
worst-case search scenario. */
#define CRITLF 0.75

/* (Rough) factor by which to grow the table 
when necessary. */
#define GF 2

/* The largest prime number that can be held 
by a SIZEINT. Used within nextprime() to check for
integer overflow. */
#define MAXPRIME (SIZEINT)2147483647

/* The type of integer used to represent the
lengths of keys. Should allow for at least
MAXPRIME unique keys. */
#define KEYLENINT uint8_t

/* Integer size used in the Jenkins hash function.
32 by default. Must be unsigned. */
#define OAATJENINT uint32_t

typedef struct HashTable {
    SIZEINT entries; 
    SIZEINT size;
    char **keys; 
    void **values;
} HashTable;

SIZEINT hash1(char*, SIZEINT);
SIZEINT hash2(char*, SIZEINT);
HashTable *htcreate(SIZEINT);
uint8_t htinsert(HashTable*, char*, void*);
SIZEINT htcontains(HashTable*, char*);
void *htget(HashTable*, char*);
void *htremove(HashTable*, char*);
void htdestroy(HashTable*);
uint8_t htgrow(HashTable*);
void htprint(HashTable*);
SIZEINT nextprime(SIZEINT);
SIZEINT prevprime(SIZEINT);
uint8_t isprime(SIZEINT);

