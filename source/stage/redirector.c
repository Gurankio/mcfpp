//
//  redirector.c
//  mcfpp4
//
//  Created by Gurankio on 18/08/2020.
//  Copyright © 2020 Gurankio. All rights reserved.
//

#include "redirector.h"

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ftw.h>
#include "../lib/vector.h"

const char * OUTPUT_DIR = "./out/";

FILE ** files;


void redirector_initialize() {
    mkdir(OUTPUT_DIR, 0755);
    files = vector_create();
}

void redirector_terminate() {
    for (int i = 0; i < vector_length(files); i++) {
        fclose(files[i]);
    }
    vector_destroy(files);
}

void redirector_push(char * filename) {
    char * dir_filename = malloc(snprintf(NULL, 0, "%s%s", OUTPUT_DIR, filename)+1);
    sprintf(dir_filename, "%s%s", OUTPUT_DIR, filename);
    FILE * t = fopen(dir_filename, "w");
    if (t != NULL) { vector_push(files, t); }
    else fprintf(stdout, "Invalid filename: %s", filename);
    free(dir_filename);
}
void redirector_pop() {
    fclose(files[vector_length(files)-1]);
    vector_pop(files);
}

void redirector_print(char * string) {
    FILE * file = files[vector_length(files)-1];
    fprintf(file, "execute %s\n", string);
    fflush(file); // just to be safe...
}
