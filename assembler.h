#ifndef ASSEMBLER_H
#define ASSEMBLER_H
/*libraries included*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
/*headers included*/
#include "struct.h"
#include "constants.h"
/*main function of assembler gets line and analize it*/
input_line *getLine(char currline[]);
/*realese memory for given line*/
void freeLine(input_line *line);
/*file state variable*/
extern FLAG flag;
/*change status flag*/
FLAG error(int status);
/*gets string and return if string is valid register*/
bool validReg(const char *regStr);
/*gets label and return if its valid label*/
bool validLabel(const char *labelStr);
/*get string and copy it to given destantion*/
bool copyStr(char **dest, const char *src);
/*return the opeartion given string is*/
int getOp(const char **ops, const char *str, const int opsAmount);
/*arguments*/
static bool getNextArg(char *src, char *dest);
/*gets string and return if its valid number*/
bool validNum(char *numStr);
/*gets line and checks if the line has valid amount of arguments*/
bool validArgumentsAmount(input_line *line);
/*return the number of lines add in obj file from givn line*/
int MachineCodeInLine(input_line *line);
#endif