//
//  command.h
//  mcfpp4
//
//  Created by Gurankio on 18/08/2020.
//  Copyright © 2020 Gurankio. All rights reserved.
//

#ifndef command_h
#define command_h

#include "context.h"

typedef enum command_type {
    ERROR,
//    PREPROCESSOR,
    ASSEMBLER, // just because assign is an exception. preprocess is treated as TERMINAL, while in assmebler as INTERMEDIATE. f**k.
    TERMINAL,
    BUFFER,
    INTERMEDIATE
} command_type;

typedef struct command {
    command_type type;
    union {
        char * (* terminal)(char ** params);
        char * (* buffer)(char ** params);
        int (* intermediate)(context * context);
    } function;
} command;

// Helper functions

//
//
//

#endif /* command_h */
