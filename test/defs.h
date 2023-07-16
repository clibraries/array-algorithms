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

// Import private functions for testing. 
#define ARRAY_ALG_STATIC
#define ARRAY_ALG_IMPLEMENTATION
#define ARRAY_ALG_TYPE int
#define ARRAY_ALG_PREFIX intv_private_
#include "../array_alg.h"
#undef ARRAY_ALG_IMPLEMENTATION
#undef ARRAY_ALG_STATIC

#endif // DEFS_H_
