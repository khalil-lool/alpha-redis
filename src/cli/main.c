#include <stdio.h>
#include <string.h>
#include "interpreter.h"

int main(void)
{
    /* ─────────────────────────────────────────
     * 1. Créer la base de données
     * 16 = nombre de cases initial du tableau
     * (le Membre 1 gère le redimensionnement)
     * ───────────────────────────────────────── */
    HashTable *table = hashtable_create(16);
    if (table == NULL)
    {
        printf("ERREUR : impossible de créer la base de données\n");
        return 1;
    }

    /* ─────────────────────────────────────────
     * 2. Message de bienvenue
     * ───────────────────────────────────────── */
    printf("α-Redis 1.0 — tapez EXIT pour quitter\n");
    printf("Commandes : SET GET DEL LPUSH RPUSH LPOP RPOP\n");
    printf("─────────────────────────────────────────────\n");

    /* ─────────────────────────────────────────
     * 3. Boucle principale
     * buffer = zone mémoire qui stocke ce que
     * l'utilisateur tape (max 256 caractères)
     * ───────────────────────────────────────── */
    char buffer[256];

    while (1)
    {
        /* Afficher le prompt */
        printf("α> ");

        /* fflush force l'affichage immédiat du prompt
         * sans attendre un saut de ligne */
        fflush(stdout);

        /* fgets lit ce que l'utilisateur tape
         * et le stocke dans buffer
         * NULL = erreur ou fin de fichier (Ctrl+D) */
        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        {
            printf("\nAu revoir !\n");
            break;
        }

        /* Supprimer le \n final laissé par fgets */
        buffer[strcspn(buffer, "\n")] = '\0';

        /* Ignorer les lignes vides */
        if (strlen(buffer) == 0) continue;

        /* ─────────────────────────────────────────
         * 4. Quitter si l'utilisateur tape EXIT
         * ───────────────────────────────────────── */
        if (strcmp(buffer, "EXIT") == 0 || strcmp(buffer, "exit") == 0)
        {
            printf("Au revoir !\n");
            break;
        }

        /* ─────────────────────────────────────────
         * 5. Passer la commande à l'interpréteur
         * ───────────────────────────────────────── */
        execute_command(table, buffer);
    }

    /* ─────────────────────────────────────────
     * 6. Libérer la mémoire avant de quitter
     * Important en C : ce qu'on malloc, on le free
     * ───────────────────────────────────────── */
    hashtable_destroy(table);

    return 0;
}