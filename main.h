#ifndef MAIN_H
#define MAIN_H
/*all libraries included*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
/*all header file included*/
#include "constants.h"
/*chaanging according to the code state*/
FLAG flag;
/*this function gets file name and checks if the file is valid*/
bool fileExists(char *src);
/*this function run each pass on given files*/
FLAG assembler(char *src, char *preAssembler, char *objectFileName, char *entryFileName, char *externFileName);
/*this function does the preassembly phase add the macrons to code*/
extern void addMacros(char *src, char *preAssembler);
/*this function is the main function of the first phase*/
extern FLAG first_pass(char *preAssembler);
/*this function is the main function of the secon pass*/
extern FLAG second_pass(char *preAssembler, char *objectFileName, char *entryFileName, char *externFileName);
#endif