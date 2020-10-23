//
//  common.c
//  mcfpp4
//
//  Created by Gurankio on 24/08/2020.
//  Copyright © 2020 Gurankio. All rights reserved.
//

#include "common.h"

#include "../lib/vector.h"

char * join_space(char ** params) {
    char * out = vector_create();
            
    for (int i = 0; i < vector_length(params); i++) {
        if (vector_length(params[i]) > 0) {
            vector_concat(out, params[i]);
            vector_push(out, (char)' ');
        }
    }
    
    return out;
}

char * run_join_space(char ** params) {
    char * out = vector_create();
    vector_push(out, (char)'r');
    vector_push(out, (char)'u');
    vector_push(out, (char)'n');
    vector_push(out, (char)' ');
    vector_concat(out, join_space(params));
    return out;
}
