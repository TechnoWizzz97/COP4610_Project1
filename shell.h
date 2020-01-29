#if !defined( EXAMPLE_H )
#define EXAMPLE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
	char** tokens;
	int numTokens;
} instruction;

void runShell(instruction* input);

#endif
