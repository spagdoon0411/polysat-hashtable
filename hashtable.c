#include "hashtable.h"
#include <stdio.h>

/* MAKE SURE TO VALIDATE KEYS--NO EMPTY STRINGS */

/* Limit key strings to a certain length? Would guarantee speed
and avoid the integer overflow issue. */

/* TODO: make sure index 0 does not contribute to the table
size. */

/* TODO: use negatives for error codes? Or is it better
not to use negatives, as they create the risk of the user
accidentally indexing out of bounds? Does the user 
ever need to manage indices? -> probably not

It seems that in a context like PolySat, it may make
more sense to include user safeguards (like ensuring
that these dereferences always result in null pointer
exceptions) */

/* TODO: for hash2, see if there's a way to involve all
characters. Maybe alternate adding and subtracting each ASCII
value from a sum and finding the magnitude? (to replace
the key[0] term) */



/* Returns a hash table index based on the
key given. Used for initial hashing. Uses
Bob Jenkins's one-at-a-time hash, found here: 

en.wikipedia.org/wiki/Jenkins_hash_function

NOTE: the traditional implementation is modified
to ensure that no key hashes to 0, as 0
is used for special purposes--i.e., 
an indicator of a process failure. */
SIZEINT hash1(char* key, SIZEINT tablesize) {
    KEYLENINT length;
    KEYLENINT i = 0;
    uint32_t hash = 0;

    length = strlen(key);

    while (i != length) {
        hash += key[i++];
        hash += hash << 10;
        hash ^= hash >> 6;
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;

    hash = hash % tablesize;

    if(!hash)
        /* Change any hashes of 0 to 1 */
        hash++;

    return hash;
}

/* Returns a hash table index based on the
key given. Used for calculating probing step
sizes. Followed a (common) second hash function 
recommendation from these University of Washington 
slides: 

courses.cs.washington.edu/courses/cse326/00wi/handouts/lecture16/sld024.htm */
SIZEINT hash2(char* key, SIZEINT tablesize) {
    return (40009 - (strlen(key) * key[0]) % 40009);
}

/* Makes an empty dynamically-allocated hash table 
with a size of the next prime number above the 
requested size. Returns NULL if the requested size 
is too large and leads to an integer overflow OR
if the inputted value is less than two. */
HashTable* htcreate(SIZEINT reqsize) {
    HashTable *ht = NULL;
    SIZEINT truesize;

    truesize = nextprime(reqsize);

    if(!truesize || reqsize == 0)
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
    SIZEINT index, rehashstep, duplicateindex;

    /* Case in which the same key was entered again */
    if(duplicateindex = htcontains(ht, key)) {
        ht->values[duplicateindex] = value;
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
    
    /* hash2 is only called if necessary. A Boolean expression
    checking if hash2 needs to be run is more efficient 
    than running hash2 once unnecessarily. */
    if(ht->keys[index] != NULL) {
        rehashstep = hash2(key, ht->size);
        
        while(ht->keys[index] != NULL) {
            /* A free space in the table is guaranteed;
            the load factor was constrained to be less than CRITLF,
            which must be less than 1. */

            /* TODO: Is this if statement necessary? */
            if(index == 0)
                continue;

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
returns 0. */
SIZEINT htcontains(HashTable *ht, char* key) {
    SIZEINT attempt, rehashstep, initialattempt;
    
    attempt = hash1(key, ht->size);
    
    if(ht->keys[attempt] != NULL && !strcmp(ht->keys[attempt], key))
        return attempt;

    rehashstep = hash2(key, ht->size);
    initialattempt = attempt;
    while(1) {
        attempt = (attempt + rehashstep) % ht->size;

        if(ht->keys[attempt] != NULL && !strcmp(ht->keys[attempt], key))
            return attempt;

        if(attempt == initialattempt)
            return 0;
    }
}

/* Gets the value associated with a particular 
key from the hash table. Returns NULL if the
key could not be found.*/
void* htget(HashTable *ht, char* key) {
    SIZEINT index;

    index = htcontains(ht, key);
    if(!index)
        return NULL;

    return ht->values[index];
}

/* Removes the entry with the given key. Returns the pointer
stored in the table with that key on success (to allow for
quick deallocation by the user) and 0 otherwise. */
void* htremove(HashTable *ht, char* key) {
    SIZEINT index;
    void* ptr;

    index = htcontains(ht, key);
    if(index == 0)
        return 0;
    
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

    newsize = nextprime((ht->size)*GF + 1);
    if(!newsize) 
        /* If nextprime returned 0 (meaning that
        the prime number requested is too large
        for a a SIZEINT integer), return 0 to indicate 
        failure. */
        return 0;

    ht->keys = (char**)malloc(sizeof(char*) * newsize);
    ht->values = (void**)malloc(sizeof(void*) * newsize);

    if(ht->keys == NULL || ht->values == NULL) {
        /* If malloc could not find free space (meaning 
        it returned NULL) for either the keys or the 
        values, abort the creation of new key and value 
        arrays and return 0 to indicate failure.*/
        ht->keys = oldkeys;
        ht->values = oldvalues;
        return 0;
    }

    for(i = 1; i < ht->size; i++) {
        if(oldkeys[i] != NULL) {
            /* htinsert() will always succeed here; the new load factor
            is guaranteed to be less than CRITLF and there are certainly
            free spaces in the hash table. */
            htinsert(ht, oldkeys[i], oldvalues[i]);
        }
    }

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
        if(i == 0)
            printf("Reserved spot (index 0)\n");
        else if(ht->keys[i] != NULL)
            printf("%s: %d\n", ht->keys[i], *((int*)ht->values[i]));
        else 
            printf("Empty spot\n");
    }
}

/* Finds the next prime number above the 
given number. Returns 0 if the prime number 
requested leads to an overflow.

geeksforgeeks.org/program-to-find-the-next-prime-number/ */
SIZEINT nextprime(SIZEINT n) {

    if (n > MAXPRIME) 
        /* If n is greater than the largest prime number
        representable by a SIZEINT, return 0 to indicate
        an overflow issue. */
        return 0;
    
    while(1) {
        if(isprime(n))
            return n;
        n++;
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