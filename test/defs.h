#ifndef DEFS_H_
#define DEFS_H_

#if __APPLE__
#define ARRAY_ALG_RANDOM(__n) (arc4random() % (__n));
#endif

#define ARRAY_LEN(a) (sizeof(a) / sizeof(a[0]))

typedef struct {
    int id;
    char name[32];
} Person;


#define ARRAY_ALG_TYPE int
#define ARRAY_ALG_PREFIX intv_
#include "../array_alg.h"

#define ARRAY_ALG_TYPE char
#define ARRAY_ALG_PREFIX string_
#include "../array_alg.h"

#define ARRAY_ALG_TYPE Person
#define ARRAY_ALG_PREFIX person_array_
#include "../array_alg.h"

#endif // DEFS_H_
