#ifndef LINKED_LIST_H
#define LINKED_LIST_H

/* ─────────────────────────────────────────────────────────────
 *  Type de valeur stockée dans un maillon
 * ───────────────────────────────────────────────────────────── */
typedef enum
{
    TYPE_STRING,   /* la valeur est un char*             */
    TYPE_LIST      /* la valeur est un DoublyLinkedList* */
} ValueType;

/* ─────────────────────────────────────────────────────────────
 *  Maillon de la liste chaînée simple (gestion des collisions)
 *  Chaque case du tableau dynamique pointe vers une tête de
 *  liste chaînée de Node.
 * ───────────────────────────────────────────────────────────── */
typedef struct Node
{
    char          *key;    /* clé de la paire clé-valeur    */
    void          *value;  /* char* ou DoublyLinkedList*    */
    ValueType      type;   /* STRING ou LIST                */
    struct Node   *next;   /* maillon suivant (collisions)  */
} Node;

/* Création / libération */
Node *node_create(const char *key, void *value, ValueType type);
void  node_free(Node *node);

/* Opérations sur la liste */
Node *linked_list_insert(Node *head, const char *key, void *value, ValueType type);
Node *linked_list_search(Node *head, const char *key);
Node *linked_list_delete(Node *head, const char *key, int *deleted_out);
void  linked_list_free(Node *head);

#endif /* LINKED_LIST_H */