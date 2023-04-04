#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>  

/* The type of integer used for the table size. 
Should be signed. Determines how large the 
hash table can grow. */
#define SIZEINT int32_t 

/* The largest prime number that can be held 
by a SIZEINT. Used within nextprime() to check for
integer overflow. */
#define MAXPRIME (SIZEINT)2147483647 

/* Load factor threshold for table expansion, 
always less than 1. 0.75 is used here, as that
leads to an expected probe count of 4 in the 
worst-case search scenario. */
#define CRITLF 0.75

/* (Rough) factor by which to grow the table 
when necessary. */
#define GF 2

typedef struct HashTable {
    SIZEINT entries; 
    SIZEINT size;
    char** keys; 
    void** values;
} HashTable;

SIZEINT hash1(char* key, SIZEINT tablesize);
SIZEINT hash2(char* key, SIZEINT tablesize);
HashTable* htcreate(SIZEINT reqsize);
uint8_t htinsert(HashTable *ht, char* key, void* value);
SIZEINT htcontains(HashTable *ht, char* key);
void* htremove(HashTable *ht, char* key);
void htdestroy(HashTable* ht);
uint8_t htgrow(HashTable *ht);
void htprint(HashTable *ht);
SIZEINT nextprime(SIZEINT n);
SIZEINT prevprime(SIZEINT n);
uint8_t isprime(SIZEINT n);

