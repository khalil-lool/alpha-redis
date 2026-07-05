#include <stdio.h>
#include <string.h>
#include <strings.h>   /* strcasecmp — ignore majuscules/minuscules */
#include "interpreter.h"

/* ─────────────────────────────────────────
 * Codes couleurs ANSI
 * \033[1;36m = cyan gras   \033[1;32m = vert gras
 * \033[1;33m = jaune gras  \033[1;31m = rouge gras
 * \033[0m    = reset
 * ───────────────────────────────────────── */
#define CYAN    "\033[1;36m"
#define VERT    "\033[1;32m"
#define JAUNE   "\033[1;33m"
#define ROUGE   "\033[1;31m"
#define RESET   "\033[0m"

/* ─────────────────────────────────────────
 * Affiche la bannière de démarrage
 * ───────────────────────────────────────── */
void print_banner(void)
{
    printf("\n");
    printf(CYAN "╔════════════════════════════════════════════════════════╗\n" RESET);
    printf(CYAN "║" RESET "                                                        " CYAN "║\n" RESET);
    printf(CYAN "║" RESET "          " CYAN "α-Redis 1.0" RESET " — Moteur NoSQL In-Memory          " CYAN "║\n" RESET);
    printf(CYAN "║" RESET "                          GLSI L3 — ESP/UCAD                " CYAN "║\n" RESET);
    printf(CYAN "║" RESET "                                                        " CYAN "║\n" RESET);
    printf(CYAN "╚════════════════════════════════════════════════════════╝\n" RESET);
    printf("\n");
}

/* ─────────────────────────────────────────
 * Affiche l'aide complète
 * ───────────────────────────────────────── */
void print_help(void)
{
    printf("\n");
    printf(JAUNE "=== Commandes disponibles ===\n\n" RESET);

    printf(VERT "  Chaînes :\n" RESET);
    printf("    SET   <clé> <valeur>   — Stocker une valeur\n");
    printf("    GET   <clé>            — Récupérer une valeur\n");
    printf("    DEL   <clé>            — Supprimer une clé\n");
    printf("    EXISTS <clé>           — Vérifier si une clé existe\n\n");

    printf(VERT "  Listes :\n" RESET);
    printf("    LPUSH <clé> <valeur>   — Insérer en tête de liste\n");
    printf("    RPUSH <clé> <valeur>   — Insérer en queue de liste\n");
    printf("    LPOP  <clé>            — Extraire de la tête\n");
    printf("    RPOP  <clé>            — Extraire de la queue\n\n");

    printf(VERT "  Utilitaires :\n" RESET);
    printf("    KEYS                   — Lister toutes les clés\n");
    printf("    DBSIZE                 — Nombre de clés stockées\n");
    printf("    FLUSHALL               — Effacer toutes les données\n");
    printf("    STATS                  — Statistiques du moteur\n");
    printf("    CLEAR                  — Nettoyer l'écran\n");
    printf("    HELP                   — Afficher cette aide\n");
    printf("    EXIT / QUIT            — Quitter\n\n");
}

/* ─────────────────────────────────────────
 * Affiche les statistiques du moteur
 * ───────────────────────────────────────── */
void print_stats(HashTable *table)
{
    printf("\n");
    printf(JAUNE "=== Statistiques du moteur ===\n\n" RESET);
    printf("  Clés stockées     : %zu\n", table->size);
    printf("  Capacité tableau  : %zu buckets\n", table->array->capacity);
    printf("  Facteur de charge : %.2f%%\n",
           (double)table->size / (double)table->array->capacity * 100.0);
    printf("\n");
}

/* ─────────────────────────────────────────
 * Affiche le résultat avec la bonne couleur
 * vert  = OK
 * rouge = ERREUR
 * blanc = autre (nil, valeurs...)
 * ───────────────────────────────────────── */
void print_result(const char *result)
{
    if (result == NULL) return;

    if (strncmp(result, "ERREUR", 6) == 0)
        printf(ROUGE "%s\n" RESET, result);
    else if (strcmp(result, "OK") == 0)
        printf(VERT "%s\n" RESET, result);
    else
        printf("%s\n", result);
}

int main(void)
{
    /* ─────────────────────────────────────────
     * 1. Bannière de démarrage
     * ───────────────────────────────────────── */
    print_banner();

    /* ─────────────────────────────────────────
     * 2. Créer la base de données
     * 16 = nombre de cases initial du tableau
     * ───────────────────────────────────────── */
    HashTable *table = hashtable_create(16);
    if (table == NULL)
    {
        printf(ROUGE "ERREUR : impossible de créer la base de données\n" RESET);
        return 1;
    }

    printf("  Tapez " VERT "HELP" RESET " pour voir les commandes disponibles.\n");
    printf("  Tapez " ROUGE "EXIT" RESET " ou " ROUGE "QUIT" RESET " pour quitter.\n\n");

    /* ─────────────────────────────────────────
     * 3. Boucle principale
     * buffer = tampon de 256 caractères max
     * ───────────────────────────────────────── */
    char buffer[256];

    while (1)
    {
        /* Prompt coloré */
        printf(CYAN "α-redis> " RESET);
        fflush(stdout);

        /* Lecture de la commande */
        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        {
            printf("\n" JAUNE "Au revoir !\n" RESET);
            break;
        }

        /* Supprimer le \n final */
        buffer[strcspn(buffer, "\n")] = '\0';

        /* Ignorer les lignes vides */
        if (strlen(buffer) == 0) continue;

        /* ─────────────────────────────────────────
         * 4. Commandes spéciales gérées dans main
         * ───────────────────────────────────────── */

        /* EXIT / QUIT — insensible à la casse */
        if (strcasecmp(buffer, "EXIT") == 0 || strcasecmp(buffer, "QUIT") == 0)
        {
            printf(JAUNE "Au revoir !\n" RESET);
            break;
        }

        /* HELP */
        if (strcasecmp(buffer, "HELP") == 0)
        {
            print_help();
            continue;
        }

        /* CLEAR / CLS — nettoyer l'écran */
        if (strcasecmp(buffer, "CLEAR") == 0 || strcasecmp(buffer, "CLS") == 0)
        {
            printf("\033[2J\033[H");
            continue;
        }

        /* STATS */
        if (strcasecmp(buffer, "STATS") == 0)
        {
            print_stats(table);
            continue;
        }

        /* ─────────────────────────────────────────
         * 5. Passer la commande à l'interpréteur
         * ───────────────────────────────────────── */
        execute_command(table, buffer);
    }

    /* ─────────────────────────────────────────
     * 6. Libérer la mémoire avant de quitter
     * ───────────────────────────────────────── */
    hashtable_destroy(table);
    return 0;
}