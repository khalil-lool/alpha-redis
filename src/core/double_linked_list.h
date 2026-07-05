#ifndef DOUBLE_LINKED_LIST_H
#define DOUBLE_LINKED_LIST_H

/* ─────────────────────────────────────────────────────────────
 *  Maillon de la liste doublement chaînée
 * ───────────────────────────────────────────────────────────── */
typedef struct DNode {
    char         *value;
    struct DNode *prev;
    struct DNode *next;
} DNode;

/* ─────────────────────────────────────────────────────────────
 *  Liste doublement chaînée avec sentinelles
 * ───────────────────────────────────────────────────────────── */
typedef struct {
    DNode *head;   /* sentinelle de tête  */
    DNode *tail;   /* sentinelle de queue */
    int    size;   /* nombre d'éléments   */
} DoublyLinkedList;

/* Création / destruction */
DoublyLinkedList *dll_create(void);
void              dll_free(DoublyLinkedList *list);

/* Opérations O(1) */
int   dll_lpush(DoublyLinkedList *list, const char *value);
int   dll_rpush(DoublyLinkedList *list, const char *value);
char *dll_lpop(DoublyLinkedList *list);
char *dll_rpop(DoublyLinkedList *list);

/* Debug */
void dll_print(const DoublyLinkedList *list);

#endif /* DOUBLE_LINKED_LIST_H */