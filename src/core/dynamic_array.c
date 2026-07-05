#include <stdlib.h>
#include <string.h>
#include "dynamic_array.h"

/* ─────────────────────────────────────────────────────────────
 *  Crée un tableau dynamique vide.
 *  Tous les buckets sont initialisés à NULL via calloc.
 *  Retourne NULL si l'allocation échoue.
 * ───────────────────────────────────────────────────────────── */
DynamicArray *dynamic_array_create(size_t initial_capacity)
{
    DynamicArray *arr = malloc(sizeof(DynamicArray));
    if (!arr) return NULL;

    /* calloc initialise tous les pointeurs à NULL */
    arr->buckets  = calloc(initial_capacity, sizeof(Node *));
    if (!arr->buckets)
    {
        free(arr);
        return NULL;
    }

    arr->capacity = initial_capacity;
    arr->size     = 0;
    return arr;
}

/* ─────────────────────────────────────────────────────────────
 *  Libère le tableau et sa structure.
 *  NE libère PAS les Node* contenus (responsabilité HashTable).
 * ───────────────────────────────────────────────────────────── */
void dynamic_array_destroy(DynamicArray *arr)
{
    if (!arr) return;
    free(arr->buckets);
    free(arr);
}

/* ─────────────────────────────────────────────────────────────
 *  Calcule le facteur de charge : size / capacity
 *  Membre 2 appelle ça après chaque SET.
 *  Seuil recommandé : > 0.7 → redimensionner.
 * ───────────────────────────────────────────────────────────── */
double dynamic_array_load_factor(const DynamicArray *arr)
{
    if (!arr || arr->capacity == 0) return 0.0;
    return (double)arr->size / (double)arr->capacity;
}

/* ─────────────────────────────────────────────────────────────
 *  Double la capacité du tableau.
 *
 *  Étapes :
 *  1. Alloue un nouveau tableau de buckets vide (double taille)
 *  2. Retourne l'ancien tableau via old_buckets_out
 *  3. Retourne l'ancienne capacité via old_capacity_out
 *  4. Met en place le nouveau tableau dans arr
 *
 *  C'est Membre 2 (HashTable) qui rehashe les Node
 *  dans le nouveau tableau après cet appel.
 *
 *  Retourne 1 si succès, 0 si allocation échoue.
 * ───────────────────────────────────────────────────────────── */
int dynamic_array_resize(DynamicArray *arr,
                         Node       ***old_buckets_out,
                         size_t       *old_capacity_out)
{
    if (!arr) return 0;

    size_t new_capacity = arr->capacity * 2;

    /* Nouveau tableau de buckets vide */
    Node **new_buckets = calloc(new_capacity, sizeof(Node *));
    if (!new_buckets) return 0;

    /* On retourne l'ancien tableau à l'appelant pour qu'il rehashe */
    *old_buckets_out  = arr->buckets;
    *old_capacity_out = arr->capacity;

    /* On installe le nouveau tableau */
    arr->buckets  = new_buckets;
    arr->capacity = new_capacity;
    arr->size     = 0;

    return 1;
}