//
//  preprocessor.c
//  mcfpp4
//
//  Created by Gurankio on 18/08/2020.
//  Copyright © 2020 Gurankio. All rights reserved.
//

#include "preprocessor.h"

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <strings.h>
#include "../lib/vector.h"
#include "../commands/commands.h"

typedef struct named_node {
    char * name;
    node * node;
} named_node;

named_node * functions;

static node * parse(char c, node * current) {
    static int content = 0, trim = 0, comment = 0;
    static int round_bracket = 0, square_bracket = 0;
        
    // Trim
    if (trim) {
        if (isspace(c)) return current;
        else trim = 0;
    }
    
    // Comment Handling
    if (comment) {
        if (c == '\n') comment = 0;
        else return current;
    }
    
    // If the current node has no child create one. REWRITE: check for an empty child?
    if (vector_length(current->children) == 0) {
        // If we are a buffer, pass down our parent, allows buffer chaining.
        vector_push(current->children, node_create(current->command.command.type == BUFFER ? current->parent : current));
    }
    
    char ** params = (vector_top(current->children))->command.params; // vec of vec of char.
    
    switch (c) {
        case '#':
            if (round_bracket == 0 && square_bracket == 0) {
                comment = 1;
                return current; // Do not add to params.
            }
            break;
            
        case '(':
            round_bracket++;
            break;
            
        case ')':
            round_bracket--;
            break;
            
        case '[':
            square_bracket++;
            break;
            
        case ']':
            square_bracket--;
            break;
            
        case '{':
            vector_pop(params);
            (vector_top(current->children))->command.params = params; // Propagate changes to struct. it might have been reallocated.
            
        case '\n': {
            trim = 1;
                        
            if (content && vector_length(params) > 0) {
                content = 0;

                // Handle 'include'
                if (strcmp(params[0], "include") == 0) {
                    node * node = preprocess(params[1], 0);
                    if (node->command.command.type != ERROR) vector_top(current->children) = node;
                    else vector_pop(current->children);
                    vector_push(current->children, node_create(current)); // New Child
                    return current;
                }
                
                // Handle 'function'
                if (strcmp(params[0], "function") == 0) {
                    vector_push(functions, ((named_node){ vector_copy(params[1]), node_create(current)}));
                    vector_pop(vector_top(functions).node->command.params);
                    for (int i = 2; i < vector_length(params); i++) vector_push(vector_top(functions).node->command.params, vector_copy(params[i]));
                    node_destroy(vector_top(current->children));
                    vector_pop(current->children);
                    return vector_top(functions).node;
                }
                
                // Copy Information
                command command = commands_get(params[0]);
                
                // Handle 'function'
                if (command.type == ERROR) {
                    for (int i = 0; i < vector_length(functions); i++) {
                        if (strcmp(functions[i].name, params[0]) == 0) {
                            
                            vector_pop(current->children);
                            
                            for (int j = 0; j < vector_length(functions[i].node->command.params); j++) {
                                node * node = node_create(current);
                                
                                node->command.command = commands_get("assign");
                                
                                vector_push(node->command.params[0], (char)'a');
                                vector_push(node->command.params[0], (char)'s');
                                vector_push(node->command.params[0], (char)'s');
                                vector_push(node->command.params[0], (char)'i');
                                vector_push(node->command.params[0], (char)'g');
                                vector_push(node->command.params[0], (char)'n');

                                vector_push(node->command.params, vector_copy(functions[i].node->command.params[j]));
                                vector_push(node->command.params, vector_copy(params[1 + j]));

                                vector_push(current->children, node);
                            }
                                
                            for (int j = 0; j < vector_length(functions[i].node->children); j++) {
                                vector_push(current->children, node_copy(functions[i].node->children[j]));
                            }
                            
                            vector_push(current->children, node_create(current)); // New Child
                            return current;
                        }
                    }
                }
                
                (vector_top(current->children))->command.command = command;
                
                // Return active node
                switch (command.type) {
                    case ERROR:
                        printf("%s, ERROR\n", params[0]);
                        return current;
                                                
                    case ASSEMBLER:
                    case TERMINAL:
                        vector_push(current->children, node_create(current));
                        return current; // If it's terminal stay where we are.
                        
                    case BUFFER:
                    case INTERMEDIATE:
                        return vector_top(current->children); // If it's a buffer / intermediate go there.
                }
            }
            return current;  // Do not add to params.
        }
            
        case '}':
            vector_pop(current->children);
            vector_push(current->parent->children, node_create(current->parent));
            return current->parent; // Do not add to params.
            
        case ' ':
            trim = 1;
            if (round_bracket == 0 && square_bracket == 0) {
                vector_push(params, vector_create());
                (vector_top(current->children))->command.params = params; // Propagate changes to struct. it might have been reallocated.
                return current; // Do not add to params.
            }
            break;
            
        default:
            content = 1;
            break;
    }
    
    vector_push(vector_top(params), c);
    (vector_top(current->children))->command.params = params; // Propagate changes to struct. it might have been reallocated.
    return current;
}

// Returns the tree node.
node * preprocess(char * filename, int main_file) {
    node * root = node_create(NULL);
    if (main_file) functions = vector_create();
    
    // File
    FILE * file = fopen(filename, "r");
    if (file == NULL) return root; // exit(-1);
    
    int c;
    node * last = root;
    while ((c = fgetc(file)) != EOF) last = parse(c, last);
    vector_pop(root->children); // Clear root children
    
    // Cleanup
    fclose(file);
    if (main_file) vector_destroy(functions);
    return root;
}
