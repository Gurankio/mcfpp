//
//  vector.c
//  vector
//
//  Created by Gurankio on 15/08/2020.
//  Copyright © 2020 Gurankio. All rights reserved.
//

#include "vector.h"

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Internal

typedef struct vector_info {
    unsigned long capacity;
    unsigned long length;
    void * data;
} vector_info;

static inline vector_info * get_info(vector v) {
    // Subtract capacity and length to get the base pointer
    return v - (sizeof(unsigned long) * 2);
}

static void grow(vector * v, unsigned long type, unsigned long length) {
    vector_info * info = get_info(*v);
    if (length >= info->capacity) {
        while (length >= info->capacity) info->capacity = !info->capacity ? 1 : info->capacity * 2;
        info = realloc(info, sizeof(vector_info) + info->capacity * type);
        *v = &(info->data);
        memset(*v + (info->length * type), 0, (info->capacity - info->length) * type);
    }
}

// External

// Memory

inline vector vector_create() {
    return &((vector_info *) calloc(1, sizeof(vector_info)))->data;
}

void _vector_resize(vector * v, unsigned long type, unsigned long length) {
    grow(v, type, length);
    vector_info * info = get_info(*v);
    if (length < info->length) {
        memset(*v + (length * type), 0, (info->length - length) * type);
    }
    info->length = length;
}

inline void vector_destroy(vector v) {
    free(get_info(v));
}

// Elements

void _vector_insert(vector * v, void * data, unsigned long type, unsigned long index) {
    vector_info * info = get_info(*v);
    if (index < info->length) {
        grow(v, type, info->length + 1);
        memmove(*v + ((index + 1) * type), *v + (index * type), (get_info(*v)->length - index) * type);
        memmove(*v + (index * type), data, type);
        get_info(*v)->length++;
    } else {
        grow(v, type, index);
        memmove(*v + (index * type), data, type);
        if (index >= get_info(*v)->length) get_info(*v)->length = index + 1; // max
    }
}

void _vector_remove(vector * v, unsigned long type, unsigned long index) {
    vector_info * info = get_info(*v);
    if (index < info->length) {
        memmove(*v + (index * type), *v + ((index + 1) * type), (info->length - index) * type);
        info->length--;
    }
}

// Vector

vector _vector_copy(vector v, unsigned long type) {
    vector n = vector_create();
    _vector_resize(&n, type, vector_length(v));
    memmove(n, v, vector_length(v) * type);
    return n;
}

void _vector_concat(vector * d, vector s, unsigned long type) {
    unsigned long d_length = vector_length(*d), s_length = vector_length(s);
    _vector_resize(d, type, d_length + s_length);
    memmove(*d + (d_length * type), s, s_length * type);
}

// Utility

unsigned long vector_capacity(vector v) {
    return get_info(v)->capacity;
}

unsigned long vector_length(vector v) {
    return get_info(v)->length;
}
