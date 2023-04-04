#include "hashtable.h"
#include <stdio.h>

/* The type of integer used to represent the
lengths of keys. Should allow for at least
MAXPRIME unique keys. */
#define KEYLENINT uint8_t

/* Integer size used in the Jenkins hash function.
32 by default. Must be unsigned. */
#define OAATJENINT uint32_t

/* MAKE SURE TO VALIDATE KEYS--NO EMPTY STRINGS */

/* TODO: for hash2, see if there's a way to involve all
characters. Maybe alternate adding and subtracting each ASCII
value from a sum and finding the magnitude (to replace
the key[0] term)? */

/* Returns a hash table index based on the
key given. Used for initial hashing. Uses
Bob Jenkins's one-at-a-time hash, found here: 

en.wikipedia.org/wiki/Jenkins_hash_function */
SIZEINT hash1(char* key, SIZEINT tablesize) {
    KEYLENINT keylen;
    KEYLENINT i = 0;
    OAATJENINT hash = 0;

    keylen = strlen(key);

    while (i != keylen) {
        hash += key[i++];
        hash += hash << 10;
        hash ^= hash >> 6;
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;

    hash = hash % tablesize;

    return hash;
}

/* Second hash function. Modified from
slide 24 from the source below:

courses.cs.washington.edu/courses/cse326/00wi/handouts/lecture16/sld024.htm */
SIZEINT hash2(char* key, SIZEINT tablesize) {
    KEYLENINT keylen = strlen(key);
    KEYLENINT i;
    uint8_t ascii_term = 0;
    SIZEINT prime = prevprime(tablesize);

    for(i = 0; i < keylen; i++) {
        if(i % 2)
            ascii_term += (uint8_t)(key[i]);
        else
            ascii_term -= (uint8_t)(key[i]);
    }

    return (prime - (keylen * ascii_term) % prime);
}


/* Makes an empty dynamically-allocated hash table 
with a size of the next prime number above the 
requested size. Returns NULL if the requested size 
is too large OR if the inputted size is 0. */
HashTable* htcreate(SIZEINT reqsize) {
    HashTable *ht = NULL;
    SIZEINT truesize;

    truesize = nextprime(reqsize);

    if(truesize < 0 || reqsize == 0)
        /* If nextprime returned 0 (meaning that
        the prime number requested is too large
        for a SIZEINT integer), return NULL to 
        indicate failure. */
        return NULL;

    ht = (HashTable*)malloc(sizeof(HashTable));
    ht->size = truesize;
    ht->keys = (char**)calloc(truesize, sizeof(char*));
    ht->values = (void**)malloc(sizeof(void*) * truesize);
    ht->entries = 0;
    ht->values[0] = NULL;

    return ht;
}

/* Places a value into the hash table. Returns 1 on success 
and 0 otherwise. */
uint8_t htinsert(HashTable *ht, char* key, void* value) {
    SIZEINT index, rehashstep;

    index = htcontains(ht, key);
    /* Case in which the same key was entered again; 
    simply overwrites the value belonging to the key
    with the new value. */
    if(index > 0) {
        ht->values[index] = value;
        return 1;
    }

    /* Grows the hash table by a factor of GF if the load
    factor is greater than CRITLF  */
    if(((float)(ht->entries + 1) / ht->size) > CRITLF)
        if(!htgrow(ht))
            /* Return 0 to indicate failure if 
            htgrow() fails. */
            return 0;


    index = hash1(key, ht->size);
    /* This wrapping if condition prevents running hash2 unnecessarily */
    if(ht->keys[index] != NULL) {
        rehashstep = hash2(key, ht->size);

        while(ht->keys[index] != NULL) {
            /* A free space in the table is guaranteed;
            the load factor was constrained to be less than CRITLF,
            which must be less than 1. */
            index = (index + rehashstep) % ht->size;
        }
    }

    ht->keys[index] = key;
    ht->values[index] = value;
    ht->entries++;

    return 1;
}

/* Searches the table for an entry with the given 
key. Returns the index of the key if found or 
returns -1. */
SIZEINT htcontains(HashTable *ht, char* key) {
    SIZEINT attempt, rehashstep, initialattempt;
    
    attempt = hash1(key, ht->size);

    /* If the key is at the first index attempt, immediately return that index.
    The Boolean condition is prepended by a null check */
    if(ht->keys[attempt] != NULL && !strcmp(ht->keys[attempt], key))
        return attempt;

    /* Record the initial attempt and probe until the key is
    found or the initial attempt is reencountered */
    initialattempt = attempt;
    rehashstep = hash2(key, ht->size);
    while(1) {
        attempt = (attempt + rehashstep) % ht->size;

        /* Case in which the key is found (with the Boolean
        condition prepended by a null check). */
        if(ht->keys[attempt] != NULL && !strcmp(ht->keys[attempt], key))
            return attempt;
        
        /* Case in which every hash table value was visited and 
        probing returned to the initial-attempt index */
        if(attempt == initialattempt)
            return -1;
    }
}

/* Gets the value associated with a particular 
key from the hash table. Returns NULL if the
key could not be found.*/
void* htget(HashTable *ht, char* key) {
    SIZEINT index;

    /* If the hash table does not contain the inputted 
    key, return NULL. */
    if((index = htcontains(ht, key)) < 0)
        return NULL;

    return ht->values[index];
}

/* Removes the entry with the given key. Returns the pointer
stored in the table with that key on success (to allow for
quick deallocation by the user) and NULL otherwise. */
void* htremove(HashTable *ht, char* key) {
    SIZEINT index;
    void* ptr;

    if((index = htcontains(ht, key)) < 0)
        return NULL;
    
    ptr = ht->values[index];
    ht->keys[index] = 0;
    /* Note: clearing the value that corresponds to the key
    is a waste of time and resources. */
    ht->entries--;

    return ptr;
}

/* Deallocates all dynamically-allocated memory associated with the 
hash table that is not the user's responsiblity. */
void htdestroy(HashTable* ht) {
    free(ht->keys);
    free(ht->values);
    free(ht);
}

/* Expands the hash table by a factor defined by GF. Returns 
1 on success; returns 0 otherwise. */
uint8_t htgrow(HashTable *ht) {
    SIZEINT i, newsize;
    char **oldkeys;
    void **oldvalues;

    oldkeys = ht->keys;
    oldvalues = ht->values;

    newsize = nextprime((SIZEINT)((ht->size) * GF + 1));
    /* If nextprime returned -1 (meaning that
    the prime number requested is too large
    for a a SIZEINT integer), return 0 to indicate 
    failure. */
    if(newsize < 0) 
        return 0;

    ht->keys = (char**)calloc(newsize, sizeof(char*));
    ht->values = (void**)malloc(sizeof(void*) * newsize);

    /* If malloc could not find free space (meaning 
    it returned NULL) for either the keys or the 
    values, abort the creation of new key and value 
    arrays and return 0 to indicate failure. */
    if(ht->keys == NULL || ht->values == NULL) {
        ht->keys = oldkeys;
        ht->values = oldvalues;
        return 0;
    }

    ht->entries = 0;

    for(i = 0; i < ht->size; i++) {
        if(oldkeys[i] != NULL) {
            /* htinsert() will always succeed here; the new load factor
            is guaranteed to be less than CRITLF and there are certainly
            free spaces in the hash table. */
            htinsert(ht, oldkeys[i], oldvalues[i]);
        }
    }

    ht->size = newsize;

    free(oldkeys);
    free(oldvalues);

    return 1;
}

/* A method used primarily for development, 
testing, etc. that prints a string representation
of the hash table to the terminal. */
void htprint(HashTable *ht) {
    printf("Entries: %d\n", ht->entries);
    printf("Size: %d\n", ht->size);
    printf("Load factor: %f\n", (float)(ht->entries) / ht->size);

    for(int i = 0; i < ht->size; i++) {
        if(ht->keys[i] != NULL)
            printf("%s: %d\n", ht->keys[i], *((int*)ht->values[i]));
        else 
            printf("Empty spot\n");
    }
}

/* Finds the next prime number above the 
given number. Returns -1 if the prime number 
requested leads to an overflow.

geeksforgeeks.org/program-to-find-the-next-prime-number/ */
SIZEINT nextprime(SIZEINT n) {

    if (n > MAXPRIME) 
        /* If n is greater than the largest prime number
        representable by a SIZEINT, return 0 to indicate
        an overflow issue. */
        return -1;
    
    while(1) {
        if(isprime(n))
            return n;
        n++;
    }
}

/* Finds the previous prime number below the 
given number. Returns -1 if there is no 
previous prime number.

geeksforgeeks.org/program-to-find-the-next-prime-number/ */
SIZEINT prevprime(SIZEINT n) {

    if (n < 2) 
        /* If n is greater than the largest prime number
        representable by a SIZEINT, return 0 to indicate
        an overflow issue. */
        return -1;
    
    while(1) {
        n--;
        if(isprime(n))
            return n;        
    }
}

/* Returns 1 if the given integer is prime and 0 otherwise. 

geeksforgeeks.org/program-to-find-the-next-prime-number/ */
uint8_t isprime(SIZEINT n) {
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