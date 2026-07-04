#include "HashFunction.h"

unsigned long djb2(const char *key)
{
    unsigned long hash = 5381;
    int c;

    // (unsigned char *) pour éviter les soucis de signe avec les
    // caractères accentués / caractères > 127
    const unsigned char *str = (const unsigned char *)key;

    while ((c = *str++))
    {
        // hash * 33 + c  <=>  ((hash << 5) + hash) + c
        hash = ((hash << 5) + hash) + (unsigned long)c;
    }

    return hash;
}

size_t hash_index(const char *key, size_t capacity)
{
    if (capacity == 0)
    {
        return 0;
    }

    unsigned long h = djb2(key);
    return (size_t)(h % (unsigned long)capacity);
}
