#ifndef TABLES_H
#define TABLES_H
/*libaries included*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
/*headers included*/
#include "struct.h"
#include "constants.h"

/*adds data to data table*/
int addData(unsigned int data);
/*gets label name and address and adds it to symbol table*/
bool addLabel(char *labelName, int address);
/*define the status of the file*/
extern FLAG error(int status);
/*adds string to extern table*/
bool addExt(char *symbol);
/*adds string to entry table*/
bool addEnt(char *symbol);
/*checks if entry table is empty*/
bool isEmptyEnt();
/*checks if given string is extern label*/
bool checkIsEntLabel(char *label);
/*checks if given string is entry label*/
bool checkIsExtLabel(char *label);
/*returns the data from data table in given location*/
int getData(int place);
/*returns the length fo data table*/
int getDataAmount();
/*realese all memory allocated in the tables*/
void realseMemory();
/*checks if extern table is empty*/
bool isEmptyExt();
/*saved words*/
    const char *constStrings[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "stop", ".string", ".data", ".extern", ".entry"};
#endif