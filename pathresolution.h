#if !defined( PATHRESOLUTION_H )
#define PATHRESOLUTION_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>

char* BuildPath(char* str);
char* ReplaceSubStr(char* dest, size_t start, size_t end, const char* source);
int ContainsChar(const char* str, char c);
int VecContainsStr(char** argv, const char* str);
int ArraySize(char** argv);
char** ArrayPushBack(char** argv, const char* str);
char** ArrayRemoveElement(char** argv, int index);
void BigFree(char** array);

#endif
