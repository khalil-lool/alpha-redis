/* ============================================================
 *  α-Redis — Module : LinkedList (implémentation)
 *  Membre 3 — Gestion des collisions par chaînage séparé
 * ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"

LinkedList *ll_create(void) {
    LinkedList *list = (LinkedList *)malloc(sizeof(LinkedList));
    if (!list) {
        fprintf(stderr, "Erreur : allocation memoire echouee (ll_create).\n");
        return NULL;
    }
    list->head = NULL;
    list->size = 0;
    return list;
}

void ll_free(LinkedList *list) {
    if (!list) return;

    Node *current = list->head;
    while (current != NULL) {
        Node *next = current->next;
        node_free(current);  /* libère uniquement key + value TYPE_STRING */
        current = next;
    }
    free(list);
}

int ll_insert(LinkedList *list, const char *key, ValueType type, void *value) {
    if (!list) return -1;

    /* 1. Vérifier si la clé existe déjà -> mise à jour */
    Node *current = list->head;
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            /* Mise à jour de la valeur existante */
            if (current->type == TYPE_STRING) {
                free(current->value);
            }
            if (type == TYPE_STRING) {
                current->value = strdup((const char *)value);
                if (!current->value) return -1;
            } else {
                current->value = value;
            }
            current->type = type;
            return 0; /* mise à jour, pas une nouvelle entrée */
        }
        current = current->next;
    }

    /* 2. La clé n'existe pas -> insertion en tête (O(1)) */
    Node *new_node = node_create(key, type, value);
    if (!new_node) return -1;

    new_node->next = list->head;
    list->head = new_node;
    list->size++;
    return 1; /* nouvelle entrée créée */
}

Node *ll_search(LinkedList *list, const char *key) {
    if (!list) return NULL;

    Node *current = list->head;
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL; /* clé non trouvée */
}

int ll_contains(LinkedList *list, const char *key) {
    return ll_search(list, key) != NULL;
}

int ll_delete(LinkedList *list, const char *key) {
    if (!list) return 0;

    Node *current = list->head;
    Node *previous = NULL;

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            if (previous == NULL) {
                list->head = current->next; /* suppression du premier maillon */
            } else {
                previous->next = current->next; /* on "recolle" la liste */
            }
            node_free(current);
            list->size--;
            return 1;
        }
        previous = current;
        current = current->next;
    }
    return 0; /* clé non trouvée */
}

int ll_is_empty(const LinkedList *list) {
    return (list == NULL || list->head == NULL);
}

int ll_size(const LinkedList *list) {
    return list ? list->size : 0;
}

void ll_print(const LinkedList *list) {
    if (!list || list->head == NULL) {
        printf("(liste vide)\n");
        return;
    }

    Node *current = list->head;
    while (current != NULL) {
        if (current->type == TYPE_STRING) {
            printf("[%s:%s] -> ", current->key, (char *)current->value);
        } else {
            printf("[%s:<liste>] -> ", current->key);
        }
        current = current->next;
    }
    printf("NULL\n");
}
