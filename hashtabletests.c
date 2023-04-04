#include <assert.h>
#include <stdio.h>
#include "hashtable.h"

/* Tests whether empty hash tables are 
created correctly when a prime number
is entered. */
void testhtcreate01() {
    int i;

    printf("01 about to run...\n");

    HashTable* ht1 = htcreate(7);
    assert(ht1->size == 7);
    assert(ht1->entries == 0);
    for(i = 1; i < ht1->size; i++) {
        assert(ht1->keys[i] == 0);
    }
    assert(ht1->values[0] == NULL);

    htdestroy(ht1);
    printf("01 successful.\n");
}

/* Tests whether empty hash tables are 
created correctly in more general cases
(i.e., not perfect primes). */
void testhtcreate02() {
    int i;

    printf("02 about to run...\n");

    HashTable* ht1 = htcreate(4);
    assert(ht1->size == 5);
    assert(ht1->entries == 0);
    for(i = 1; i < ht1->size; i++) {
        assert(ht1->keys[i] == 0);
    }
    assert(ht1->values[0] == NULL);

    HashTable* ht2 = htcreate(-2);
    HashTable* ht3 = htcreate(0);
    assert(ht2 == NULL);
    assert(ht3 == NULL);

    htdestroy(ht1);
    printf("02 successful.\n");
}

/* Tests whether NULL is returned
when the requested size for a hash
table is 0 or greater than the 
maximum prime number able to be 
held by a SIZEINT. */
void testhtcreate03() {
    SIZEINT size = MAXPRIME + 1;
    HashTable *ht1;
    HashTable *ht2; 
    
    printf("03 about to run...\n");

    if(size > 0)
        assert(htcreate(size) == NULL);
    assert((ht1 = htcreate(MAXPRIME)) != NULL);
    assert((ht2 = htcreate(1)) != NULL);

    htdestroy(ht1);
    htdestroy(ht2);
    printf("03 successful.\n");
}

/* Tests htinsert() and htcontains() in cases
where the hash table DOES NOT need to grow. 

NOTE: this test case assumes a CRITLF
of 0.75 and a GF of 1.5. */
void testhtinsertandhtcontains04() {
    printf("04 about to run...\n");

    HashTable* ht = htcreate(20);
    char* key1 = "eggs";
    char* key2 = "milk";
    char* key3 = "butter";
    char* key4 = "tomato soup";
    int data1 = 1;
    int data2 = 2;
    int data3 = 3;
    int data4 = 4;

    char* unusedkey = "potatoes";
    char* duplicatekey1 = "eggs";

    char duplicatekey2[] = "bananas";
    char duplicatekey3[] = "bananas";

    assert(htinsert(ht, key1, &data1));
    assert(ht->entries == 1);
    assert(htinsert(ht, key2, &data2));
    assert(ht->entries == 2);
    assert(htinsert(ht, key3, &data3));
    assert(ht->entries == 3);
    assert(htinsert(ht, key4, &data4));
    assert(ht->entries == 4);

    assert(ht->keys[0] == 0);

    assert(htcontains(ht, key1) >= 0);
    assert(htcontains(ht, key2) >= 0);
    assert(htcontains(ht, key3) >= 0);
    assert(htcontains(ht, key4) >= 0);
    assert(htcontains(ht, duplicatekey1) >= 0);
    
    assert(htcontains(ht, unusedkey) < 0);
    
    /* Investigates value overwriting, which is
    expected when the same string is inputted 
    as a key again. */
    assert(htinsert(ht, duplicatekey2, &data3));
    assert(ht->values[htcontains(ht, duplicatekey2)] == &data3);
    assert(htinsert(ht, duplicatekey3, &data4));
    assert(htcontains(ht, duplicatekey2) >= 0);
    assert(htcontains(ht, duplicatekey3) >= 0);

    assert(ht->values[htcontains(ht, duplicatekey2)] == &data4);
    assert(ht->values[htcontains(ht, duplicatekey3)] == &data4);

    htdestroy(ht);
    printf("04 successful.\n");
}

/* Tests htinsert() and htcontains() in cases
where the hash table DOES need to grow. 

NOTE: this test case assumes a CRITLF
of 0.75 and a GF of 1.5. */
void testhtinsertandhtcontains05() {
    printf("05 about to run...\n");

    HashTable* ht = htcreate(4);
    char* key1 = "eggs";
    char* key2 = "milk";
    char* key3 = "butter";
    char* key4 = "tomato soup";
    char* key5 = "bananas";
    int data1 = 1;
    int data2 = 2;
    int data3 = 3;
    int data4 = 4;
    int data5 = 5;

    /* Load factor <= CRITLF for the below assertions */
    assert(htinsert(ht, key1, &data1));
    assert(htinsert(ht, key2, &data2));
    assert(htinsert(ht, key3, &data3));

    assert(ht->entries == 3);
    assert(ht->size == 5);

    SIZEINT oldsize = ht->size;

    /* Load factor now exceeds CRITLF */
    assert(htinsert(ht, key4, &data4));
    assert(htinsert(ht, key5, &data5));

    /* Verify that the entries and size are 
    are the expected values */
    assert(ht->entries == 5);
    assert(ht->size == nextprime((SIZEINT)(oldsize * GF + 1)));

    /* Verify that all previous and new data was 
    transferred correctly, with all old keys mapping 
    to the same values that they did before htgrow(). */
    assert(htcontains(ht, key1) >= 0);
    assert(htcontains(ht, key2) >= 0);
    assert(htcontains(ht, key3) >= 0);
    assert(htcontains(ht, key4) >= 0);
    assert(htcontains(ht, key5) >= 0);
    assert(ht->values[htcontains(ht, key1)] == &data1);
    assert(ht->values[htcontains(ht, key2)] == &data2);
    assert(ht->values[htcontains(ht, key3)] == &data3);
    assert(ht->values[htcontains(ht, key4)] == &data4);
    assert(ht->values[htcontains(ht, key5)] == &data5);

    /* Repeat the process above, causing more 
    scaling and checking if the hash table has
    not lost data at the end of the process--
    and that keys and values are still paired 
    correctly. */
    char* key6 = "eggs2";
    char* key7 = "milk2";
    char* key8 = "butter2";
    char* key9 = "tomato soup2";
    char* key10 = "bananas2";
    int data6 = 6;
    int data7 = 7;
    int data8 = 8;
    int data9 = 9;
    int data10 = 10;

    assert(htinsert(ht, key6, &data6));
    assert(htinsert(ht, key7, &data7));
    assert(htinsert(ht, key8, &data8));
    
    oldsize = ht->size;
    assert(htinsert(ht, key9, &data9));
    assert(ht->entries == 9);
    assert(ht->size == nextprime((SIZEINT)(oldsize * GF + 1)));

    assert(htcontains(ht, key10) < 0);
    assert(htinsert(ht, key10, &data10));
    assert(htcontains(ht, key10) >= 0);

    assert(htcontains(ht, key1) >= 0);
    assert(htcontains(ht, key2) >= 0);
    assert(htcontains(ht, key3) >= 0);
    assert(htcontains(ht, key4) >= 0);
    assert(htcontains(ht, key5) >= 0);
    assert(htcontains(ht, key6) >= 0);
    assert(htcontains(ht, key7) >= 0);
    assert(htcontains(ht, key8) >= 0);
    assert(htcontains(ht, key9) >= 0);
    assert(htcontains(ht, key10) >= 0);
    assert(ht->values[htcontains(ht, key1)] == &data1);
    assert(ht->values[htcontains(ht, key2)] == &data2);
    assert(ht->values[htcontains(ht, key3)] == &data3);
    assert(ht->values[htcontains(ht, key4)] == &data4);
    assert(ht->values[htcontains(ht, key5)] == &data5);
    assert(ht->values[htcontains(ht, key6)] == &data6);
    assert(ht->values[htcontains(ht, key7)] == &data7);
    assert(ht->values[htcontains(ht, key8)] == &data8);
    assert(ht->values[htcontains(ht, key9)] == &data9);
    assert(ht->values[htcontains(ht, key10)] == &data10);

    htdestroy(ht);
    printf("05 successful.\n");
}

/* Tests removal of values from the hash table */
void testhtremove06() {
    printf("06 about to run...\n");

    HashTable *ht = htcreate(8);

    char* key1 = "eggs";
    char* key2 = "milk";
    char* key3 = "butter";
    char* key4 = "tomato soup";
    char* key5 = "bananas";
    int data1 = 1;
    int data2 = 2;
    int data3 = 3;
    int data4 = 4;
    int data5 = 5;

    char* fkey1 = "banana";
    char* fkey2 = "monkey";
    char* fkey3 = "soup";

    htinsert(ht, key1, &data1);
    htinsert(ht, key2, &data2);
    htinsert(ht, key3, &data3);
    htinsert(ht, key4, &data4);
    htinsert(ht, key5, &data5);

    SIZEINT keyidx = htcontains(ht, key1); 
    assert(htremove(ht, key1) == &data1);
    assert(ht->keys[keyidx] == 0);

    keyidx = htcontains(ht, key2); 
    assert(htremove(ht, key2) == &data2);
    assert(ht->keys[keyidx] == 0);

    assert(htremove(ht, fkey1) == NULL);

    keyidx = htcontains(ht, key3); 
    assert(htremove(ht, key3) == &data3);
    assert(ht->keys[keyidx] == 0);

    assert(htremove(ht, fkey3) == NULL);

    assert(htcontains(ht, key4) >= 0);
    assert(htcontains(ht, key5) >= 0);

    keyidx = htcontains(ht, key4); 
    assert(htremove(ht, key4) == &data4);
    assert(ht->keys[keyidx] == 0);

    keyidx = htcontains(ht, key5); 
    assert(htremove(ht, key5) == &data5);
    assert(ht->keys[keyidx] == 0);

    assert(htcontains(ht, key1) < 0);
    assert(htcontains(ht, key2) < 0);
    assert(htcontains(ht, key3) < 0);
    assert(htcontains(ht, key4) < 0);
    assert(htcontains(ht, key5) < 0);

    htdestroy(ht);
    printf("06 successful.\n");
}

void testhtget07() {
    printf("07 about to run...\n");

    HashTable *ht = htcreate(8);

    char* key1 = "eggs";
    char* key2 = "milk";
    char* key3 = "butter";
    char* key4 = "tomato soup";
    char* key5 = "bananas";
    int data1 = 1;
    int data2 = 2;
    int data3 = 3;
    int data4 = 4;
    int data5 = 5;

    htinsert(ht, key1, &data1);
    htinsert(ht, key2, &data2);
    htinsert(ht, key3, &data3);
    htinsert(ht, key4, &data4);

    assert(htget(ht, key1) == &data1);
    assert(htget(ht, key2) == &data2);
    assert(htget(ht, key3) == &data3);
    assert(htget(ht, key4) == &data4);
    assert(htget(ht, key5) == NULL);

    htdestroy(ht);

    printf("07 about to run...\n");
}

int main() {
    testhtcreate01();
    testhtcreate02();
    testhtcreate03();
    testhtinsertandhtcontains04();
    testhtinsertandhtcontains05();
    testhtremove06();
    testhtget07();
    return 0;
}