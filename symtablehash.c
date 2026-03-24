#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "symtable.h"

int bucketcount = 509;

struct Binding {
    const void* value;
    char* key;
    struct Binding* next;
};


struct SymTable_T {
    struct Binding* Buckets[bucketcount];
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
    struct SymTable_T* oSymTable = calloc(1, sizeof(struct SymTable_T));
    if (oSymTable == NULL) return NULL;
    oSymTable->length = 0;
    return oSymTable;
}

void SymTable_free(SymTable_T oSymTable) {

    assert(oSymTable != NULL);

    for (int i = 0; i < bucketcount; i++) {
        
        struct Binding* temp = oSymTable->Buckets[i];

        while (temp != NULL) {
            struct Binding* temp2 = temp->next;
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

    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    
    size_t hash = SymTable_hash(pcKey, bucketcount);

    struct Binding* next = oSymTable->Buckets[hash];

    while (next != NULL) {
        if (strcmp(next->key, pcKey) == 0) {
            return 0;
        }

        next = next->next;
    }

    struct Binding* new = malloc(sizeof(struct Binding));
    if( new == NULL) return 0;

    char* tempkey = malloc(strlen(pcKey) + 1);
    if (tempkey == NULL) {
        free (new);
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
    
    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    size_t hash = SymTable_hash(pcKey, bucketcount);

    struct Binding* next = oSymTable->Buckets[hash];

    while (next != NULL) {
        if (strcmp(next->key, pcKey) == 0) {

            void* temp = (void*) next->value;
            next->value = pvValue;

            return temp;
        }

        next = next->next;
    }

    return NULL;
}

int SymTable_contains(SymTable_T oSymTable, const char *pcKey) {

    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    
    size_t hash = SymTable_hash(pcKey, bucketcount);

    struct Binding* next = oSymTable->Buckets[hash];

    while (next != NULL) {
        if (strcmp(next->key, pcKey) == 0) {
            return 1;
        }

        next = next->next;
    }

    return 0;
}

void *SymTable_get(SymTable_T oSymTable, const char *pcKey) {
        
    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    
    size_t hash = SymTable_hash(pcKey, bucketcount);

    struct Binding* next = oSymTable->Buckets[hash];

    while (next != NULL) {
        if (strcmp(next->key, pcKey) == 0) {
            return (void*) next->value;
        }

        next = next->next;
    }

    return NULL;
}

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey) {
    
    struct Binding* next;
    size_t hash = SymTable_hash(pcKey, bucketcount);


    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    next = oSymTable->Buckets[hash];

    if (next == NULL) {
        return NULL;
    }

    if(strcmp(next->key, pcKey) == 0) {
        if (next->next != NULL) {
            oSymTable->Buckets[hash] = next->next;
        }
        else oSymTable->Buckets[hash] = NULL;

        void* retval = (void*) next->value;

        free(next->key);
        free(next);
        
        oSymTable->length--;
        return retval;
    }

    while (next->next != NULL) {
        if (strcmp(next->next->key, pcKey) == 0) {
            struct Binding* temp = next->next;
            void* retval = (void*) temp->value;
            next->next = temp->next;
            free(temp->key);
            free(temp);

            oSymTable->length--;
            return retval;
        }
        next = next->next;
    }

    return NULL;
}


void SymTable_map(SymTable_T oSymTable,
    void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
    const void *pvExtra){

    assert(oSymTable != NULL);
    assert(pfApply != NULL);
    
    for (int i = 0; i < bucketcount; i++){
        struct Binding* next = oSymTable->Buckets[i];

        while(next != NULL) {
            pfApply(next->key, (void*) next->value, (void*) pvExtra);

            next = next->next;
        }
    }
}











