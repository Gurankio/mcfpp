//
//  node.c
//  mcfpp4
//
//  Created by Gurankio on 18/08/2020.
//  Copyright © 2020 Gurankio. All rights reserved.
//

#include "node.h"

#include <stdlib.h>
#include "../lib/vector.h"

node * node_create(node * parent) {
    node * node = calloc(1, sizeof(struct node));
    node->parent = parent;
    node->children = vector_create();
    node->command.command.type = ERROR;
    node->command.params = vector_create();
    vector_push(node->command.params, vector_create());
    return node;
}

node * node_copy(node * source) {
    node * node = node_create(source->parent);
    node->command.command = source->command.command;
    node->command.params = vector_create();
    for (int i = 0; i < vector_length(source->command.params); i++) {
        char * temp = vector_copy(source->command.params[i]);
        vector_push(node->command.params, temp);
    }
    
    for (int i = 0; i < vector_length(source->children); i++) {
        vector_push(node->children, node_copy(source->children[i]))
    }
    return node;
}

void node_destroy(node * node) {
    for (unsigned long i = 0; i < vector_length(node->children); i++) node_destroy(node->children[i]);
    vector_destroy(node->children);
    for (unsigned long i = 0; i < vector_length(node->command.params); i++) vector_destroy(node->command.params[i]);
    vector_destroy(node->command.params);
    free(node);
}
