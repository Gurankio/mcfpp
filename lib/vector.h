//
//  vector.h
//  vector
//
//  Created by Gurankio on 15/08/2020.
//  Copyright © 2020 Gurankio. All rights reserved.
//

#ifndef vector_h
#define vector_h

typedef void * vector;

// Memory
vector vector_create(void);
void _vector_resize(vector *, unsigned long, unsigned long);
#define vector_resize(v, length) { _vector_resize((vector *)&v, sizeof(typeof(*v)), length); }
void vector_destroy(vector v);

// Elements
void _vector_insert(vector *, void *, unsigned long, unsigned long);
#define vector_insert(v, index, rhs) { typeof(rhs) _ = rhs; _vector_insert((vector *)&v, &_, sizeof(_), index); }
void _vector_remove(vector *, unsigned long, unsigned long);
#define vector_remove(v, index) _vector_remove((vector *)&v, sizeof(typeof(*v)), index)

// Vector
vector _vector_copy(vector, unsigned long);
#define vector_copy(v) _vector_copy(v, sizeof(typeof(*v)))
void _vector_concat(vector *, vector, unsigned long);
#define vector_concat(d, s) _vector_concat((vector *)&d, s, sizeof(typeof(*s)));

// Stack
#define vector_top(v) (v[vector_length(v)-1])
#define vector_push(v, rhs) { typeof(rhs) _ = rhs; _vector_insert((vector *)&v, &_, sizeof(_), vector_length(v)); }
#define vector_pop(v) _vector_remove((vector *)&v, sizeof(typeof(*v)), vector_length(v)-1)

// Utility
unsigned long vector_capacity(vector);
unsigned long vector_length(vector);

#endif /* vector_h */
