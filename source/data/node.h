//
//  node.h
//  mcfpp4
//
//  Created by Gurankio on 18/08/2020.
//  Copyright © 2020 Gurankio. All rights reserved.
//

#ifndef node_h
#define node_h

#include "command.h"

typedef struct executable_command {
    char ** params; // vector of vector
    command command;
} executable_command;

typedef struct node {
    struct node * parent;
    struct node ** children; // vector
    executable_command command;
} node;

// Helper functions

node * node_create(node * parent);
node * node_copy(node * source);
void node_destroy(node *);

//
//
//

#endif /* node_h */
