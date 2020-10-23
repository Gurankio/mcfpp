//
//  file.c
//  mcfpp4
//
//  Created by Gurankio on 26/08/2020.
//  Copyright © 2020 Gurankio. All rights reserved.
//

#include "file.h"

#include <stdlib.h>
#include "../lib/vector.h"
#include "../stage/redirector.h"

int file(context * context) {
    if (context->extra == NULL) {
        context->extra = malloc(1);
        vector_push(context->params[1], (char)'.');
        vector_push(context->params[1], (char)'m');
        vector_push(context->params[1], (char)'c');
        vector_push(context->params[1], (char)'f');
        vector_push(context->params[1], (char)'u');
        vector_push(context->params[1], (char)'n');
        vector_push(context->params[1], (char)'c');
        vector_push(context->params[1], (char)'t');
        vector_push(context->params[1], (char)'i');
        vector_push(context->params[1], (char)'o');
        vector_push(context->params[1], (char)'n');
        redirector_push(context->params[1]);
    } else {
        redirector_pop();
        free(context->extra);
    }
    return 0;
}
