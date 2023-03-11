#include "second_pass.h"
/*main function creates all three files .ext .ent .ob*/
FLAG second_pass(char *preAssembler, char *objectFileName, char *entryFileName, char *externFileName)
{
    createEntryFile(preAssembler, entryFileName);   /*creates entry file*/
    createExternFile(preAssembler, externFileName); /*creates extern file*/
    createObjectFile(preAssembler, objectFileName); /*create object file*/
    return flag;                                    /*return the status of current file*/
} /*end of second_pass*/

/*gets line and update given array with the machine code of main line*/
void updateLine(int line[], input_line *Curline)
{
    int i;
    int command = Curline->cmd;
    int numberOfArgs = 0;
    for (i = 0; i < 14; i++) /*reset array*/
    {
        line[i] = 0;
    }
    /*updateing 0->1*/
    line[0] = line[1] = 0;
    /*updateing bits 6->9*/
    for (i = 3; i >= 0; i--)
    {
        line[9 - i] = command & 1;
        command = command >> 1;
    }
    if (Curline->cmd == STOP || Curline->cmd == RTS)
        return;

    /*updating bits 2->5*/
    while (Curline->args[numberOfArgs] != NULL)
    {
        numberOfArgs++;
    }
    if (numberOfArgs == 3)
    {
        line[3] = 1;
        line[2] = line[4] = line[5] = 0;
    }
    else
    {
        if (numberOfArgs != 1)
        {
            if (Curline->args[0] != NULL)
            {
                if (validReg(Curline->args[0]) == true)
                {
                    line[4] = 1;
                    line[5] = 1;
                }
                else if (validNum(Curline->args[0]) == true)
                {
                    line[4] = line[5] = 0;
                }
                else
                {
                    line[4] = 1;
                    line[5] = 0;
                }
            }
            if (Curline->args[1] != NULL)
            {
                if (validReg(Curline->args[1]) == true)
                {
                    line[2] = line[3] = 1;
                }
                else if (validNum(Curline->args[1]) == true)
                {
                    line[2] = line[3] = 0;
                }
                else
                {
                    line[2] = 1;
                    line[3] = 0;
                }
            }
        }
        else
        {
            if (validReg(Curline->args[0]) == true)
            {
                line[2] = 1;
                line[3] = 1;
            }
            else if (validNum(Curline->args[0]) == true)
            {
                line[2] = line[3] = 0;
            }
            else
            {
                line[2] = 1;
                line[3] = 0;
            }
        }
    }
    /*updateing 10->13*/
    if (numberOfArgs != 3)
    {
        line[10] = line[11] = line[12] = line[13] = 0;
    }
    else
    {
        if (validNum(Curline->args[1]) == true)
        {
            line[13] = line[12] = 0;
        }
        else if (validReg(Curline->args[1]) == true)
        {
            line[13] = line[12] = 1;
        }
        else
        {
            line[13] = 0;
            line[12] = 1;
        }
        if (validNum(Curline->args[2]) == true)
        {
            line[11] = line[10] = 0;
        }
        else if (validReg(Curline->args[2]) == true)
        {
            line[11] = line[10] = 1;
        }
        else
        {
            line[11] = 0;
            line[10] = 1;
        }
    }
} /*end of update line*/

/*update line with the machine code of label*/
void updateLabel(char *label, int line[])
{
    int i;
    if (checkIsLabel(label) != NULL)
    {
        for (i = 11; i >= 0; i--)
        {
            line[2 + i] = checkIsLabel(label)->address >> i & 1;
        }
        line[0] = 0;
        line[1] = 1;
    }
    else
    {
        line[0] = 1;
        line[13] = line[12] = line[11] = line[10] = line[9] = line[8] = line[7] = line[6] = line[5] = line[4] = line[3] = line[2] = line[1] = 0;
    }
} /*end of updateLabel*/

/*update line with the machine code of number*/
void updateNumber(char *number, int line[])
{
    int check = 0;
    int num;
    int i;
    bool isNegative = false;
    if (strchr(number, '-') != NULL)
        isNegative = true;
    if (number[check] == '#')
    {
        check++;
    }
    if (number[check] == '+' || number[check] == '-')
        check++;
    num = atoi((number + check));
    if (isNegative == true)
        num = ~num + 1;
    for (i = 11; i >= 0; i--)
    {
        line[i + 2] = num >> i & 1;
    }
    line[0] = line[1] = 0;
} /*end of updateNumber*/

/*writes to object file all the data at the end of file*/
void writeData(char *obj, int MachineCode)
{
    FILE *objectfile;
    int i = getDataAmount();
    char cmcStr[4];
    int x = 0;
    int num = 0;
    int j = 0;
    objectfile = fopen(obj, "a");
    if (i == -1)
        return;
    else
    {
        while (x != i)
        {
            sprintf(cmcStr, "%d", MachineCode++);
            fputs(cmcStr, objectfile);
            fputs("         ", objectfile);
            num = getData(x++);
            for (j = 13; j >= 0; j--)
            {
                if (((num >> j) & 1) == 1)
                    fputc('/', objectfile);
                else
                    fputc('.', objectfile);
            }
            fputc('\n', objectfile);
        }
    }
    fclose(objectfile);
} /*end of writeData*/

/*update line with the machine code of register*/
void updateRegister(char *reg, int place, int line[])
{
    int i;
    int num;
    num = atoi(reg + 1);
    if (place == 0)
    {
        for (i = 5; i >= 0; i--)
        {
            line[(8 + i)] = ((num >> i) & 1);
        }
        line[0] = line[1] = line[2] = line[3] = line[4] = line[5] = line[6] = line[7] = 0;
    }
    else
    {
        line[13] = line[12] = line[11] = line[10] = line[9] = line[8] = line[0] = line[1] = 0;
        for (i = 5; i >= 0; i--)
        {
            line[i + 2] = num >> i & 1;
        }
    }
} /*end of updateRegister*/

/*update line with the machine code of two registers*/
void updateTwoRegister(char *reg1, char *reg2, int line[])
{
    int i;
    int num;
    num = atoi(reg1 + 1);
    for (i = 13; i >= 0; i--)
        line[i] = 0;
    for (i = 5; i >= 0; i--)
    {
        line[8 + i] = num >> i & 1;
    }
    num = atoi(reg2 + 1);
    for (i = 5; i >= 0; i--)
    {
        line[2 + i] = num >> i & 1;
    }
    line[0] = line[1] = 0;
} /*end of updateTwoRegister*/

/*write to object file the given line and machine code*/
void writeToFile(char *obj, int line[], int currentMachineCode)
{
    FILE *objectFile;
    int i;
    char cmcStr[4];
    sprintf(cmcStr, "%d", currentMachineCode);
    objectFile = fopen(obj, "a");
    fputs(cmcStr, objectFile);
    fputs("         ", objectFile);
    for (i = 13; i >= 0; i--)
    {
        if (line[i] == 0)
            fputc('.', objectFile);
        else
            fputc('/', objectFile);
    }
    fputc('\n', objectFile);
    fclose(objectFile);
} /*end of writeToFile*/

/*creates object file*/
void createObjectFile(char *preAss, char *obj)
{
    input_line *line;
    char curLine[81];
    int MachineCodeLine[14];
    FILE *assembler;
    FILE *objectFile;
    int currentMachineCode = 100;
    int MachineCode[14];
    assembler = fopen(preAss, "r");
    objectFile = fopen(obj, "w");
    fputs("Base 10 address  Base 2 code\n\n\n", objectFile);
    fclose(objectFile);
    while (fgets(curLine, 81, assembler) != NULL) /*scan all the .am file for each line*/
    {
        line = getLine(curLine);                                                                                                                                         /*gets line */
        if (line->cmd != DOT_ENTRY && line->cmd != DOT_EXTERN && line->cmd != DOT_DATA && line->cmd != DOT_STRING && line->cmd != STOP && validArg(line, true) == false) /*checks if line argument are valid ignore non realtble op*/
        {
            error(0);
        }
        /*skip non machine code lines nor data lines that thier values added at end*/
        if (line->cmd == DOT_EXTERN || line->cmd == DOT_ENTRY || line->cmd == DOT_DATA || line->cmd == DOT_STRING)
            continue;

        updateLine(MachineCodeLine, line);                       /*update first line*/
        writeToFile(obj, MachineCodeLine, currentMachineCode++); /*writes the first line of given line*/
        if (line->cmd == STOP || line->cmd == RTS)               /*continue if lines dont have arguments*/
            continue;

        if (MachineCodeLine[2] == 0 && MachineCodeLine[3] == 1) /*if there are three opeartor*/
        {
            updateLabel(line->args[0], MachineCode);             /*update first argument*/
            writeToFile(obj, MachineCode, currentMachineCode++); /*write first argument*/
            if (validReg(line->args[1]) && validReg(line->args[2]) && validReg(line->args[1]) == true)
            { /*check if the last two arguments are register and write them*/
                updateTwoRegister(line->args[1], line->args[2], MachineCode);
                writeToFile(obj, MachineCode, currentMachineCode++);
            }
            else
            { /*analize each argument to know how to fill the machine code array and write it to file*/
                if (validNum(line->args[1]))
                    updateNumber(line->args[1], MachineCode);
                else if (validReg(line->args[1]))
                    updateRegister(line->args[1], 0, MachineCode);
                else
                    updateLabel(line->args[1], MachineCode);

                writeToFile(obj, MachineCode, currentMachineCode++);

                if (validNum(line->args[2]))
                    updateNumber(line->args[2], MachineCode);
                else if (validReg(line->args[2]))
                    updateRegister(line->args[2], 1, MachineCode);
                else
                    updateLabel(line->args[2], MachineCode);

                writeToFile(obj, MachineCode, currentMachineCode++);
            }
        }
        else if (validReg(line->args[0]) && validReg(line->args[1]) && validReg(line->args[0]) == true)
        { /*check if the two arguments are registers and write them*/
            updateTwoRegister(line->args[0], line->args[1], MachineCode);
            writeToFile(obj, MachineCode, currentMachineCode++);
        }
        else
        { /*analize each argument to know how to fill the machine code array and write it to file*/
            if (line->args[0] != NULL)
            {
                if (validNum(line->args[0]))
                    updateNumber(line->args[0], MachineCode);
                else if (validReg(line->args[0]))
                    updateRegister(line->args[0], 0, MachineCode);
                else
                    updateLabel(line->args[0], MachineCode);
                writeToFile(obj, MachineCode, currentMachineCode++);
            }
            if (line->args[1] != NULL)
            {
                if (validNum(line->args[1]))
                    updateNumber(line->args[1], MachineCode);
                else if (validReg(line->args[1]))
                    updateRegister(line->args[1], 1, MachineCode);
                else
                    updateLabel(line->args[1], MachineCode);
                writeToFile(obj, MachineCode, currentMachineCode++);
            }
        }
    }
    writeData(obj, currentMachineCode);
    realseMemory();
    fclose(assembler);
} /*end of createObjectFile*/

/*create extern file*/
void createExternFile(char *preAss, char *ext)
{
    input_line *line;
    char *c;
    char curLine[81];
    FILE *assembler;
    FILE *externfile;
    bool found = false;
    int MachineCode = 100;
    int i = 0;
    if (isEmptyExt() == false)
        return;
    assembler = fopen(preAss, "r");
    externfile = fopen(ext, "w");
    while (fgets(curLine, 81, assembler) != NULL) /*scan each line in .am file*/
    {
        line = getLine(curLine);
        switch (line->cmd) /*checks each arguments depents on cmd for extern file and increase the machine code depnts for file*/
        {
        case DOT_ENTRY: /*dont of arguments that can be extern*/
        case DOT_EXTERN:
            break;
        case DOT_DATA:
            i = 1;
            MachineCode++;
            while (line->args[i++] != NULL)
            {
                MachineCode++;
            }
            break;
        case DOT_STRING:
            MachineCode++;
            c = line->args[0];
            if (*c++ == '"')
            {
                while (*c++ != '"')
                    MachineCode++;
            }
            break;
        case STOP:
        case RTS:
            MachineCode--;
        default:
            MachineCode++;
            if (line->args[0] != NULL)
            {
                if (line->args[1] != NULL)
                {
                    if (line->args[2] != NULL) /*three arguments command*/
                    {
                        if (checkIsExtLabel(line->args[0]) == true) /*found extern label*/
                        {
                            fprintf(externfile, "%s   %d\n", line->args[0], MachineCode);
                            found = true;
                        }
                        MachineCode++;
                        if (!(validReg(line->args[1]) && validReg(line->args[2]))) /*if not other two arguments registers*/
                        {
                            if (checkIsExtLabel(line->args[1]) == true) /*check if first argument is extern*/
                            {
                                found = true;
                                fprintf(externfile, "%s   %d\n", line->args[1], MachineCode);
                            }
                            MachineCode++;
                            if (checkIsExtLabel(line->args[2]) == true) /*check if second argument is extern*/
                            {
                                fprintf(externfile, "%s   %d\n", line->args[2], MachineCode);
                                found = true;
                            }
                            MachineCode++;
                        }
                        else
                        {
                            MachineCode++;
                        }
                    }
                    else /*line has two arguments*/
                    {
                        if (!(validReg(line->args[0]) && validReg(line->args[1]))) /*checks if not two argument registers*/
                        {
                            if (checkIsExtLabel(line->args[0]) == true) /*chceck if first argument is register*/
                            {
                                fprintf(externfile, "%s   %d\n", line->args[0], MachineCode);
                                found = true;
                            }
                            MachineCode++;
                            if (checkIsExtLabel(line->args[1]) == true) /*checks if second argument is register*/
                            {
                                fprintf(externfile, "%s   %d\n", line->args[1], MachineCode);
                                found = true;
                            }
                            MachineCode++;
                        }
                        else
                        {
                            MachineCode++;
                        }
                    }
                }
                else /*line has one argument*/
                {
                    if (checkIsExtLabel(line->args[0]) == true) /*checks if this argument is extern or not*/
                    {
                        fprintf(externfile, "%s   %d\n", line->args[0], MachineCode);
                        found = true;
                    }
                    MachineCode++;
                }
            }
        }
    }
    fclose(assembler);
    fclose(externfile);
    if (found == false)
    {
        remove(ext);
    }
} /*end of create extern file*/

/*create entry file*/
void createEntryFile(char *preAss, char *entry)
{
    input_line *line;
    char curLine[81];
    FILE *assembler;
    FILE *entryFile;
    bool found = false;
    int MachineCode = 100;
    if (isEmptyEnt() == false)
        return;
    assembler = fopen(preAss, "r");
    entryFile = fopen(entry, "w");
    while (fgets(curLine, 81, assembler) != NULL) /*scan .am file to check each line label if its entry label*/
    {
        line = getLine(curLine);
        if (line->label != NULL)
            if (checkIsEntLabel(line->label) == true) /*given line label is entry label*/
            {
                found = true;
                fprintf(entryFile, "%s   %d\n", line->label, MachineCode);
            }
        MachineCode += MachineCodeInLine(line);
    }
    fclose(assembler);
    fclose(entryFile);
    if (found == false)
    {
        remove(entry);
    }
} /*end of create entry file*/