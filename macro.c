
#include "preassembler.h"
void add_string(struct Macro *macro, const char *str, int numberOfMacros)/*add string to given macro number*/
{
    while (numberOfMacros != 0)
    {
        macro = macro->next;
        numberOfMacros--;
    }
    macro->strings = realloc(macro->strings, (macro->count + 1) * sizeof(char *)); /*allocate memory for string*/
    macro->strings[macro->count] = malloc(strlen(str) + 1); 
    strcpy(macro->strings[macro->count], str);/*copys string to macro strings in avilable place*/
    macro->count++;
}/*ends of add_string*/

char* get_string(struct Macro *macro,int number)/*return string in given number in given macro*/
{
return macro->strings[number];
}/*end of get_string*/