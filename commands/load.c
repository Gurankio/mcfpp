//
//  load.c
//  mcfpp4
//
//  Created by Gurankio on 26/08/2020.
//  Copyright © 2020 Gurankio. All rights reserved.
//

#include "load.h"

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <strings.h>
#include "../lib/vector.h"

typedef struct load_data {
    FILE * file;
    char ** variables_names;
} load_data;

int load(context * context) {
    load_data * data;
    if (context->extra == NULL) {
        context->extra = malloc(sizeof(load_data));
        data = context->extra;
        
        data->file = fopen(context->params[1], "r");
        if (data->file == NULL) exit(-1);
        
        data->variables_names = vector_create();
        vector_push(data->variables_names, vector_create());
        
        int trim = 0;
        char c;
        while ((c = fgetc(data->file)) != EOF && c != '\n') {
            if (trim) {
                if (!isspace(c)) trim = 0;
                else continue;
            }
            
            switch (c) {
                case ' ':
                    trim = 1;
                    break;
                
                case ',':
                    vector_push(data->variables_names, vector_create());
                    break;
                    
                default:
                    vector_push(vector_top(data->variables_names), c);
                    break;
            }
        }
    } else {
        data = context->extra;
    }
    
    char ** variables_values = vector_create();
    vector_push(variables_values, vector_create());

    int trim = 0;
    char c;
    while ((c = fgetc(data->file)) != EOF && c != '\n') {
        if (trim) {
            if (!isspace(c)) trim = 0;
            else continue;
        }
        
        switch (c) {
            case ' ':
                trim = 1;
                break;
            
            case ',':
                vector_push(variables_values, vector_create());
                break;
                
            default:
                vector_push(vector_top(variables_values), c);
                break;
        }
    }
    
    if (c == EOF || vector_length(variables_values) != vector_length(data->variables_names)) {
//        for (int j = 0; j < vector_length(data->variables_names); j++) {
//            for (int i = 0; i < vector_length(*context->variables); i++) {
//                if (strcmp((*context->variables)[i].name, data->variables_names[j]) == 0) {
//                    vector_destroy((*context->variables)[i].value);
//                    (*context->variables)[i].value = variables_values[j];
//                    found = 1;
//                    break;
//                }
//            }
//        }
        
        
        
        for (int i = 0; i < vector_length(data->variables_names); i++) {
            vector_destroy(data->variables_names[i]);
        }
        
        vector_destroy(data->variables_names);
        free(context->extra);
        return 0;
    }
    
    for (int j = 0; j < vector_length(data->variables_names); j++) {
        int found = 0;
        for (int i = 0; i < vector_length(*context->variables); i++) {
            if (strcmp((*context->variables)[i].name, data->variables_names[j]) == 0) {
                vector_destroy((*context->variables)[i].value);
                (*context->variables)[i].value = variables_values[j];
                found = 1;
                break;
            }
        }
        
        if (!found) {
            variable v;
            v.name = data->variables_names[j];
            v.value = variables_values[j];
            _vector_insert((vector *)context->variables, &v, sizeof(v), vector_length(*context->variables));
        }
    }
    return 1;
}
