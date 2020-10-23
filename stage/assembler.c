//
//  assembler.c
//  mcfpp4
//
//  Created by Gurankio on 18/08/2020.
//  Copyright © 2020 Gurankio. All rights reserved.
//

#include "assembler.h"

#include <stdlib.h>
#include <strings.h>
#include "../lib/vector.h"
#include "../lib/tinyexpr.h"
#include "../data/context.h"
#include "../stage/redirector.h"

#include <stdio.h>

static char * string_replace_pattern(char * source, char * pattern, char * replacement) {
    char * new = vector_create();
    char * start = source, * end = source;
    while ((start = strstr(source + (start - source), pattern)) != NULL) {
        for (char * k = end; k < start; k++) vector_push(new, *k);
        vector_concat(new, replacement);
        
        start += vector_length(pattern);
        end = start;
    }
    for (char * k = end; k < source + vector_length(source); k++) vector_push(new, *k);
    return new;
}
    
static void replace(char ** params, variable * variables) {
    for (int i = 0; i < vector_length(params); i++) {
        if (strstr(params[i], "$(") == NULL) continue;
        
        for (int j = 0; j < vector_length(variables); j++) {
            if (strstr(params[i], variables[j].name) == NULL) continue;
            
            char * pattern = vector_create();
            vector_push(pattern, (char)'$');
            vector_push(pattern, (char)'(');
            vector_concat(pattern, variables[j].name);
            vector_push(pattern, (char)')');
            char * new = string_replace_pattern(params[i], pattern, variables[j].value);
            vector_destroy(pattern);

            vector_destroy(params[i]);
            params[i] = new;
        }
    }
}

static void math(char ** params, variable * variables) {
    for (int i = 0; i < vector_length(params); i++) {
        if (strstr(params[i], "?(") == NULL) continue;
        
        char * new = vector_create();
        char * start = params[i], * end = params[i];
        while ((start = strstr(params[i] + (start - params[i]), "?(")) != NULL) {
            for (char * k = end; k < start; k++) vector_push(new, *k);
            
            start += 2;
            char * expression = vector_create();
            int round_brackets = 1;
            while (round_brackets != 0) {
                if (*start == '(') round_brackets++;
                if (*start == ')') round_brackets--;
                vector_push(expression, *start);
                start++;
            }
            vector_pop(expression);
            
            long error;
            double result = te_interp(expression, &error);
            vector_destroy(expression);
                        
            char * conversion = malloc(snprintf(NULL, 0, "%.4f", result)+1);
            sprintf(conversion, "%.4f", result);
            char * walker = conversion;
            while (*walker != '\0') {
                vector_push(new, *walker);
                walker++;
            }
            free(conversion);
            
            end = start;
        }
        for (char * k = end; k < params[i] + vector_length(params[i]); k++) vector_push(new, *k);
        
        vector_destroy(params[i]);
        params[i] = new;
    }
}

void assemble(node * node, char ** parent_buffer, variable ** parent_variables) {
    char * buffer = vector_copy(*parent_buffer);
    variable * variables = vector_copy(*parent_variables);
    char ** params = vector_create();
    for (int i = 0; i < vector_length(node->command.params); i++)
        vector_push(params, vector_copy(node->command.params[i]));
    
    replace(params, variables);
    math(params, variables);
    
    command * command = &node->command.command;
    switch (command->type) {
        case ASSEMBLER:
        case INTERMEDIATE: {
            context context = {parent_buffer, parent_variables, &buffer, &variables, params, NULL};
            command->function.intermediate(&context);;
            do {
                for (int i = 0; i < vector_length(node->children); i++) {
                    assemble(node->children[i], &buffer, &variables);
                }
            } while (command->function.intermediate(&context));
            break;
        }
            
        case BUFFER: {
            char * out_buffer = command->function.buffer(params);
            vector_concat(buffer, out_buffer);
            vector_destroy(out_buffer);
            
            for (int i = 0; i < vector_length(node->children); i++) {
                assemble(node->children[i], &buffer, &variables);
            }
            break;
        }
            
        case TERMINAL: {
            char * out_terminal = command->function.terminal(params);
            vector_concat(buffer, out_terminal);
            vector_destroy(out_terminal);
            
            redirector_print(buffer);
            break;
        }
         
//        case PREPROCESSOR:
        case ERROR:
//            puts("Command Error");
            break;
    }
    
    // Cleanup
    vector_destroy(buffer);
    vector_destroy(variables);
    for (int i = 0; i < vector_length(params); i++)
        vector_destroy(params[i]);
    vector_destroy(params);
}

