//
//  main.c
//  mcfpp4
//
//  Created by Gurankio on 18/08/2020.
//  Copyright © 2020 Gurankio. All rights reserved.
//

#include <stdio.h>
#include <strings.h>

#include "stage/preprocessor.h"
#include "stage/assembler.h"
#include "stage/redirector.h"

#include "commands/commands.h"

#include "lib/vector.h"

void debug_print_tree(node * node, int level) {
    if (level == -1) goto end;
    for (int i = 0; i < level; i++) printf("   ");
    switch (node->command.command.type) {
        case ERROR:
            printf("ERROR ");
            break;
            
        case ASSEMBLER:
        case TERMINAL:
            if (node->command.command.function.terminal != NULL) printf("OK, ");
            printf("TERMINAL, ");
            break;
            
        case BUFFER:
            if (node->command.command.function.buffer != NULL) printf("OK, ");
            printf("BUFFER, ");
            break;
            
        case INTERMEDIATE:
            if (node->command.command.function.intermediate != NULL) printf("OK, ");
            printf("INTERMEDIATE, ");
            break;
    }
end: for (int i = 0; i < vector_length(node->command.params); i++) printf("%s ", node->command.params[i]);
    printf("\n");
    
    for (int i = 0; i < vector_length(node->children); i++) debug_print_tree(node->children[i], level+1);
}


int main(int argc, char * argv[]) {
    if (argc != 2) {
        printf("Usage: ./mcfpp file\n");
        return 0;
    }
    
    // Initialization
    redirector_initialize();
    char * output_file = vector_create();
    char * last_dot;
    for (last_dot = argv[1] + strlen(argv[1]) - 1; last_dot >= argv[1] && *last_dot != '.'; last_dot--);
    for (char * c = argv[1]; c < last_dot; c++) vector_push(output_file, *c);
    vector_push(output_file, (char)'.');
    vector_push(output_file, (char)'m');
    vector_push(output_file, (char)'c');
    vector_push(output_file, (char)'f');
    vector_push(output_file, (char)'u');
    vector_push(output_file, (char)'n');
    vector_push(output_file, (char)'c');
    vector_push(output_file, (char)'t');
    vector_push(output_file, (char)'i');
    vector_push(output_file, (char)'o');
    vector_push(output_file, (char)'n');
    vector_push(output_file, (char)'\0');
    redirector_push(output_file);
    commands_initialize();

    // Stages
    node * root = preprocess(argv[1], 1);
    debug_print_tree(root, -1);
        
    char * buffer = vector_create();
    variable* variables = vector_create();
    for (int i = 0; i < vector_length(root->children); i++) assemble(root->children[i], &buffer, &variables);
    
    node_destroy(root);
        
    // Termination
    vector_destroy(output_file);
    redirector_terminate();
    commands_terminate();
    return 0;
}
