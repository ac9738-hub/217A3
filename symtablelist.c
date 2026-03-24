#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "symtable.h"
 
/* A single binding: holds a defensive copy of the key, a value,
   and a pointer to the next binding in the list. */
struct Binding {
    char *key;
    const void *value;
    struct Binding *next;
};
 
/* A SymTable holds a pointer to the first binding and
   a count of how many bindings it contains. */
struct SymTable_T {
    size_t length;
    struct Binding *first;
};
 
/*------------------------------------------------------------*/
 
SymTable_T SymTable_new(void) {
    struct SymTable_T *oSymTable = malloc(sizeof(struct SymTable_T));
    if (oSymTable == NULL) return NULL;
    oSymTable->length = 0;
    oSymTable->first = NULL;
    return oSymTable;
}
 
/*------------------------------------------------------------*/
 
void SymTable_free(SymTable_T oSymTable) {
    struct Binding *temp;
    struct Binding *nextb;
 
    assert(oSymTable != NULL);
 
    temp = oSymTable->first;
 
    while (temp != NULL) {
        nextb = temp->next;
        free(temp->key);
        free(temp);
        temp = nextb;
    }
    free(oSymTable);
}
 
/*------------------------------------------------------------*/
 
size_t SymTable_getLength(SymTable_T oSymTable) {
    assert(oSymTable != NULL);
    return oSymTable->length;
}
 
/*------------------------------------------------------------*/
 
int SymTable_put(SymTable_T oSymTable,
                 const char *pcKey, const void *pvValue) {
    struct Binding* next;
    struct Binding* temp;
 
    assert(oSymTable != NULL);
    assert(pcKey != NULL);
 
    /* check for duplicate key */
    next = oSymTable->first;
    while (next != NULL) {
        if (strcmp(pcKey, next->key) == 0)
            return 0;
        next = next->next;
    }
 
    /* create new binding */
    temp = malloc(sizeof(struct Binding));
    if (temp == NULL) return 0;
 
    temp->key = malloc(strlen(pcKey) + 1);
    if (temp->key == NULL) { 
            free(temp); 
            return 0; 
        }
    strcpy(temp->key, pcKey);
 
    temp->value = pvValue;
    temp->next = oSymTable->first; /* insert at front */
    oSymTable->first = temp;
    oSymTable->length++;
    return 1;
}

/*------------------------------------------------------------*/

void *SymTable_replace(SymTable_T oSymTable,
                       const char *pcKey, const void *pvValue) {
    /* TODO */
    struct Binding *next;
 
    assert(oSymTable != NULL);
    assert(pcKey != NULL);
 
    /* check for duplicate key */
    next = oSymTable->first;
    while (next != NULL) {
        if (strcmp(pcKey, next->key) == 0) {
            const void* temp = next->value;
            next->value = pvValue;
            return (void*) temp;
        }
        next = next->next;
    }

    return NULL;
}

/*------------------------------------------------------------*/

int SymTable_contains(SymTable_T oSymTable, const char *pcKey) {

    struct Binding* next;

    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    next = oSymTable->first;

    while(next != NULL) {
        if(strcmp(next->key, pcKey) == 0) {
            return 1;
        }
        next = next->next;
    }

    return 0;
}

/*------------------------------------------------------------*/

void *SymTable_get(SymTable_T oSymTable, const char *pcKey) {
    
    struct Binding* next;

    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    next = oSymTable->first;

    while(next != NULL) {
        if(strcmp(next->key, pcKey) == 0) {
            return (void*) next->value;
        }
        next = next->next;
    }

    return NULL;
}

/*------------------------------------------------------------*/

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey) {
    
    struct Binding* next;
    void* retval;

    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    next = oSymTable->first;

    if (oSymTable->length == 0) {
        return NULL;
    }

    if(strcmp(next->key, pcKey) == 0) {
        if (next->next != NULL) {
            oSymTable->first = next->next;
        }
        else oSymTable->first = NULL;

        retval = (void*) next->value;

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

/*------------------------------------------------------------*/

void SymTable_map(SymTable_T oSymTable,
                  void (*pfApply)(const char *pcKey, void *pvVValue, void *pvExtra),
     const void *pvExtra) {
    struct Binding* next;

    assert(oSymTable != NULL);
    assert(pfApply != NULL);

    next = oSymTable->first;

    while(next != NULL) {

        pfApply(next->key, (void*) next->value, (void*) pvExtra);

        next = next->next;
    }
}