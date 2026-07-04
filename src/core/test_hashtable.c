#include <stdio.h>
#include "HashTable.h"
#include "HashFunction.h"

int main(void)
{
    printf("=== Test DJB2 ===\n");
    printf("djb2(\"hello\")  = %lu\n", djb2("hello"));
    printf("djb2(\"world\")  = %lu\n", djb2("world"));
    printf("hash_index(\"hello\", 16) = %zu\n", hash_index("hello", 16));

    printf("\n=== Test HashTable ===\n");
    HashTable *table = hashtable_create(16);

    hashtable_set(table, "nom", "Lissa");
    hashtable_set(table, "role", "Membre2");

    printf("GET nom  -> %s\n", hashtable_get(table, "nom"));
    printf("GET role -> %s\n", hashtable_get(table, "role"));
    printf("GET inconnu -> %s\n",
           hashtable_get(table, "inconnu") ? hashtable_get(table, "inconnu") : "(NULL, cle absente)");

    hashtable_set(table, "nom", "Lissa2"); // update
    printf("GET nom apres update -> %s\n", hashtable_get(table, "nom"));

    int deleted = hashtable_del(table, "role");
    printf("DEL role -> %s\n", deleted ? "supprime" : "echec");
    printf("GET role apres DEL -> %s\n",
           hashtable_get(table, "role") ? hashtable_get(table, "role") : "(NULL, cle absente)");

    hashtable_destroy(table);
    return 0;
}
