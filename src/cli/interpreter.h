#ifndef INTERPRETER_H
#define INTERPRETER_H

/* On inclut HashTable.h pour que interpreter.c
   connaisse le type HashTable et ses fonctions */
#include "../core/HashTable.h"
#include "../core/double_linked_list.h"

/*
 * execute_command :
 * Reçoit la commande brute tapée par l'utilisateur (ex: "SET nom Fatou")
 * et la base de données, exécute la commande, affiche le résultat.
 *
 * Paramètres :
 *   - table   : la base de données HashTable
 *   - command : la chaîne brute tapée par l'utilisateur
 */
void execute_command(HashTable *table, char *command);

#endif /* INTERPRETER_H */