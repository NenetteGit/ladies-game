#include "data_type.h"
#include <string.h>

Vector *vector_init()
{
    return vector_init_alloc(1);
}

Vector *vector_init_alloc(size_t capacity)
{
    Vector *vector = malloc(sizeof(Vector));
    *vector = (Vector){
        .capacity = capacity,
        .length = 0,
        .items = malloc(sizeof(void *) * capacity)};

    return vector;
}

void vector_check_alloc(Vector *vector)
{
    if (vector->length >= vector->capacity)
    {
        vector->capacity += (vector->capacity < CAPACITY_LIMIT) ? vector->capacity : CAPACITY_LIMIT;
        vector->items = realloc(vector->items, (sizeof(void *) * vector->capacity));
    }
}

void vector_push_ref(Vector *vector, void *value)
{
    vector_check_alloc(vector);
    vector->items[vector->length++] = value;
}

void vector_push(Vector *vector, void *value, size_t size_mem, size_t size)
{
    void *ptr = malloc(size_mem * size);
    memcpy(ptr, value, size_mem * size);
    vector_push_ref(vector, ptr);
}

String *string_init_alloc(size_t capacity)
{
    String *string = malloc(sizeof(String));
    *string = (String){
        .capacity = capacity,
        .buffer = calloc(sizeof(char), capacity),
        .length = 0};

    return string;
}

inline String *string_init()
{
    return string_init_alloc(1);
}

String *string_init_with(const char *str)
{
    size_t len = strlen(str);
    String *string = string_init_alloc(len + 1);
    strcpy(string->buffer, str);
    string->length = len;

    return string;
}

void string_check_alloc(String *string)
{
    if (string->capacity < CAPACITY_LIMIT)
    {
        if (string->length >= string->capacity)
        {
            string->capacity *= 2;

            string->buffer = realloc(string->buffer, (sizeof(char) * string->capacity));

            memset(string->buffer + string->length, '\0', (string->capacity / 2));
        }
    }
}


Coordinate *init_coordinate(size_t x, size_t y)
{

}