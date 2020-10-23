//
//  range.c
//  mcfpp4
//
//  Created by Gurankio on 26/08/2020.
//  Copyright © 2020 Gurankio. All rights reserved.
//

#include "range.h"

#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include "../lib/vector.h"
#include "../lib/tinyexpr.h"

typedef struct range_data {
    char * variable_name;
    int current;
    int step;
    int end;
} range_data;

int range(context * context) {
    range_data * data;
    if (context->extra == NULL) {
        context->extra = malloc(sizeof(range_data));
        
        int variable_index = 1, current_index = 2, step_index, end_index;
        switch (vector_length(context->params)) {
            case 4:
                step_index = -1; end_index = 3;
                break;
            case 5:
                step_index = 3; end_index = 4;
                break;
                
            default:
                return 0;
        }

        data = context->extra;
        data->variable_name = vector_copy(context->params[variable_index]);
                
        long error;
        int current = te_interp(context->params[current_index], &error);
        if (error != 0) current = 0;
        data->current =  current;
        
        int end = te_interp(context->params[end_index], &error);
        if (error != 0) end = current;
        data->end = end;
        
        if (step_index != -1) {
            int step = te_interp(context->params[step_index], &error);
            if (error != 0) step = end - current;
            data->step = step;
        } else {
            data->step = 1;
        }
    } else {
        data = context->extra;
    }
    
    if (data->step > 0 ? data->current >= data->end : data->current <= data->end) {
        vector_destroy(data->variable_name);
        free(context->extra);
        return 0;
    }
    
    int found = 0;
    for (int i = 0; i < vector_length(*context->variables); i++) {
        if (strcmp((*context->variables)[i].name, data->variable_name) == 0) {
            vector_resize((*context->variables)[i].value, snprintf(NULL, 0, "%d", data->current));
            sprintf((*context->variables)[i].value, "%d", data->current);
            found = 1;
            break;
        }
    }
    
    if (!found) {
        variable v;
        v.name = data->variable_name;
        v.value = vector_create();
        vector_resize(v.value, snprintf(NULL, 0, "%d", data->current));
        sprintf(v.value, "%d", data->current);
        _vector_insert((vector *)context->variables, &v, sizeof(v), vector_length(*context->variables));
    }
    
    data->current += data->step;
    return 1;
}
