#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stddef.h>

/* ============================================================
 * MOCK TEMPORAIRE — À SUPPRIMER APRÈS MERGE
 * ------------------------------------------------------------
 * Ces structures imitent :
 *   - Node          -> module du Membre 3 (LinkedList.c / Node.c)
 *   - DynamicArray  -> module du Membre 1 (DynamicArray.c)
 *
 * Elles me permettent de coder et tester HashTable.c en isolation
 * sur ma branche feature/hash-table. Dès que les vraies branches
 * sont mergées dans main, je supprime ce bloc et j'inclus :
 *   #include "Node.h"
 *   #include "DynamicArray.h"
 * ============================================================ */

typedef enum
{
    TYPE_STRING,
    TYPE_LIST // sera un pointeur vers une DoubleLinkedList (Membre 4)
} ValueType;

typedef struct Node
{
    char *key;
    ValueType type;
    void *value;        // char* si TYPE_STRING, DoubleLinkedList* si TYPE_LIST
    struct Node *next;   // chaînage séparé (collisions)
} Node;

/* Version simplifiée : tableau brut de buckets, sans logique de
 * redimensionnement (le redimensionnement appartient au Membre 1). */
typedef struct
{
    Node **buckets;
    size_t capacity;
} DynamicArray;

/* ============================================================
 * FIN DU MOCK
 * ============================================================ */

typedef struct
{
    DynamicArray *array;
    size_t size; // nombre de clés actuellement stockées
} HashTable;

/* Création / destruction */
HashTable *hashtable_create(size_t capacity);
void hashtable_destroy(HashTable *table);

/* API principale imposée par le sujet */
void hashtable_set(HashTable *table, const char *key, const char *value);
const char *hashtable_get(HashTable *table, const char *key);
int hashtable_del(HashTable *table, const char *key);

/* Utilitaire interne (utile aussi pour les tests / debug) */
Node *hashtable_find_node(HashTable *table, const char *key);

#endif // HASH_TABLE_H
