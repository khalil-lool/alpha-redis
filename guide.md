# α-Redis — Guide des commandes

Moteur de stockage NoSQL In-Memory · GLSI L3 · ESP/UCAD

---

## Compilation et lancement

```bash
# Compiler
gcc src/cli/main.c src/cli/interpreter.c \
    src/core/HashTable.c src/core/HashFunction.c \
    src/core/dynamic_array.c src/core/linked_list.c \
    src/core/double_linked_list.c \
    -o alpha-redis

# Lancer
./alpha-redis
```

---

## Commandes sur les chaînes

### SET
Stocke une valeur associée à une clé. Si la clé existe déjà, la valeur est mise à jour.

```
α-redis> SET nom Fatou
OK
α-redis> SET nom Kine
OK
```

### GET
Retourne la valeur associée à une clé. Retourne `(nil)` si la clé n'existe pas.

```
α-redis> GET nom
"Kine"
α-redis> GET inexistant
(nil)
```

### DEL
Supprime une clé. Retourne `(integer) 1` si supprimée, `(integer) 0` si la clé n'existait pas.

```
α-redis> DEL nom
(integer) 1
α-redis> DEL nom
(integer) 0
```

### EXISTS
Vérifie si une clé existe. Retourne `(integer) 1` si oui, `(integer) 0` sinon.

```
α-redis> SET ville Dakar
OK
α-redis> EXISTS ville
(integer) 1
α-redis> EXISTS pays
(integer) 0
```

---

## Commandes sur les listes

### LPUSH
Insère une valeur en **tête** de liste. Crée la liste si elle n'existe pas encore. Retourne la nouvelle taille.

```
α-redis> LPUSH fruits mangue
(integer) 1
α-redis> LPUSH fruits citron
(integer) 2
```

La liste ressemble à : `head ↔ [citron] ↔ [mangue] ↔ tail`

### RPUSH
Insère une valeur en **queue** de liste. Retourne la nouvelle taille.

```
α-redis> RPUSH fruits banane
(integer) 3
```

La liste ressemble à : `head ↔ [citron] ↔ [mangue] ↔ [banane] ↔ tail`

### LPOP
Extrait et supprime la **tête** de la liste. Retourne `(nil)` si la liste est vide.

```
α-redis> LPOP fruits
"citron"
```

### RPOP
Extrait et supprime la **queue** de la liste. Retourne `(nil)` si la liste est vide.

```
α-redis> RPOP fruits
"banane"
```

---

## Commandes utilitaires

### KEYS
Liste toutes les clés actuellement stockées dans la base.

```
α-redis> SET nom Fatou
OK
α-redis> SET age 21
OK
α-redis> KEYS
"nom"
"age"
```

### DBSIZE
Retourne le nombre total de clés stockées.

```
α-redis> DBSIZE
(integer) 2
```

### FLUSHALL
Vide complètement la base de données. Toutes les clés sont supprimées.

```
α-redis> FLUSHALL
OK
α-redis> DBSIZE
(integer) 0
```

### STATS
Affiche les statistiques internes du moteur.

```
α-redis> STATS
=== Statistiques du moteur ===
  Clés stockées     : 3
  Capacité tableau  : 16 buckets
  Facteur de charge : 18.75%
```

### DATA
Affiche toutes les clés avec leurs valeurs et leurs types.

```
α-redis> DATA
=== Données stockées ===
  fruits (list, 1 élément(s))
  age (string) : "21"
  nom (string) : "Fatou"
```

### PING
Teste si le moteur répond.

```
α-redis> PING
PONG
```

### HELP
Affiche la liste de toutes les commandes disponibles.

```
α-redis> HELP
```

### CLEAR / CLS
Nettoie l'écran du terminal.

```
α-redis> CLEAR
```

### EXIT / QUIT
Quitte le programme proprement en libérant toute la mémoire.

```
α-redis> EXIT
Au revoir !
```

---

## Session de test complète

Copie-colle cette session pour valider toutes les commandes d'un coup :

```
PING
SET nom Fatou
SET age 21
SET ville Dakar
GET nom
GET age
EXISTS ville
EXISTS pays
KEYS
DBSIZE
LPUSH fruits mangue
LPUSH fruits citron
RPUSH fruits banane
DATA
LPOP fruits
RPOP fruits
STATS
DEL nom
EXISTS nom
FLUSHALL
DBSIZE
EXIT
```

### Résultats attendus

| Commande | Résultat attendu |
|---|---|
| PING | PONG |
| SET nom Fatou | OK |
| GET nom | "Fatou" |
| EXISTS ville | (integer) 1 |
| EXISTS pays | (integer) 0 |
| DBSIZE | (integer) 3 |
| LPUSH fruits mangue | (integer) 1 |
| LPUSH fruits citron | (integer) 2 |
| RPUSH fruits banane | (integer) 3 |
| LPOP fruits | "citron" |
| RPOP fruits | "banane" |
| DEL nom | (integer) 1 |
| FLUSHALL | OK |
| DBSIZE après FLUSHALL | (integer) 0 |

---

## Complexité des opérations

| Commande | Complexité |
|---|---|
| SET | O(1) moyen |
| GET | O(1) moyen |
| DEL | O(1) moyen |
| EXISTS | O(1) moyen |
| LPUSH / RPUSH | O(1) |
| LPOP / RPOP | O(1) |
| KEYS | O(n) |
| DBSIZE | O(1) |
| FLUSHALL | O(n) |

---

## Architecture interne

```
Commande utilisateur
        │
   interpreter.c (Membre 5)
        │
   HashTable.c (Membre 2)
   ├── HashFunction.c — DJB2 → index
   ├── dynamic_array.c (Membre 1) — tableau de buckets
   ├── linked_list.c (Membre 3) — collisions par chaînage
   └── double_linked_list.c (Membre 4) — type Liste
```

---

*α-Redis 1.0 · GLSI L3 · ESP/UCAD · Juillet 2026*