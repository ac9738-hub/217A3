#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "symtable.h"

#define BUCKET_COUNT 509

struct Binding {
    const void* value;
    char* key;
    struct Binding* next;
};

struct SymTable {
    struct Binding* Buckets[BUCKET_COUNT];  /* Now uses constant */
    size_t length;
};


static size_t SymTable_hash(const char *pcKey, size_t uBucketCount){
    
    const size_t HASH_MULTIPLIER = 65599;
    size_t u;
    size_t uHash = 0;

    assert(pcKey != NULL);

    for (u = 0; pcKey[u] != '\0'; u++)
        uHash = uHash * HASH_MULTIPLIER + (size_t)pcKey[u];

    return uHash % uBucketCount;
}

SymTable_T SymTable_new(void) {
    struct SymTable* oSymTable = calloc(1, sizeof(struct SymTable));
    if (oSymTable == NULL) return NULL;
    oSymTable->length = 0;
    return oSymTable;
}

void SymTable_free(SymTable_T oSymTable) {
    int i;
    struct Binding* temp;
    struct Binding* temp2;

    assert(oSymTable != NULL);

    for (i = 0; i < BUCKET_COUNT; i++) {
        
        temp = oSymTable->Buckets[i];

        while (temp != NULL) {
            temp2 = temp->next;
            free(temp->key);
            free(temp);
            temp = temp2;
        }
    }

    free(oSymTable);
}

size_t SymTable_getLength(SymTable_T oSymTable) {
    
    assert(oSymTable != NULL);
    return oSymTable->length;
}

int SymTable_put(SymTable_T oSymTable,
    const char *pcKey, const void *pvValue){

    size_t hash;
    struct Binding* next;
    struct Binding* new;
    char* tempkey;

    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    
    hash = SymTable_hash(pcKey, BUCKET_COUNT);

    next = oSymTable->Buckets[hash];

    while (next != NULL) {
        if (strcmp(next->key, pcKey) == 0) {
            return 0;
        }

        next = next->next;
    }

    new = malloc(sizeof(struct Binding));
    if (new == NULL) return 0;

    tempkey = malloc(strlen(pcKey) + 1);
    if (tempkey == NULL) {
        free(new);
        return 0;
    }
    
    strcpy(tempkey, pcKey);

    new->key = tempkey;
    new->value = pvValue;

    new->next = oSymTable->Buckets[hash];
    oSymTable->Buckets[hash] = new;

    oSymTable->length++;
    return 1;

}

void *SymTable_replace(SymTable_T oSymTable,
    const char *pcKey, const void *pvValue) {
    
    size_t hash;
    struct Binding* next;
    void* temp;

    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    hash = SymTable_hash(pcKey, BUCKET_COUNT);

    next = oSymTable->Buckets[hash];

    while (next != NULL) {
        if (strcmp(next->key, pcKey) == 0) {

            temp = (void*) next->value;
            next->value = pvValue;

            return temp;
        }

        next = next->next;
    }

    return NULL;
}

int SymTable_contains(SymTable_T oSymTable, const char *pcKey) {

    size_t hash;
    struct Binding* next;

    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    
    hash = SymTable_hash(pcKey, BUCKET_COUNT);

    next = oSymTable->Buckets[hash];

    while (next != NULL) {
        if (strcmp(next->key, pcKey) == 0) {
            return 1;
        }

        next = next->next;
    }

    return 0;
}

void *SymTable_get(SymTable_T oSymTable, const char *pcKey) {
    
    size_t hash;
    struct Binding* next;

    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    
    hash = SymTable_hash(pcKey, BUCKET_COUNT);

    next = oSymTable->Buckets[hash];

    while (next != NULL) {
        if (strcmp(next->key, pcKey) == 0) {
            return (void*) next->value;
        }

        next = ne