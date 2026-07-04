/* ============================================================
 *  α-Redis — Module : Node (maillon générique)
 *  Membre 3 — Listes chaînées simples (gestion des collisions)
 * ============================================================ */

#ifndef NODE_H
#define NODE_H

/* ─────────────────────────────────────────────────────────────
 *  ENUM : ValueType
 *  Permet de savoir ce que contient réellement le champ "value"
 *  d'un maillon, puisqu'une clé du moteur peut stocker :
 *    - une chaîne de caractères classique  -> TYPE_STRING
 *    - une référence vers une liste doublement chaînée
 *      (module du Membre 4, pour LPUSH/RPUSH/LPOP/RPOP) -> TYPE_LIST
 * ───────────────────────────────────────────────────────────── */
typedef enum {
    TYPE_STRING,
    TYPE_LIST
} ValueType;

/* Déclaration anticipée (forward declaration) de la structure du Membre 4.
 * On ne connaît pas son contenu ici (pas besoin), on stocke juste un pointeur. */
struct DoublyLinkedList;

/* ─────────────────────────────────────────────────────────────
 *  STRUCTURE : Node
 *  Un maillon de la liste chaînée simple (chaînage séparé).
 *
 *    key   : la clé (chaîne de caractères, copiée avec strdup)
 *    type  : TYPE_STRING ou TYPE_LIST (voir ValueType ci-dessus)
 *    value : void* générique -> cast en char* si TYPE_STRING,
 *            ou en (struct DoublyLinkedList*) si TYPE_LIST
 *    next  : pointeur vers le maillon suivant (NULL si dernier)
 * ───────────────────────────────────────────────────────────── */
typedef struct Node {
    char           *key;
    ValueType       type;
    void           *value;
    struct Node    *next;
} Node;

/* Crée un nouveau maillon. Retourne NULL si l'allocation échoue.
 * NOTE : si type == TYPE_STRING, "value" doit être un (const char*)
 *        qui sera copié en interne (strdup). Si type == TYPE_LIST,
 *        "value" est déjà un pointeur vers une DoublyLinkedList
 *        existante (créée par le module du Membre 4) ; il n'est PAS
 *        dupliqué, on stocke juste le pointeur. */
Node *node_create(const char *key, ValueType type, void *value);

/* Libère la mémoire d'un maillon.
 * IMPORTANT : si type == TYPE_LIST, cette fonction NE LIBÈRE PAS
 * la DoublyLinkedList pointée (ownership du Membre 4 / HashTable) :
 * il faut appeler dll_free() sur cette liste AVANT de supprimer
 * le maillon, sinon fuite mémoire. Cette fonction libère uniquement
 * la clé et, si TYPE_STRING, la chaîne de valeur. */
void node_free(Node *node);

#endif /* NODE_H */
