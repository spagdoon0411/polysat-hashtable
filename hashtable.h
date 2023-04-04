#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>  

/* The type of integer used for the table size. 
Should be unsigned. Determines how large the 
hash table can grow. */
#define SIZEINT uint32_t 

/* The prime number just below or equal to the 
maximum value that can be held by a SIZEINT 
integer. In other words, the largest prime
number that can be held by a SIZEINT. */
#define MAXPRIME (SIZEINT)4294967291 

/* Load factor threshold for table expansion, 
always less than 1. 0.75 is used here, as that
leads to an expected probe count of 4 in the 
worst-case search scenario. */
#define CRITLF 0.75 

/* (Rough) factor by which to grow the table 
when necessary. */
#define GF 1.5 

/* The type of integer used to represent the
lengths of keys. Should allow for at least
MAXPRIME unique keys. */
#define KEYLENINT uint8_t

typedef struct HashTable {
    SIZEINT entries; 
    SIZEINT size;
    char** keys; 
    void** values;
} HashTable;

SIZEINT hash1(char* key, SIZEINT tablesize);
SIZEINT hash2(char* key);
HashTable* htcreate(SIZEINT reqsize);
uint8_t htinsert(HashTable *ht, char* key, void* value);
SIZEINT htcontains(HashTable *ht, char* key);
void* htremove(HashTable *ht, char* key);
void htdestroy(HashTable* ht);
uint8_t htgrow(HashTable *ht);
void htprint(HashTable *ht);
SIZEINT nextprime(SIZEINT n);
uint8_t isprime(SIZEINT n);

