#ifndef HASH_FUNCTION_H
#define HASH_FUNCTION_H

#include <stdint.h>
#include <stddef.h>

/**
 * djb2 : algorithme de hachage de Daniel J. Bernstein.
 * hash = 5381
 * hash = hash * 33 + c  pour chaque caractère c de la clé
 *
 * Retourne la valeur de hachage brute (non bornée).
 */
unsigned long djb2(const char *key);

/**
 * hash_index : calcule l'index dans le tableau dynamique à partir
 * d'une clé et de la taille actuelle du tableau (capacity).
 *
 * index = djb2(key) % capacity
 */
size_t hash_index(const char *key, size_t capacity);

#endif // HASH_FUNCTION_H
