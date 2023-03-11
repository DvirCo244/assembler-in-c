#ifndef FIRST_PASS_H
#define FIRST_PASS_H
/*all libraries included*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
/*all headers included*/
#include "struct.h"
#include "constants.h"
/*the main function in first phase*/
FLAG first_pass(char *preAss);
/*define the state of the code*/
int static MachineCodeCounter = 100;
extern FLAG flag;
/*resets machine code counter*/
void resetMachineCodeCounter();
/*checks if the line has valid number of arguments*/
bool validArgumentsAmount(input_line *line);
/*return the number of lines add in ob file*/
int MachineCodeInLine(input_line *line);
/*checks if the line arguments are valid gets afterfirstscan variable to do 2 different scan after creation and fill of tables*/
bool validArg(input_line *line, bool afterFirstScan);
/*gets a line and analize it*/
static bool handleLine(input_line *line);
/*gets a line from .am file and analize it*/
extern input_line *getLine(char currline[]);
/*realeses given line memory allocated*/
extern void freeLine(input_line *line);
/*adds given symbol to extern table*/
extern bool addExt(char *symbol);
/*adds given data to data table*/
extern int addData(unsigned int data);
/*adds given symbol to entry table*/
extern bool addEnt(char *symbol);
/*changing the flag status*/
extern FLAG error(int status);
/*gets a string and checks if its valid register*/
extern bool validReg(const char *regStr);
/*gets a string and checks if its valid label*/
extern bool validLabel(const char *labelStr);
/*gets a string and checks if its valid number*/
extern bool validNum(char *numStr);
/*gets a string and copies it to dest*/
extern bool copyStr(char **dest, const char *src);
/*gets a string and checks if its opeartor*/
extern int getOp(const char **ops, const char *str, const int opsAmount);
/*checks if given label is extern*/
extern bool checkIsExtLabel(char *label);
/*checks if given label is entry*/
extern bool checkIsEntLabel(char *label);
/*checks if its label*/
extern label *checkIsLabel(char *label);
/*adds label to symbol table*/
extern bool addLabel(char *labelName, int address);
#endif