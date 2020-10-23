//
//  context.h
//  mcfpp4
//
//  Created by Gurankio on 20/08/2020.
//  Copyright © 2020 Gurankio. All rights reserved.
//

#ifndef context_h
#define context_h

#include "../data/variable.h"

typedef struct context {
    char ** parent_buffer;
    variable ** parent_variables;
    char ** buffer;
    variable ** variables;
    char ** params;
    void * extra;
} context;

// Helper functions

//
//
//


#endif /* context_h */
