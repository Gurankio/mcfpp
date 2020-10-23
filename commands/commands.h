//
//  commands.h
//  mcfpp4
//
//  Created by Gurankio on 18/08/2020.
//  Copyright © 2020 Gurankio. All rights reserved.
//

#ifndef commands_h
#define commands_h

#include "../data/command.h"

void commands_initialize(void);
void commands_terminate(void);

command commands_get(char *);

#endif /* commands_h */
