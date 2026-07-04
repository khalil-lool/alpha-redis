#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stddef.h>

/* Déclaration anticipée : la définition complète de Node vient du module
 * de Membre 3 (Node.h). Je n'ai pas besoin de connaître son contenu,
 * juste de stocker et déplacer des pointeurs vers elle. */
typedef struct Node Node;

typedef struct
{
    Node   **buckets;   /* tableau de pointeurs vers têtes de listes chaînées */
    size_t   capacity;  /* nombre total de cases */
    size_t   size;      /* nombre de cases occupées (non-NULL) */
} DynamicArray;

/* Crée un tableau vide (buckets initialisés à NULL via calloc). */
DynamicArray *dynamic_array_create(size_t initial_capacity);

/* Libère buckets + la struct elle-même.
 * NE libère PAS les Node* qu'elle contient (responsabilité HashTable). */
void dynamic_array_destroy(DynamicArray *arr);

/* size / capacity — Membre 2 appelle ça après chaque SET pour savoir
 * s'il doit redimensionner (seuil recommandé : > 0.7). */
double dynamic_array_load_factor(const DynamicArray *arr);

/* Double la capacité du tableau. Alloue un NOUVEAU tableau de buckets vide
 * et le met en place dans arr. Retourne l'ANCIEN tableau de buckets via
 * old_buckets_out (et son ancienne capacité via old_capacity_out) pour que
 * l'appelant puisse rehasher.
 *
 * IMPORTANT : cette fonction ne déplace AUCUN Node. Elle ne connaît pas
 * leur structure (clé/valeur) — seul Membre 2 sait les rehasher.
 * Après l'appel, Membre 2 doit :
 *   1. Parcourir chaque case de old_buckets_out (0 à old_capacity)
 *   2. Pour chaque Node de la liste chaînée : recalculer
 *      hash_index(node->key, arr->capacity)   <- la NOUVELLE capacité
 *   3. Réinsérer le Node en tête de arr->buckets[nouvel_index]
 *   4. Une fois TOUS les Node déplacés : free(old_buckets_out)
 *
 * Retourne 1 si succès, 0 si l'allocation échoue (arr reste inchangé). */
int dynamic_array_resize(DynamicArray *arr, Node ***old_buckets_out, size_t *old_capacity_out);

#endif