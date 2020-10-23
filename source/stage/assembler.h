//
//  assembler.h
//  mcfpp4
//
//  Created by Gurankio on 18/08/2020.
//  Copyright © 2020 Gurankio. All rights reserved.
//

#ifndef assembler_h
#define assembler_h

#include "../data/node.h"
#include "../data/variable.h"

// Gets a tree root, talks with the global Redirector.
void assemble(node * node, char ** parent_buffer, variable ** parent_variables);

#endif /* assembler_h */
