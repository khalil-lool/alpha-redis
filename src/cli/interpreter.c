#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "interpreter.h"
#include "../core/double_linked_list.h"

/* ─────────────────────────────────────────
 * Codes couleurs ANSI
 * ───────────────────────────────────────── */
#define CYAN    "\033[1;36m"
#define VERT    "\033[1;32m"
#define JAUNE   "\033[1;33m"
#define ROUGE   "\033[1;31m"
#define RESET   "\033[0m"

/* ─────────────────────────────────────────
 * parse_command :
 * Découpe "SET nom Fatou" en parts[0..2]
 * Retourne le nombre de parties trouvées
 * ───────────────────────────────────────── */
static int parse_command(char *command, char *parts[3])
{
    int count = 0;
    char *token = strtok(command, " \t\n");

    while (token != NULL && count < 3)
    {
        parts[count] = token;
        count++;
        token = (count == 2) ? strtok(NULL, "\n") : strtok(NULL, " \t\n");
    }
    return count;
}

/* ─────────────────────────────────────────
 * execute_command :
 * Fonction principale de l'interpréteur.
 * Reçoit la commande brute, la découpe,
 * identifie la commande et appelle la bonne
 * fonction de HashTable ou DoublyLinkedList.
 * ───────────────────────────────────────── */
void execute_command(HashTable *table, char *command)
{
    char *parts[3] = {NULL, NULL, NULL};
    int count = parse_command(command, parts);

    if (count == 0) return;

    /* ─────────────────────────────────────────
     * SET key value — O(1)
     * Stocke une valeur string dans la base
     * ───────────────────────────────────────── */
    if (strcmp(parts[0], "SET") == 0)
    {
        if (count < 3)
        {
            printf(ROUGE "ERREUR : syntaxe → SET <clé> <valeur>\n" RESET);
            return;
        }
        hashtable_set(table, parts[1], parts[2]);
        printf(VERT "OK\n" RESET);
    }

    /* ─────────────────────────────────────────
     * GET key — O(1)
     * Retourne la valeur associée à la clé
     * ───────────────────────────────────────── */
    else if (strcmp(parts[0], "GET") == 0)
    {
        if (count < 2)
        {
            printf(ROUGE "ERREUR : syntaxe → GET <clé>\n" RESET);
            return;
        }
        const char *result = hashtable_get(table, parts[1]);
        if (result == NULL)
            printf("(nil)\n");
        else
            printf(CYAN "\"%s\"\n" RESET, result);
    }

    /* ─────────────────────────────────────────
     * DEL key — O(1)
     * Supprime le maillon de la liste chaînée
     * Retourne 1 si supprimé, 0 si inexistant
     * ───────────────────────────────────────── */
    else if (strcmp(parts[0], "DEL") == 0)
    {
        if (count < 2)
        {
            printf(ROUGE "ERREUR : syntaxe → DEL <clé>\n" RESET);
            return;
        }
        int deleted = hashtable_del(table, parts[1]);
        printf("(integer) %d\n", deleted);
    }

    /* ─────────────────────────────────────────
     * EXISTS key — O(1)
     * Vérifie si une clé existe
     * Retourne (integer) 1 si oui, 0 sinon
     * ───────────────────────────────────────── */
    else if (strcmp(parts[0], "EXISTS") == 0)
    {
        if (count < 2)
        {
            printf(ROUGE "ERREUR : syntaxe → EXISTS <clé>\n" RESET);
            return;
        }
        Node *node = hashtable_find_node(table, parts[1]);
        printf("(integer) %d\n", node != NULL ? 1 : 0);
    }

    /* ─────────────────────────────────────────
     * KEYS — O(n)
     * Parcourt tous les buckets du tableau
     * et affiche chaque clé trouvée
     * ───────────────────────────────────────── */
    else if (strcmp(parts[0], "KEYS") == 0)
    {
        int found = 0;
        for (size_t i = 0; i < table->array->capacity; i++)
        {
            Node *current = table->array->buckets[i];
            while (current != NULL)
            {
                printf(CYAN "\"%s\"\n" RESET, current->key);
                found++;
                current = current->next;
            }
        }
        if (found == 0)
            printf("(empty)\n");
    }

    /* ─────────────────────────────────────────
     * DBSIZE — O(1)
     * Retourne le nombre de clés stockées
     * table->size est mis à jour à chaque
     * SET et DEL automatiquement
     * ───────────────────────────────────────── */
    else if (strcmp(parts[0], "DBSIZE") == 0)
    {
        printf("(integer) %zu\n", table->size);
    }

    /* ─────────────────────────────────────────
     * FLUSHALL — O(n)
     * Vide toute la base de données
     * Parcourt tous les buckets et libère
     * chaque maillon de chaque liste chaînée
     * ───────────────────────────────────────── */
    else if (strcmp(parts[0], "FLUSHALL") == 0)
    {
        size_t capacity = table->array->capacity;
        for (size_t i = 0; i < capacity; i++)
        {
            Node *current = table->array->buckets[i];
            while (current != NULL)
            {
                Node *next = current->next;
                free(current->key);
                if (current->type == TYPE_STRING)
                    free(current->value);
                else if (current->type == TYPE_LIST)
                    dll_free((DoublyLinkedList *)current->value);
                free(current);
                current = next;
            }
            table->array->buckets[i] = NULL;
        }
        table->size = 0;
        printf(VERT "OK\n" RESET);
    }

    /* ─────────────────────────────────────────
     * PING — O(1)
     * Teste si le moteur répond
     * ───────────────────────────────────────── */
    else if (strcmp(parts[0], "PING") == 0)
    {
        printf(VERT "PONG\n" RESET);
    }

    /* ─────────────────────────────────────────
     * DATA — O(n)
     * Affiche toutes les clés et leurs valeurs
     * ───────────────────────────────────────── */
    else if (strcmp(parts[0], "DATA") == 0)
    {
        int found = 0;
        printf("\n" JAUNE "=== Données stockées ===\n\n" RESET);
        for (size_t i = 0; i < table->array->capacity; i++)
        {
            Node *current = table->array->buckets[i];
            while (current != NULL)
            {
                if (current->type == TYPE_STRING)
                {
                    printf("  " CYAN "%s" RESET " (string) : \"%s\"\n",
                           current->key, (char *)current->value);
                }
                else if (current->type == TYPE_LIST)
                {
                    DoublyLinkedList *list = (DoublyLinkedList *)current->value;
                    printf("  " CYAN "%s" RESET " (list, %d élément(s))\n",
                           current->key, list->size);
                }
                found++;
                current = current->next;
            }
        }
        if (found == 0)
            printf("  (base de données vide)\n");
        printf("\n");
    }

    /* ─────────────────────────────────────────
     * LPUSH key value — O(1)
     * Insère en tête de liste doublement chaînée
     * Crée la liste si la clé n'existe pas
     * ───────────────────────────────────────── */
    else if (strcmp(parts[0], "LPUSH") == 0)
    {
        if (count < 3)
        {
            printf(ROUGE "ERREUR : syntaxe → LPUSH <clé> <valeur>\n" RESET);
            return;
        }
        Node *node = hashtable_find_node(table, parts[1]);
        DoublyLinkedList *list;

        if (node == NULL)
        {
            /* Clé inexistante : on crée une nouvelle liste */
            list = dll_create();
            hashtable_set(table, parts[1], (const char *)list);
            node = hashtable_find_node(table, parts[1]);
            node->type  = TYPE_LIST;
            node->value = list;
        }
        else if (node->type != TYPE_LIST)
        {
            printf(ROUGE "ERREUR : la clé '%s' contient déjà une valeur string\n" RESET, parts[1]);
            return;
        }
        else
        {
            list = (DoublyLinkedList *)node->value;
        }

        int size = dll_lpush(list, parts[2]);
        printf("(integer) %d\n", size);
    }

    /* ─────────────────────────────────────────
     * RPUSH key value — O(1)
     * Insère en queue de liste doublement chaînée
     * Crée la liste si la clé n'existe pas
     * ───────────────────────────────────────── */
    else if (strcmp(parts[0], "RPUSH") == 0)
    {
        if (count < 3)
        {
            printf(ROUGE "ERREUR : syntaxe → RPUSH <clé> <valeur>\n" RESET);
            return;
        }
        Node *node = hashtable_find_node(table, parts[1]);
        DoublyLinkedList *list;

        if (node == NULL)
        {
            list = dll_create();
            hashtable_set(table, parts[1], (const char *)list);
            node = hashtable_find_node(table, parts[1]);
            node->type  = TYPE_LIST;
            node->value = list;
        }
        else if (node->type != TYPE_LIST)
        {
            printf(ROUGE "ERREUR : la clé '%s' contient déjà une valeur string\n" RESET, parts[1]);
            return;
        }
        else
        {
            list = (DoublyLinkedList *)node->value;
        }

        int size = dll_rpush(list, parts[2]);
        printf("(integer) %d\n", size);
    }

    /* ─────────────────────────────────────────
     * LPOP key — O(1)
     * Extrait et supprime la tête de la liste
     * ───────────────────────────────────────── */
    else if (strcmp(parts[0], "LPOP") == 0)
    {
        if (count < 2)
        {
            printf(ROUGE "ERREUR : syntaxe → LPOP <clé>\n" RESET);
            return;
        }
        Node *node = hashtable_find_node(table, parts[1]);
        if (node == NULL || node->type != TYPE_LIST)
        {
            printf("(nil)\n");
            return;
        }
        char *val = dll_lpop((DoublyLinkedList *)node->value);
        if (val == NULL)
            printf("(nil)\n");
        else
        {
            printf(CYAN "\"%s\"\n" RESET, val);
            free(val); /* dll_lpop transfère la propriété à l'appelant */
        }
    }

    /* ─────────────────────────────────────────
     * RPOP key — O(1)
     * Extrait et supprime la queue de la liste
     * ───────────────────────────────────────── */
    else if (strcmp(parts[0], "RPOP") == 0)
    {
        if (count < 2)
        {
            printf(ROUGE "ERREUR : syntaxe → RPOP <clé>\n" RESET);
            return;
        }
        Node *node = hashtable_find_node(table, parts[1]);
        if (node == NULL || node->type != TYPE_LIST)
        {
            printf("(nil)\n");
            return;
        }
        char *val = dll_rpop((DoublyLinkedList *)node->value);
        if (val == NULL)
            printf("(nil)\n");
        else
        {
            printf(CYAN "\"%s\"\n" RESET, val);
            free(val);
        }
    }

    /* ─────────────────────────────────────────
     * Commande inconnue
     * ───────────────────────────────────────── */
    else
    {
        printf(ROUGE "ERREUR : commande inconnue '%s'\n" RESET, parts[0]);
    }
}