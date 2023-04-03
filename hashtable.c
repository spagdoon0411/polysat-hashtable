#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "hashtable.h"
#define CRITLF 0.75 /* Load factor threshold for table expansion */
#define GF 1.5 /* Factor to grow the table by */

/* NOTE: MUST CHOOSE HASH FUNCTIONS THAT ARE 
POSITIVE, INCLUDING NON-ZERO */

/* Returns an integer hash code based on the
key given. Used for initial hashing. */
int hash1(char* key) {
    return 0;
}

/* Returns an integer hash code based on the
key given. Used for calculating probing step
size. */
int hash2(char* key) {
    return 0;
}

/* Makes an empty dynamically-allocated hash table 
with a size of the next prime number above the 
requested size. */
HashTable* htcreate(int reqsize) {
    HashTable *ht = NULL;
    ht->size = 0;
    ht->entries = 0;
    ht->keys = NULL;
    ht->values = NULL;

    ht = (HashTable*)malloc(sizeof(HashTable));
    ht->size = nextprime(reqsize);
    ht->keys = (char**)calloc(ht->size, sizeof(char*));
    ht->values = (void**)malloc(sizeof(void*) * ht->size);

    return ht;
}


/* Expands the hash table by a factor defined by GF */
void htgrow(HashTable *ht) {
    /* MAKE SURE TO CHANGE THE KEYS AND VALUES POINTERS */
}

/* Seaches the table for an entry with the given 
key. Returns the index of the key if found or */
int htcontains(HashTable *ht, char* key) {
    
}

/* Removes the entry with the given key. */
int htremove(HashTable *ht, char* key) {

}

/* Places a value into the hash table. */
void htinsert(HashTable *ht, char* key, void* value) {
    int index, rehashstep;

    if((double)(ht->entries + 1) / ht->size > CRITLF) {
        htgrow(ht);
    }
    
    index = hash1(key);
    
    /* hash2 is only called if necessary. A Boolean expression
    checking if hash2 needs to be run is more efficient 
    than running hash2 once unnecessarily. */
    if(ht->keys[index] != NULL) {
        rehashstep = hash2(key);
        while(ht->keys[index] != NULL)
            index = index + rehashstep;
    }

    ht->keys[index] = key;
    ht->values[index] = value;
    ht->entries++;

    return ht;
}

/* Deallocates all heap memory associated with the hash table */
void htdestroy(HashTable* ht) {

}

/* Finds the next prime number above the 
given number.

geeksforgeeks.org/program-to-find-the-next-prime-number/ */
int nextprime(long n) {
    while(1) {
        if(isprime(n))
            return n;
        n++;
    }
}

/* Checks if the given integer is prime.

geeksforgeeks.org/program-to-find-the-next-prime-number/ */
int isprime(int n) {
    int i;

    if (n <= 1)
        return 0;
    if (n <= 3)
        return 1;

    if (n % 2 == 0 || n % 3 == 0)
        return 0;
    
    for(i = 5; i*i<=n; i=i+6)
        if(n % i == 0 || n % (i+2) == 0)
            return 0;
    
    return 1;
}