#ifndef PREASSEMBLER_H
#define PREASSEMBLER_H
/*all libraries*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/*all headers file included*/
#include "struct.h"
#include "constants.h"
/*the main functions get the file name  and the preassembli file name*/
void addMacros(char *src,char *preAss);
/*checks if the macro exists*/
int macroExists(struct Macro *macro, char *line, int numberOfMacros);
/*check if a line contains macro or not*/
char* containsMacro(char str[]);
/*add string to given macro*/
void add_string(struct Macro *macro, const char *str, int numberOfMacros);
/*change macro name*/
    void changeName(struct Macro *macro, char name[], int numberOfMacros);
/*return the macro name*/
char* macroName(char str[]);
/*realese all the memory allocated in this phase*/
void realeseMemoryAllocated(struct Macro *macros, int numberOfMacros);
#endif