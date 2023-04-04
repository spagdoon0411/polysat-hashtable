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

    free(ht1);
    printf("01 successful.\n");
}

/* Tests whether empty hash tables are 
created correctly when a composite number
is entered. */
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

    free(ht1);
    printf("02 successful.\n");
}

/* Tests whether NULL is returned
when the requested size for a hash
table results in an integer overflow */
void testhtcreate03() {
    printf("03 about to run...\n");

    HashTable* ht1 = htcreate(MAXPRIME + 1);
    assert(ht1 == NULL);
    free(ht1);
    ht1 = htcreate(MAXPRIME + 2);
    assert(ht1 == NULL);
    free(ht1);
    ht1 = htcreate(MAXPRIME + 3);
    assert(ht1 == NULL);
    free(ht1);
    ht1 = htcreate(MAXPRIME + 4);
    assert(ht1 == NULL);
    free(ht1);
    ht1 = htcreate(MAXPRIME + 5);
    assert(ht1 == NULL);
    free(ht1);
    ht1 = htcreate(0);
    assert(ht1 == NULL);
    free(ht1);
    ht1 = htcreate(1);
    assert(ht1 != NULL);
    free(ht1);
    
    printf("03 successful.\n");
}

/* Tests htinsert() and htcontains() in cases
where the hash table DOES NOT need to grow. */
void testhtinsertandcontains04() {
    printf("04 about to run...\n");

    HashTable* ht = htcreate(20);
    char* key1 = "eggs";
    int data1 = 1;
    char* key2 = "milk";
    int data2 = 2;
    char* key3 = "butter";
    int data3 = 3;
    char* key4 = "tomato soup";
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

    assert(htcontains(ht, key1));
    assert(htcontains(ht, key2));
    assert(htcontains(ht, key3));
    assert(htcontains(ht, key4));
    assert(htcontains(ht, duplicatekey1));
    
    assert(!(htcontains(ht, unusedkey)));
    
    /* Investigates value overwriting, which is
    expected when the same string is inputted 
    as a key again. */
    assert(htinsert(ht, duplicatekey2, &data3));
    assert(ht->values[htcontains(ht, duplicatekey2)] == &data3);
    assert(htinsert(ht, duplicatekey3, &data4));
    assert(htcontains(ht, duplicatekey2));
    assert(htcontains(ht, duplicatekey3));

    assert(ht->values[htcontains(ht, duplicatekey2)] == &data4);
    assert(ht->values[htcontains(ht, duplicatekey3)] == &data4);

    free(ht);
    printf("04 successful.\n");
}

/* Tests htinsert() and htcontains() in cases
where the hash table DOES need to grow. */
void testhtinsertandcontains05() {
    printf(" about to run...");
    printf(" successful.");
}

/* Tests how the htinsert() function handles hash
collisions. */
void testinsertcollisions() {

}

/* Tests removal of values from the hash table */
void testhtremove() {
    printf(" about to run...");
    printf(" successful.");
}

void testhashrange() {

}

void testhtdestroy() {
    printf(" about to run...");
    printf(" successful.");
}

void testhtgrow() {
    printf(" about to run...");
    printf(" successful.");
}

/* Tests cases in which integers may overflow 
related to finding a prime number that is too 
large for the integers used for the sizes
of the table */
void nextprimeoverflow() {

}

/* tests cases in which integers may overflow 
and how the program handles those cases when 
the situations that caused them relate to
the hash table growing too large */
void htgrowoverflow() {

}


int main() {
    testhtcreate01();
    testhtcreate02();
    testhtcreate03();
    testhtinsertandcontains04();
    return 0;
}