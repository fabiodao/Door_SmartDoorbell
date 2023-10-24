#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h> // perror
#include <stdlib.h> // abort
#include "debug.h"

void panic(char *msg);
#define panic(m)	{perror(m); DEBUG_MSG(m); abort();}

#endif // !__UTILS_H__