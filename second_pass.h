#ifndef SECOND_PASS_H
#define SECOND_PASS_H

/*libraries included*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/*headers included*/
#include "struct.h"
#include "constants.h"
extern FLAG flag;

/*change file status*/
extern FLAG error(int status);
/*get line from .am file and analize it*/
extern input_line *getLine(char currline[]);
/*creates the entry file*/
void createEntryFile(char *preAss,char *Entry);
/*creates the object file*/
void createObjectFile(char *preAss,char *obj);
/*creates the extern file*/
void createExternFile(char *preAss, char *ext);
/*checks if entry table is empty*/
extern bool isEmptyEnt();
/*return the number of machine code given line represtend in*/
extern int MachineCodeInLine(input_line *line);
/*checks if given label is entry label*/
extern bool checkIsEntLabel(char *label);
/*checks if given extern label is extern label*/
extern bool checkIsExtLabel(char *label);
/*checks if given string is valid register*/
extern bool validReg(const char *regStr);
/*checks if extern table is empty or*/
extern bool isEmptyExt();
/*gets a line and update given line with the machine code in it (the main line in the machine code)*/
void updateLine(int line[], input_line *Curline);
/*gets a string and return if its valid number*/
extern bool validNum(char *numStr);
/*gets a string label and return a opinter to the label if exist else null*/
extern label *checkIsLabel(char *label);
/*return the data from data table in given place*/
extern int getData(int place);
/*return the length of data table*/
extern int getDataAmount();
/*realese all the memory allocated in table*/
extern void realseMemory();
/*checks if all the argument in given line is valid or not*/
extern bool validArg(input_line* line,bool afterFirstScan);
/*gets int array and label and update the array to hold the machine code of label*/
void updateLabel(char *label, int line[]);
/*gets int array and number and update the array to hold the machine code of numebr*/
void updateNumber(char *number, int line[]);
/*gets int array and register and update the array to hold the machine code of register*/
void updateRegister(char *reg, int place, int line[]);
/*writes data to object file at the end*/
void writeData(char *obj, int MachineCode);
/*gets int array and two register and update the array to hold the machine code of two register*/
void updateTwoRegister(char *reg1, char *reg2, int line[]);
/*writes to object file given line and machine code*/
void writeToFile(char *obj, int line[], int currentMachineCode);
#endif