#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "HashTable.h"
#include "HashFunction.h"

/* ─────────────────────────────────────────────────────────────
 *  Création de la HashTable
 *  Utilise le vrai DynamicArray du Membre 1
 * ───────────────────────────────────────────────────────────── */
HashTable *hashtable_create(size_t capacity)
{
    HashTable *table = malloc(sizeof(HashTable));
    if (!table) return NULL;

    /* Utilise dynamic_array_create du Membre 1 */
    table->array = dynamic_array_create(capacity);
    if (!table->array)
    {
        free(table);
        return NULL;
    }

    table->size = 0;
    return table;
}

/* ─────────────────────────────────────────────────────────────
 *  Destruction de la HashTable
 *  Libère tous les maillons de chaque bucket
 * ───────────────────────────────────────────────────────────── */
void hashtable_destroy(HashTable *table)
{
    if (!table) return;

    for (size_t i = 0; i < table->array->capacity; i++)
    {
        /* linked_list_free libère toute la liste chaînée du bucket */
        linked_list_free(table->array->buckets[i]);
    }

    dynamic_array_destroy(table->array);
    free(table);
}

/* ─────────────────────────────────────────────────────────────
 *  Recherche d'un nœud par clé — O(1) en moyenne
 * ───────────────────────────────────────────────────────────── */
Node *hashtable_find_node(HashTable *table, const char *key)
{
    size_t index = hash_index(key, table->array->capacity);
    return linked_list_search(table->array->buckets[index], key);
}

/* ─────────────────────────────────────────────────────────────
 *  Redimensionnement automatique du tableau
 *  Déclenché quand le facteur de charge dépasse 0.7
 * ───────────────────────────────────────────────────────────── */
static void hashtable_resize_if_needed(HashTable *table)
{
    if (dynamic_array_load_factor(table->array) <= 0.7) return;

    Node   **old_buckets;
    size_t   old_capacity;

    /* dynamic_array_resize double la capacité et retourne l'ancien tableau */
    if (!dynamic_array_resize(table->array, &old_buckets, &old_capacity))
        return;

    /* Rehasher tous les anciens nœuds dans le nouveau tableau */
    for (size_t i = 0; i < old_capacity; i++)
    {
        Node *current = old_buckets[i];
        while (current != NULL)
        {
            Node *next = current->next;

            /* Calculer le nouvel index avec la nouvelle capacité */
            size_t new_index = hash_index(current->key, table->array->capacity);

            /* Réinsérer en tête du nouveau bucket */
            current->next = table->array->buckets[new_index];
            table->array->buckets[new_index] = current;
            table->array->size++;

            current = next;
        }
    }

    /* Libérer l'ancien tableau de buckets (pas les Node, déjà déplacés) */
    free(old_buckets);
}

/* ─────────────────────────────────────────────────────────────
 *  SET key value — O(1) en moyenne
 *  Si la clé existe : met à jour la valeur
 *  Sinon : crée un nouveau maillon en tête du bucket
 * ───────────────────────────────────────────────────────────── */
void hashtable_set(HashTable *table, const char *key, const char *value)
{
    Node *existing = hashtable_find_node(table, key);

    if (existing)
    {
        /* Clé existante : mise à jour de la valeur */
        if (existing->type == TYPE_STRING)
            free(existing->value);
        existing->value = strdup(value);
        existing->type  = TYPE_STRING;
        return;
    }

    /* Nouvelle clé : insertion en tête du bucket */
    size_t index = hash_index(key, table->array->capacity);
    table->array->buckets[index] = linked_list_insert(
        table->array->buckets[index], key, strdup(value), TYPE_STRING
    );
    table->array->size++;
    table->size++;

    /* Redimensionner si nécessaire */
    hashtable_resize_if_needed(table);
}

/* ─────────────────────────────────────────────────────────────
 *  GET key — O(1) en moyenne
 *  Retourne la valeur associée ou NULL si clé absente
 * ───────────────────────────────────────────────────────────── */
const char *hashtable_get(HashTable *table, const char *key)
{
    Node *node = hashtable_find_node(table, key);

    if (!node)             return NULL;   /* clé inexistante  */
    if (node->type != TYPE_STRING) return NULL;   /* mauvais type     */

    return (const char *)node->value;
}

/* ─────────────────────────────────────────────────────────────
 *  DEL key — O(1) en moyenne
 *  Retourne 1 si supprimé, 0 si clé inexistante
 * ───────────────────────────────────────────────────────────── */
int hashtable_del(HashTable *table, const char *key)
{
    size_t index   = hash_index(key, table->array->capacity);
    int    deleted = 0;

    table->array->buckets[index] = linked_list_delete(
        table->array->buckets[index], key, &deleted
    );

    if (deleted)
    {
        table->array->size--;
        table->size--;
    }

    return deleted;
}