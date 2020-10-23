//
//  redirector.h
//  mcfpp4
//
//  Created by Gurankio on 18/08/2020.
//  Copyright © 2020 Gurankio. All rights reserved.
//

#ifndef redirector_h
#define redirector_h

void redirector_initialize(void);
void redirector_terminate(void);

void redirector_push(char *);
void redirector_pop(void);

void redirector_print(char *);

#endif /* redirector_h */
