typedef struct HashTable {
    int entries; 
    int size;
    char** keys; 
    void** values;
} HashTable;