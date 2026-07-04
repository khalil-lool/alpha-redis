#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "HashTable.h"
#include "HashFunction.h"

/* ------------------------------------------------------------
 * Création / destruction
 * ------------------------------------------------------------ */

HashTable *hashtable_create(size_t capacity)
{
    HashTable *table = malloc(sizeof(HashTable));
    if (!table) return NULL;

    table->array = malloc(sizeof(DynamicArray));
    if (!table->array)
    {
        free(table);
        return NULL;
    }

    table->array->buckets = calloc(capacity, sizeof(Node *));
    if (!table->array->buckets)
    {
        free(table->array);
        free(table);
        return NULL;
    }

    table->array->capacity = capacity;
    table->size = 0;

    return table;
}

static void free_node(Node *node)
{
    free(node->key);
    if (node->type == TYPE_STRING)
    {
        free(node->value);
    }
    // TYPE_LIST : la libération de la DoubleLinkedList revient au
    // module du Membre 4 (double_linked_list_destroy), à brancher
    // ici après intégration.
    free(node);
}

void hashtable_destroy(HashTable *table)
{
    if (!table) return;

    for (size_t i = 0; i < table->array->capacity; i++)
    {
        Node *current = table->array->buckets[i];
        while (current)
        {
            Node *next = current->next;
            free_node(current);
            current = next;
        }
    }

    free(table->array->buckets);
    free(table->array);
    free(table);
}

/* ------------------------------------------------------------
 * Recherche d'un nœud par clé (parcours de la liste chaînée du bucket)
 * ------------------------------------------------------------ */

Node *hashtable_find_node(HashTable *table, const char *key)
{
    size_t index = hash_index(key, table->array->capacity);
    Node *current = table->array->buckets[index];

    while (current)
    {
        if (strcmp(current->key, key) == 0)
        {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

/* ------------------------------------------------------------
 * SET key value  -> O(1) en moyenne
 * ------------------------------------------------------------ */

void hashtable_set(HashTable *table, const char *key, const char *value)
{
    Node *existing = hashtable_find_node(table, key);

    if (existing)
    {
        // Clé déjà présente : on remplace la valeur
        if (existing->type == TYPE_STRING)
        {
            free(existing->value);
        }
        existing->value = strdup(value);
        existing->type = TYPE_STRING;
        return;
    }

    // Nouvelle clé : on crée un maillon en tête de la liste du bucket
    size_t index = hash_index(key, table->array->capacity);

    Node *node = malloc(sizeof(Node));
    node->key = strdup(key);
    node->type = TYPE_STRING;
    node->value = strdup(value);
    node->next = table->array->buckets[index];

    table->array->buckets[index] = node;
    table->size++;
}

/* ------------------------------------------------------------
 * GET key -> O(1) en moyenne
 * ------------------------------------------------------------ */

const char *hashtable_get(HashTable *table, const char *key)
{
    Node *node = hashtable_find_node(table, key);

    if (!node)
    {
        return NULL; // clé inexistante -> à gérer par l'interpréteur (Membre 5)
    }

    if (node->type != TYPE_STRING)
    {
        return NULL; // mauvais type -> erreur à remonter par l'interpréteur
    }

    return (const char *)node->value;
}

/* ------------------------------------------------------------
 * DEL key -> O(1) en moyenne, retourne 1 si supprimé, 0 sinon
 * ------------------------------------------------------------ */

int hashtable_del(HashTable *table, const char *key)
{
    size_t index = hash_index(key, table->array->capacity);
    Node *current = table->array->buckets[index];
    Node *prev = NULL;

    while (current)
    {
        if (strcmp(current->key, key) == 0)
        {
            if (prev)
            {
                prev->next = current->next;
            }
            else
            {
                table->array->buckets[index] = current->next;
            }

            free_node(current);
            table->size--;
            return 1;
        }

        prev = current;
        current = current->next;
    }

    return 0; // clé inexistante
}
