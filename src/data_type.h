#ifndef DATA_TYPE_H
#define DATA_TYPE_H

#include <stdlib.h>

#define CAPACITY_LIMIT 2000

typedef struct string_t String;
typedef struct vector_t Vector;

struct string_t
{
    char *buffer;
    size_t capacity;
    size_t length;
};

struct vector_t
{
    void **items;
    size_t capacity;
    size_t length;
};

Vector *vector_init();

Vector *vector_init_alloc(size_t);

void vector_check_alloc(Vector *);

void vector_push_ref(Vector *, void *);

void vector_push(Vector *, void *, size_t, size_t);

String *string_init_alloc(size_t);

String *string_init_with(const char *);

void string_check_alloc(String *);


#endif