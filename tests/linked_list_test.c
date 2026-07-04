/* ============================================================
 *  α-Redis — Test manuel : LinkedList / Node
 *  Membre 3 — A retirer ou déplacer dans un dossier tests/
 *  avant la remise finale.
 * ============================================================ */

#include <stdio.h>
#include "linked_list.h"

int main(void) {
    printf("==================================================\n");
    printf("  Tests de la Liste Chainee (collisions) - a-Redis \n");
    printf("==================================================\n\n");

    LinkedList *bucket = ll_create();

    printf("--- Test 1 : insertion simple ---\n");
    ll_insert(bucket, "nom", TYPE_STRING, "Maty");
    ll_print(bucket);

    printf("\n--- Test 2 : collision simulee (meme case du tableau) ---\n");
    /* On imagine que hash("nom") == hash("age") -> meme bucket */
    ll_insert(bucket, "age", TYPE_STRING, "22");
    ll_print(bucket);

    printf("\n--- Test 3 : recherche ---\n");
    Node *found = ll_search(bucket, "nom");
    printf("nom -> %s\n", found ? (char *)found->value : "(non trouve)");
    found = ll_search(bucket, "ville");
    printf("ville -> %s\n", found ? (char *)found->value : "(non trouve)");

    printf("\n--- Test 4 : mise a jour d'une cle existante ---\n");
    int result = ll_insert(bucket, "nom", TYPE_STRING, "Ndeye Maty");
    printf("Nouvelle entree ? %s\n", result == 1 ? "oui" : "non (mise a jour)");
    ll_print(bucket);

    printf("\n--- Test 5 : suppression ---\n");
    int deleted = ll_delete(bucket, "age");
    printf("Suppression de 'age' reussie ? %s\n", deleted ? "oui" : "non");
    ll_print(bucket);

    deleted = ll_delete(bucket, "age");
    printf("Suppression de 'age' a nouveau ? %s\n", deleted ? "oui" : "non");

    printf("\n--- Test 6 : taille et vide ---\n");
    printf("Taille actuelle : %d\n", ll_size(bucket));
    ll_delete(bucket, "nom");
    printf("Liste vide apres tout supprimer ? %s\n", ll_is_empty(bucket) ? "oui" : "non");

    printf("\n--- Test 7 : maillon de type TYPE_LIST (simulation Membre 4) ---\n");
    /* On simule un pointeur factice vers une DoublyLinkedList
       (juste pour montrer que le typage fonctionne, sans dereferencer) */
    int fake_list_marker = 42;
    ll_insert(bucket, "mescourses", TYPE_LIST, &fake_list_marker);
    ll_print(bucket);
    found = ll_search(bucket, "mescourses");
    printf("Type de 'mescourses' : %s\n", found->type == TYPE_LIST ? "TYPE_LIST" : "TYPE_STRING");

    ll_free(bucket);
    printf("\n[OK] Tous les tests termines, memoire liberee.\n");
    return 0;
}
