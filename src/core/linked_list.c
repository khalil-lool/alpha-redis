#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"

/* ─────────────────────────────────────────────────────────────
 *  Crée un nouveau maillon avec la clé et la valeur données.
 *  Retourne NULL si l'allocation échoue.
 * ───────────────────────────────────────────────────────────── */
Node *node_create(const char *key, void *value, ValueType type)
{
    Node *node = malloc(sizeof(Node));
    if (!node) return NULL;

    node->key   = strdup(key);   /* copie la clé */
    node->value = value;         /* valeur : char* ou DoublyLinkedList* */
    node->type  = type;
    node->next  = NULL;
    return node;
}

/* ─────────────────────────────────────────────────────────────
 *  Libère la mémoire d'un maillon.
 *  La valeur elle-même est libérée selon son type.
 * ───────────────────────────────────────────────────────────── */
void node_free(Node *node)
{
    if (!node) return;
    free(node->key);
    /* La valeur string est libérée ici.
     * La DoublyLinkedList est libérée par dll_free (Membre 4). */
    if (node->type == TYPE_STRING)
        free(node->value);
    free(node);
}

/* ─────────────────────────────────────────────────────────────
 *  Insère un maillon en tête de liste — O(1)
 *  Retourne la nouvelle tête.
 * ───────────────────────────────────────────────────────────── */
Node *linked_list_insert(Node *head, const char *key, void *value, ValueType type)
{
    Node *node = node_create(key, value, type);
    if (!node) return head;

    /* Insertion en tête : le nouveau maillon pointe sur l'ancienne tête */
    node->next = head;
    return node;
}

/* ─────────────────────────────────────────────────────────────
 *  Recherche un maillon par clé dans la liste — O(n)
 *  Retourne le maillon trouvé ou NULL.
 * ───────────────────────────────────────────────────────────── */
Node *linked_list_search(Node *head, const char *key)
{
    Node *current = head;
    while (current != NULL)
    {
        if (strcmp(current->key, key) == 0)
            return current;
        current = current->next;
    }
    return NULL;
}

/* ─────────────────────────────────────────────────────────────
 *  Supprime le maillon avec la clé donnée — O(n)
 *  Retourne la nouvelle tête de liste.
 *  Retourne 1 via deleted_out si supprimé, 0 sinon.
 * ───────────────────────────────────────────────────────────── */
Node *linked_list_delete(Node *head, const char *key, int *deleted_out)
{
    Node *current = head;
    Node *prev    = NULL;
    *deleted_out  = 0;

    while (current != NULL)
    {
        if (strcmp(current->key, key) == 0)
        {
            /* Court-circuit : on retire current de la chaîne */
            if (prev == NULL)
                head = current->next;   /* suppression en tête */
            else
                prev->next = current->next;

            node_free(current);
            *deleted_out = 1;
            return head;
        }
        prev    = current;
        current = current->next;
    }
    return head;
}

/* ─────────────────────────────────────────────────────────────
 *  Libère toute la liste chaînée.
 * ───────────────────────────────────────────────────────────── */
void linked_list_free(Node *head)
{
    Node *current = head;
    while (current != NULL)
    {
        Node *next = current->next;
        node_free(current);
        current = next;
    }
}