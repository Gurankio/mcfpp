//
//  assign.c
//  mcfpp4
//
//  Created by Gurankio on 24/08/2020.
//  Copyright © 2020 Gurankio. All rights reserved.
//

#include "assign.h"

#include "../lib/vector.h"
#include <strings.h>
#include <stdio.h>

int assign(context * context) {
    variable v;
    v.name = vector_copy(context->params[1]);
    v.value = vector_create();
    for (int i = 2; i < vector_length(context->params); i++) {
        vector_concat(v.value, context->params[i]);
        vector_push(v.value, (char)' ');
    }
    vector_pop(v.value);
    
    for (int i = 0; i < vector_length(*context->parent_variables); i++) {
        if (strcmp((*context->parent_variables)[i].name, v.name) == 0) {
            _vector_remove((vector *)context->parent_variables, sizeof(v), i);
        }
    }
    
    _vector_insert((vector *)context->parent_variables, &v, sizeof(v), vector_length(*context->parent_variables));
    return 0;
}
