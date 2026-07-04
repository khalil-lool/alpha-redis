/* ============================================================
 *  α-Redis — Module : Node (implémentation)
 *  Membre 3 — Listes chaînées simples (gestion des collisions)
 * ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node.h"

Node *node_create(const char *key, ValueType type, void *value) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node) {
        fprintf(stderr, "Erreur : allocation memoire echouee (node_create).\n");
        return NULL;
    }

    node->key = strdup(key);
    if (!node->key) {
        fprintf(stderr, "Erreur : allocation memoire echouee (strdup key).\n");
        free(node);
        return NULL;
    }

    node->type = type;

    if (type == TYPE_STRING) {
        /* On copie la chaîne de valeur pour que le Node en soit propriétaire */
        node->value = strdup((const char *)value);
        if (!node->value) {
            fprintf(stderr, "Erreur : allocation memoire echouee (strdup value).\n");
            free(node->key);
            free(node);
            return NULL;
        }
    } else {
        /* TYPE_LIST : on stocke juste le pointeur, pas de copie */
        node->value = value;
    }

    node->next = NULL;
    return node;
}

void node_free(Node *node) {
    if (!node) return;

    free(node->key);

    /* On ne libère la valeur que si c'est une chaîne (TYPE_STRING).
     * Si c'est TYPE_LIST, la DoublyLinkedList doit être libérée
     * séparément via dll_free() par le module qui la possède. */
    if (node->type == TYPE_STRING) {
        free(node->value);
    }

    free(node);
}
