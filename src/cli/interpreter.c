#include <stdio.h>
#include <string.h>
#include "interpreter.h"

/*
 * parse_command :
 * Découpe la commande brute en parties.
 * Exemple : "SET nom Fatou" → parts[0]="SET", parts[1]="nom", parts[2]="Fatou"
 *
 * Retourne le nombre de parties trouvées.
 */
static int parse_command(char *command, char *parts[3])
{
    int count = 0;

    /* strtok découpe la chaîne à chaque espace
     * 1er appel : on passe la chaîne originale
     * Appels suivants : on passe NULL (strtok se souvient) */
    char *token = strtok(command, " \t\n");

    while (token != NULL && count < 3)
    {
        parts[count] = token;
        count++;

        /* À partir du 3e token, on prend TOUT le reste
         * (pour les valeurs avec espaces ex: "SET phrase bonjour monde") */
        if (count == 2)
        {
            /* strtok avec "\n" uniquement pour garder les espaces dans la valeur */
            token = strtok(NULL, "\n");
        }
        else
        {
            token = strtok(NULL, " \t\n");
        }
    }

    return count;
}

/*
 * execute_command :
 * Fonction principale de l'interpréteur.
 * Reçoit la commande brute, la découpe, identifie la commande
 * et appelle la bonne fonction de HashTable.
 */
void execute_command(HashTable *table, char *command)
{
    /* Tableau de 3 pointeurs pour stocker les parties de la commande
     * parts[0] = commande (SET, GET...)
     * parts[1] = clé
     * parts[2] = valeur (si besoin) */
    char *parts[3] = {NULL, NULL, NULL};

    /* On découpe la commande */
    int count = parse_command(command, parts);

    /* Si la commande est vide, on ne fait rien */
    if (count == 0) return;

    /* ─────────────────────────────────────────
     * strcmp compare deux chaînes :
     * retourne 0 si elles sont égales
     * ───────────────────────────────────────── */

    /* SET key value */
    if (strcmp(parts[0], "SET") == 0)
    {
        /* SET a besoin de 3 parties */
        if (count < 3)
        {
            printf("ERREUR : syntaxe -> SET <cle> <valeur>\n");
            return;
        }
        /* On appelle la fonction du Membre 2 */
        hashtable_set(table, parts[1], parts[2]);
        printf("OK\n");
    }

    /* GET key */
    else if (strcmp(parts[0], "GET") == 0)
    {
        if (count < 2)
        {
            printf("ERREUR : syntaxe -> GET <cle>\n");
            return;
        }
        const char *result = hashtable_get(table, parts[1]);

        /* Si result est NULL, la clé n'existe pas */
        if (result == NULL)
        {
            printf("(nil)\n");
        }
        else
        {
            printf("%s\n", result);
        }
    }

    /* DEL key */
    else if (strcmp(parts[0], "DEL") == 0)
    {
        if (count < 2)
        {
            printf("ERREUR : syntaxe -> DEL <cle>\n");
            return;
        }
        /* hashtable_del retourne 1 si supprimé, 0 si clé inexistante */
        int deleted = hashtable_del(table, parts[1]);
        printf("(%d)\n", deleted);
    }

    /* LPUSH key value */
    else if (strcmp(parts[0], "LPUSH") == 0)
    {
        if (count < 3)
        {
            printf("ERREUR : syntaxe -> LPUSH <cle> <valeur>\n");
            return;
        }
        /* TODO : brancher sur le Membre 4 après merge */
        printf("OK\n");
    }

    /* RPUSH key value */
    else if (strcmp(parts[0], "RPUSH") == 0)
    {
        if (count < 3)
        {
            printf("ERREUR : syntaxe -> RPUSH <cle> <valeur>\n");
            return;
        }
        /* TODO : brancher sur le Membre 4 après merge */
        printf("OK\n");
    }

    /* LPOP key */
    else if (strcmp(parts[0], "LPOP") == 0)
    {
        if (count < 2)
        {
            printf("ERREUR : syntaxe -> LPOP <cle>\n");
            return;
        }
        /* TODO : brancher sur le Membre 4 après merge */
        printf("(nil)\n");
    }

    /* RPOP key */
    else if (strcmp(parts[0], "RPOP") == 0)
    {
        if (count < 2)
        {
            printf("ERREUR : syntaxe -> RPOP <cle>\n");
            return;
        }
        /* TODO : brancher sur le Membre 4 après merge */
        printf("(nil)\n");
    }

    /* Commande inconnue */
    else
    {
        printf("ERREUR : commande inconnue '%s'\n", parts[0]);
    }
}