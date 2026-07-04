/* ============================================================
 *  α-Redis — Module : LinkedList (liste chaînée simple)
 *  Membre 3 — Gestion des collisions par chaînage séparé
 *
 *  Contexte : chaque case du tableau dynamique (Membre 1) pointe
 *  vers une LinkedList. Quand hash(clé1) == hash(clé2) (collision),
 *  les deux clés sont stockées à la suite dans la MÊME LinkedList.
 * ============================================================ */

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "node.h"

/* ─────────────────────────────────────────────────────────────
 *  STRUCTURE : LinkedList
 *    head : premier maillon (NULL si liste vide)
 *    size : nombre d'éléments (utile pour stats/debug)
 * ───────────────────────────────────────────────────────────── */
typedef struct {
    Node *head;
    int   size;
} LinkedList;

/* Crée une liste chaînée vide. Retourne NULL si allocation échouée. */
LinkedList *ll_create(void);

/* Libère toute la liste (tous les maillons).
 * ATTENTION : pour les maillons de type TYPE_LIST, cette fonction
 * ne libère PAS la DoublyLinkedList référencée (voir node.h) —
 * à libérer explicitement avant si nécessaire pour éviter une fuite. */
void ll_free(LinkedList *list);

/* insert - Ajoute une clé/valeur, ou met à jour la valeur si la clé
 * existe déjà (comportement SET). Utilisé par HashTable.set().
 * Retourne 1 si une NOUVELLE entrée a été créée, 0 si mise à jour,
 * -1 en cas d'erreur d'allocation. */
int ll_insert(LinkedList *list, const char *key, ValueType type, void *value);

/* search - Recherche une clé et retourne son maillon (Node*) complet,
 * pour que l'appelant puisse lire à la fois value ET type.
 * Retourne NULL si la clé n'existe pas dans cette liste. */
Node *ll_search(LinkedList *list, const char *key);

/* contains - Vérifie l'existence d'une clé (utile pour LPUSH/RPUSH
 * qui doivent savoir si une liste existe déjà avant d'en créer une). */
int ll_contains(LinkedList *list, const char *key);

/* delete - Supprime le maillon correspondant à une clé.
 * Retourne 1 si supprimé, 0 si la clé n'existait pas.
 * ATTENTION : si le maillon supprimé est de type TYPE_LIST, cette
 * fonction NE LIBÈRE PAS la DoublyLinkedList associée — l'appelant
 * (HashTable) doit appeler dll_free() sur cette liste AVANT de
 * supprimer le maillon, sinon fuite mémoire. */
int ll_delete(LinkedList *list, const char *key);

/* is_empty - Renvoie 1 si la liste est vide, 0 sinon.
 * Utile pour le Membre 1 : si une case devient vide après suppression,
 * on peut la remettre à NULL dans le tableau dynamique. */
int ll_is_empty(const LinkedList *list);

/* size - Retourne le nombre d'éléments dans la liste. */
int ll_size(const LinkedList *list);

/* print - Affiche la liste (debug / démonstration collisions).
 * Format : [clé1:valeur1] -> [clé2:valeur2] -> NULL */
void ll_print(const LinkedList *list);

#endif /* LINKED_LIST_H */
