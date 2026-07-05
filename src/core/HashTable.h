#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stddef.h>
#include "linked_list.h"
#include "dynamic_array.h"

/* ─────────────────────────────────────────────────────────────
 *  Table de hachage principale
 *  Combine le tableau dynamique (Membre 1) et les listes
 *  chaînées de collisions (Membre 3)
 * ───────────────────────────────────────────────────────────── */
typedef struct
{
    DynamicArray *array;   /* tableau dynamique de buckets  */
    size_t        size;    /* nombre de clés stockées       */
} HashTable;

/* Création / destruction */
HashTable  *hashtable_create(size_t capacity);
void        hashtable_destroy(HashTable *table);

/* API principale imposée par le sujet */
void        hashtable_set(HashTable *table, const char *key, const char *value);
const char *hashtable_get(HashTable *table, const char *key);
int         hashtable_del(HashTable *table, const char *key);

/* Utilitaire interne */
Node       *hashtable_find_node(HashTable *table, const char *key);

#endif /* HASH_TABLE_H */