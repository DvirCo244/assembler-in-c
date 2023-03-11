#include "preassembler.h"

void addMacros(char *src,char *preAss)
{
    FILE *assembler;
    FILE *preAssembler;
    char line[81];
    int flaginpointer = 0; 
    int numberOfMacros = 0; /*reseting the macro data structer*/
    struct Macro *macros = NULL;
    macros = (struct Macro *)malloc(sizeof(struct Macro));
    macros->strings = NULL;
    macros->count = 0;
    preAssembler = fopen(preAss, "w");
    assembler = fopen(src, "r");

    while (fgets(line, 81, assembler) != NULL)/*running through the given as file*/
    {
        if(strcmp(line,"\n") == 0){/*checking if its ignorable line*/
            continue;
        }
       else if(line[0] == ';')
        continue;
        else if (strcmp(containsMacro(line), "found end") == 0) /*macro ends*/
        {
            flaginpointer = 0;
            numberOfMacros++;
        }

        else if (flaginpointer == 1)/*in macro adds the lines in currect macro*/
        {
            add_string(macros, line, numberOfMacros);
        }

        else if (strcmp(containsMacro(line), "found start") == 0)/*found macro*/
        {
            flaginpointer = 1;
            changeName(macros, macroName(line), numberOfMacros);
        }

        else if (macroExists(macros, line, numberOfMacros) != -1)/*checks if macro exists in line if so adds the line to am file*/
        {
            int i = 0;
            int j = macroExists(macros, line, numberOfMacros);
            struct Macro *pointer = macros;
            while (j != 0)
            {
                pointer = pointer->next;
                j--;
            }
            while (i < pointer->count)
            {
                fprintf(preAssembler, "%s", *(pointer->strings + i));
                i++;
            }
        }
        else
            fprintf(preAssembler, "%s", line);
    }
    /*closing all opened file and realese the memory allocated in this part*/
    fclose(assembler);
    fclose(preAssembler);
    realeseMemoryAllocated(macros, numberOfMacros);
}/*end of addMacros*/

/*realsing memory*/
void realeseMemoryAllocated(struct Macro *macros, int numberOfMacros)
{
    int i = 0;
    while (numberOfMacros != -1)
    {
        free(macros->name);
        i = macros->count;
        while (i != -1)
        {
            free((macros->strings + i));
            i--;
        }
        macros = macros->next;
        numberOfMacros--;
    }
}/*end of realeseMemoryAllocated*/

/*checks if macro exists in given line*/
int macroExists(struct Macro *macro, char *line, int numberOfMacros)
{
    int i = 0;
    while (numberOfMacros != -1)
    {
        if (strcmp(macro->name, line) == 0)
        {
            return i;
        }
        i++;
        macro = macro->next;
        numberOfMacros--;
    }
    return -1;
}/*end of macroExists*/

/*change macro name*/
void changeName(struct Macro *macro, char name[], int numberOfMacros)
{
    while (numberOfMacros != 0)
    {
        macro = macro->next;
        numberOfMacros--;
    }
    macro->next = (struct Macro *)malloc(sizeof(struct Macro));
    macro->next->count = 0;
    macro->next->strings = NULL;
    macro->name = name;
}/*end of changeName*/

/*getting the macro name*/
char *macroName(char str[])
{
    int i;
    char *name = NULL;
    int size = 0;
    for (i = 0; i < strlen(str) - 2; i++)
    {
        if (str[i] == 'm' && str[i + 1] == 'c' && str[i + 2] == 'r')
        {
            str += 3;
            while (i < strlen(str))
            {
                if (str[i] != ' ')
                {
                    name = (char *)realloc(name, size * sizeof(char) + 1);
                    *(name + size) = str[i];
                    size++;
                }
                i++;
            }
        }
        name[size] = '\0';
    }
    return name;
}/*end of macroName*/

/*checking if a line contains a macro*/
char *containsMacro(char str[])
{
    int i;
    int mcr_found = 0;
    int non_mcr = 0;

    for (i = 0; i < strlen(str) - 2; i++)
    {
        if (mcr_found == 0 && str[i] != 'm' && str[i] != '\0')
            non_mcr = 1;
        if (str[i] == 'm' && str[i + 1] == 'c' && str[i + 2] == 'r')
        {
            mcr_found = 1;
        }
    }
    if (mcr_found == 1 && non_mcr != 1)
        return "found start";
    else if (mcr_found == 1)
        return "found end";
    else
        return " ";
}/*end of containsMacro*/