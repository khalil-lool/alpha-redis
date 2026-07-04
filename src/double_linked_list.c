/* ============================================================
 *  α-Redis — Chapitre 4 : Liste Doublement Chaînée
 *  Membre 4 — Type de donnée "Liste"
 *  Commandes : LPUSH, RPUSH, LPOP, RPOP  (complexité O(1))
 * ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ─────────────────────────────────────────────────────────────
 *  STRUCTURE : Maillon (DNode)
 *  Chaque maillon contient :
 *    - value : la valeur stockée (chaîne de caractères)
 *    - prev  : pointeur vers le maillon précédent
 *    - next  : pointeur vers le maillon suivant
 * ───────────────────────────────────────────────────────────── */
typedef struct DNode {
    char        *value;     /* valeur stockée                  */
    struct DNode *prev;     /* ← maillon précédent             */
    struct DNode *next;     /* → maillon suivant               */
} DNode;


/* ─────────────────────────────────────────────────────────────
 *  STRUCTURE : Liste Doublement Chaînée
 *  Utilise deux sentinelles (head, tail) pour simplifier
 *  les insertions/suppressions aux extrémités.
 *
 *  Structure avec 3 éléments A, B, C :
 *    head ↔ [A] ↔ [B] ↔ [C] ↔ tail
 * ───────────────────────────────────────────────────────────── */
typedef struct {
    DNode *head;    /* sentinelle de tête (ne stocke pas de valeur) */
    DNode *tail;    /* sentinelle de queue (ne stocke pas de valeur) */
    int    size;    /* nombre d'éléments réels                       */
} DoublyLinkedList;


/* ─────────────────────────────────────────────────────────────
 *  Crée un nouveau maillon avec la valeur donnée.
 *  Retourne NULL si l'allocation échoue.
 * ───────────────────────────────────────────────────────────── */
DNode *dnode_create(const char *value) {
    DNode *node = (DNode *)malloc(sizeof(DNode));
    if (!node) {
        fprintf(stderr, "Erreur : allocation mémoire échouée.\n");
        return NULL;
    }
    node->value = strdup(value);  /* copie la chaîne               */
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
 *  Crée les deux sentinelles et les relie.
 *  Retourne NULL si l'allocation échoue.
 * ───────────────────────────────────────────────────────────── */
DoublyLinkedList *dll_create(void) {
    DoublyLinkedList *list = (DoublyLinkedList *)malloc(sizeof(DoublyLinkedList));
    if (!list) return NULL;

    /* Création des sentinelles */
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
 *  Libère toute la mémoire de la liste (maillons + sentinelles).
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
 *
 *  Insère un nouveau maillon juste après la sentinelle head.
 *
 *  Avant : head ↔ [A] ↔ ...
 *  Après : head ↔ [NEW] ↔ [A] ↔ ...
 *
 *  Retourne la nouvelle taille, ou -1 en cas d'erreur.
 * ───────────────────────────────────────────────────────────── */
int dll_lpush(DoublyLinkedList *list, const char *value) {
    if (!list) return -1;

    DNode *node = dnode_create(value);
    if (!node) return -1;

    /* NEW s'insère entre head et l'ancien premier élément */
    node->next = list->head->next;   /* NEW → ancien premier        */
    node->prev = list->head;         /* head ← NEW                  */

    /* Mise à jour des voisins */
    list->head->next->prev = node;   /* ancien premier ← NEW        */
    list->head->next       = node;   /* head → NEW                  */

    list->size++;
    return list->size;
}


/* ─────────────────────────────────────────────────────────────
 *  RPUSH — Insertion en QUEUE  O(1)
 *
 *  Insère un nouveau maillon juste avant la sentinelle tail.
 *
 *  Avant : ... ↔ [Z] ↔ tail
 *  Après : ... ↔ [Z] ↔ [NEW] ↔ tail
 *
 *  Retourne la nouvelle taille, ou -1 en cas d'erreur.
 * ───────────────────────────────────────────────────────────── */
int dll_rpush(DoublyLinkedList *list, const char *value) {
    if (!list) return -1;

    DNode *node = dnode_create(value);
    if (!node) return -1;

    /* NEW s'insère entre l'ancien dernier et tail */
    node->prev = list->tail->prev;   /* ancien dernier ← NEW        */
    node->next = list->tail;         /* NEW → tail                  */

    /* Mise à jour des voisins */
    list->tail->prev->next = node;   /* ancien dernier → NEW        */
    list->tail->prev       = node;   /* NEW ← tail                  */

    list->size++;
    return list->size;
}


/* ─────────────────────────────────────────────────────────────
 *  LPOP — Extraction en TÊTE  O(1)
 *
 *  Retire et retourne le premier élément réel (head->next).
 *  Retourne NULL si la liste est vide.
 *
 *  Avant : head ↔ [A] ↔ [B] ↔ ...
 *  Après : head ↔ [B] ↔ ...   (retourne "A")
 *
 *  ATTENTION : l'appelant doit libérer la chaîne retournée
 *              avec free() après utilisation.
 * ───────────────────────────────────────────────────────────── */
char *dll_lpop(DoublyLinkedList *list) {
    if (!list || list->size == 0) return NULL;

    DNode *target = list->head->next;   /* premier élément réel    */

    /* Court-circuit : on retire target de la chaîne */
    list->head->next       = target->next;   /* head → B            */
    target->next->prev     = list->head;     /* head ← B            */

    /* On récupère la valeur avant de libérer le maillon */
    char *value = target->value;
    target->value = NULL;   /* évite que dnode_free libère la valeur */
    target->prev  = NULL;
    target->next  = NULL;
    free(target);           /* libère le maillon mais PAS la valeur  */

    list->size--;
    return value;           /* l'appelant doit faire free(value)     */
}


/* ─────────────────────────────────────────────────────────────
 *  RPOP — Extraction en QUEUE  O(1)
 *
 *  Retire et retourne le dernier élément réel (tail->prev).
 *  Retourne NULL si la liste est vide.
 *
 *  Avant : ... ↔ [Y] ↔ [Z] ↔ tail
 *  Après : ... ↔ [Y] ↔ tail   (retourne "Z")
 *
 *  ATTENTION : l'appelant doit libérer la chaîne retournée
 *              avec free() après utilisation.
 * ───────────────────────────────────────────────────────────── */
char *dll_rpop(DoublyLinkedList *list) {
    if (!list || list->size == 0) return NULL;

    DNode *target = list->tail->prev;   /* dernier élément réel    */

    /* Court-circuit : on retire target de la chaîne */
    list->tail->prev       = target->prev;   /* Y ← tail            */
    target->prev->next     = list->tail;     /* Y → tail            */

    /* On récupère la valeur avant de libérer le maillon */
    char *value = target->value;
    target->value = NULL;
    target->prev  = NULL;
    target->next  = NULL;
    free(target);

    list->size--;
    return value;
}


/* ─────────────────────────────────────────────────────────────
 *  Affiche tous les éléments de la liste (pour debug/tests).
 *  Format : head ↔ [A] ↔ [B] ↔ [C] ↔ tail
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


/* ─────────────────────────────────────────────────────────────
 *  TESTS — Validation des 4 opérations
 * ───────────────────────────────────────────────────────────── */
int main(void) {
    printf("==================================================\n");
    printf("  Tests de la Liste Doublement Chainee - a-Redis  \n");
    printf("==================================================\n\n");

    DoublyLinkedList *lst = dll_create();

    /* ── Test LPUSH ─────────────────────────────────────── */
    printf("--- LPUSH ---\n");
    dll_lpush(lst, "B");
    dll_lpush(lst, "A");
    printf("Apres LPUSH B puis LPUSH A : ");
    dll_print(lst);
    /* Attendu : head <-> [A] <-> [B] <-> tail */

    /* ── Test RPUSH ─────────────────────────────────────── */
    printf("\n--- RPUSH ---\n");
    dll_rpush(lst, "C");
    dll_rpush(lst, "D");
    printf("Apres RPUSH C puis RPUSH D : ");
    dll_print(lst);
    /* Attendu : head <-> [A] <-> [B] <-> [C] <-> [D] <-> tail */

    /* ── Test LPOP ──────────────────────────────────────── */
    printf("\n--- LPOP ---\n");
    char *val = dll_lpop(lst);
    printf("LPOP retourne : '%s'\n", val);
    printf("Liste restante : ");
    dll_print(lst);
    free(val);  /* l'appelant libère la valeur */
    /* Attendu : A retiré */

    /* ── Test RPOP ──────────────────────────────────────── */
    printf("\n--- RPOP ---\n");
    val = dll_rpop(lst);
    printf("RPOP retourne : '%s'\n", val);
    printf("Liste restante : ");
    dll_print(lst);
    free(val);
    /* Attendu : D retiré */

    /* ── Test liste vide ────────────────────────────────── */
    printf("\n--- Liste vide ---\n");
    DoublyLinkedList *lst2 = dll_create();
    printf("LPOP sur liste vide : %s\n", dll_lpop(lst2) ? "valeur" : "(nil)");
    printf("RPOP sur liste vide : %s\n", dll_rpop(lst2) ? "valeur" : "(nil)");

    /* ── Libération mémoire ─────────────────────────────── */
    dll_free(lst);
    dll_free(lst2);

    printf("\n[OK] Tous les tests passes avec succes !\n");
    return 0;
}
