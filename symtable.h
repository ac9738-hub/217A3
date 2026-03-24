#ifndef SYMTABLE_INCLUDED
#define SYMTABLE_INCLUDED

#include <stddef.h>

/*--------------------------------------------------------------------*/
/* SymTable_T is an opaque pointer to a symbol table structure.      */
/* Clients can declare variables of type SymTable_T, but cannot      */
/* dereference them or access the internal structure directly.       */
/*--------------------------------------------------------------------*/

typedef struct SymTable_T *SymTable_T;

/*--------------------------------------------------------------------*/
/* Creates and returns a new, empty symbol table, or NULL if         */
/* insufficient memory is available.                                 */
/*--------------------------------------------------------------------*/

SymTable_T SymTable_new(void);

/*--------------------------------------------------------------------*/
/* Frees all memory occupied by oSymTable. It is a checked runtime   */
/* error for oSymTable to be NULL.                                   */
/*--------------------------------------------------------------------*/

void SymTable_free(SymTable_T oSymTable);

/*--------------------------------------------------------------------*/
/* Returns the number of bindings in oSymTable. It is a checked      */
/* runtime error for oSymTable to be NULL.                           */
/*--------------------------------------------------------------------*/

size_t SymTable_getLength(SymTable_T oSymTable);

/*--------------------------------------------------------------------*/
/* If oSymTable does not contain a binding with key pcKey, adds a    */
/* new binding with key pcKey and value pvValue to oSymTable and     */
/* returns 1. Otherwise, leaves oSymTable unchanged and returns 0.   */
/* Returns 0 if insufficient memory is available. It is a checked    */
/* runtime error for oSymTable or pcKey to be NULL.                  */
/*--------------------------------------------------------------------*/

int SymTable_put(SymTable_T oSymTable, const char *pcKey, 
                 const void *pvValue);

/*--------------------------------------------------------------------*/
/* If oSymTable contains a binding with key pcKey, replaces the      */
/* binding's value with pvValue and returns the old value.           */
/* Otherwise, leaves oSymTable unchanged and returns NULL. It is a   */
/* checked runtime error for oSymTable or pcKey to be NULL.          */
/*--------------------------------------------------------------------*/

void *SymTable_replace(SymTable_T oSymTable, const char *pcKey, 
                       const void *pvValue);

/*--------------------------------------------------------------------*/
/* Returns 1 if oSymTable contains a binding with key pcKey, and 0   */
/* otherwise. It is a checked runtime error for oSymTable or pcKey   */
/* to be NULL.                                                        */
/*--------------------------------------------------------------------*/

int SymTable_contains(SymTable_T oSymTable, const char *pcKey);

/*--------------------------------------------------------------------*/
/* Returns the value of the binding with key pcKey in oSymTable, or  */
/* NULL if no such binding exists. It is a checked runtime error for */
/* oSymTable or pcKey to be NULL.                                    */
/*--------------------------------------------------------------------*/

void *SymTable_get(SymTable_T oSymTable, const char *pcKey);

/*--------------------------------------------------------------------*/
/* If oSymTable contains a binding with key pcKey, removes that      */
/* binding from oSymTable and returns the binding's value.           */
/* Otherwise, leaves oSymTable unchanged and returns NULL. It is a   */
/* checked runtime error for oSymTable or pcKey to be NULL.          */
/*--------------------------------------------------------------------*/

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey);

/*--------------------------------------------------------------------*/
/* Applies function *pfApply to each binding in oSymTable, passing   */
/* pvExtra as an extra argument. That is, the function calls         */
/* (*pfApply)(pcKey, pvValue, pvExtra) for each binding. It is a     */
/* checked runtime error for oSymTable or pfApply to be NULL.        */
/*--------------------------------------------------------------------*/

void SymTable_map(SymTable_T oSymTable,
                  void (*pfApply)(const char *pcKey, void *pvValue, 
                                  void *pvExtra),
                  const void *pvExtra);

#endif