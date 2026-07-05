/* ============================================================
 *  α-Redis — Liste Doublement Chaînée
 *  Membre 4 — Type de donnée "Liste"
 *  Commandes : LPUSH, RPUSH, LPOP, RPOP  (complexité O(1))
 * ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "double_linked_list.h"

/* ─────────────────────────────────────────────────────────────
 *  Crée un nouveau maillon avec la valeur donnée.
 * ───────────────────────────────────────────────────────────── */
DNode *dnode_create(const char *value) {
    DNode *node = (DNode *)malloc(sizeof(DNode));
    if (!node) {
        fprintf(stderr, "Erreur : allocation mémoire échouée.\n");
        return NULL;
    }
    node->value = strdup(value);
    node->prev  = NULL;
    node->next  = NULL;
    return node;
}

/* ─────────────────────────────────────────────────────────────
 *  Libère la mémoire d'un maillon.
 * ───────────────────────────────────────────────────────────── */
void dnode_free(DNode *node) {
    if (!node) return;
    free(node->value);
    free(node);
}

/* ─────────────────────────────────────────────────────────────
 *  Initialise une liste doublement chaînée vide.
 * ───────────────────────────────────────────────────────────── */
DoublyLinkedList *dll_create(void) {
    DoublyLinkedList *list = (DoublyLinkedList *)malloc(sizeof(DoublyLinkedList));
    if (!list) return NULL;

    list->head = dnode_create("");
    list->tail = dnode_create("");

    if (!list->head || !list->tail) {
        free(list);
        return NULL;
    }

    /* Liaison initiale : head ↔ tail */
    list->head->next = list->tail;
    list->tail->prev = list->head;
    list->size = 0;

    return list;
}

/* ─────────────────────────────────────────────────────────────
 *  Libère toute la mémoire de la liste.
 * ───────────────────────────────────────────────────────────── */
void dll_free(DoublyLinkedList *list) {
    if (!list) return;

    DNode *cur = list->head;
    while (cur) {
        DNode *next = cur->next;
        dnode_free(cur);
        cur = next;
    }
    free(list);
}

/* ─────────────────────────────────────────────────────────────
 *  LPUSH — Insertion en TÊTE  O(1)
 *  Avant : head ↔ [A] ↔ ...
 *  Après : head ↔ [NEW] ↔ [A] ↔ ...
 * ───────────────────────────────────────────────────────────── */
int dll_lpush(DoublyLinkedList *list, const char *value) {
    if (!list) return -1;

    DNode *node = dnode_create(value);
    if (!node) return -1;

    node->next = list->head->next;
    node->prev = list->head;
    list->head->next->prev = node;
    list->head->next       = node;

    list->size++;
    return list->size;
}

/* ─────────────────────────────────────────────────────────────
 *  RPUSH — Insertion en QUEUE  O(1)
 *  Avant : ... ↔ [Z] ↔ tail
 *  Après : ... ↔ [Z] ↔ [NEW] ↔ tail
 * ───────────────────────────────────────────────────────────── */
int dll_rpush(DoublyLinkedList *list, const char *value) {
    if (!list) return -1;

    DNode *node = dnode_create(value);
    if (!node) return -1;

    node->prev = list->tail->prev;
    node->next = list->tail;
    list->tail->prev->next = node;
    list->tail->prev       = node;

    list->size++;
    return list->size;
}

/* ─────────────────────────────────────────────────────────────
 *  LPOP — Extraction en TÊTE  O(1)
 *  Retourne NULL si liste vide.
 *  L'appelant doit libérer la valeur retournée avec free().
 * ───────────────────────────────────────────────────────────── */
char *dll_lpop(DoublyLinkedList *list) {
    if (!list || list->size == 0) return NULL;

    DNode *target = list->head->next;

    list->head->next   = target->next;
    target->next->prev = list->head;

    char *value   = target->value;
    target->value = NULL;
    target->prev  = NULL;
    target->next  = NULL;
    free(target);

    list->size--;
    return value;
}

/* ─────────────────────────────────────────────────────────────
 *  RPOP — Extraction en QUEUE  O(1)
 *  Retourne NULL si liste vide.
 *  L'appelant doit libérer la valeur retournée avec free().
 * ───────────────────────────────────────────────────────────── */
char *dll_rpop(DoublyLinkedList *list) {
    if (!list || list->size == 0) return NULL;

    DNode *target = list->tail->prev;

    list->tail->prev   = target->prev;
    target->prev->next = list->tail;

    char *value   = target->value;
    target->value = NULL;
    target->prev  = NULL;
    target->next  = NULL;
    free(target);

    list->size--;
    return value;
}

/* ─────────────────────────────────────────────────────────────
 *  Affiche la liste (debug)
 * ───────────────────────────────────────────────────────────── */
void dll_print(const DoublyLinkedList *list) {
    if (!list || list->size == 0) {
        printf("(liste vide)\n");
        return;
    }
    printf("head");
    DNode *cur = list->head->next;
    while (cur != list->tail) {
        printf(" <-> [%s]", cur->value);
        cur = cur->next;
    }
    printf(" <-> tail  (size=%d)\n", list->size);
}