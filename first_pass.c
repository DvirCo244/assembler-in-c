
#include "first_pass.h"
/*the main function in the first phase returning if the first pass is correct*/
FLAG first_pass(char *preAss)
{
    input_line *line;
    char curLine[81];
    FILE *assembler;
    assembler = fopen(preAss, "r");
    while (fgets(curLine, 81, assembler) != NULL)/*ruuning through each line in am file after preassembler phase*/
    {
        if ((line = getLine(curLine))) /*getline is function that*/
        {
            if (line->isEOF)
            {
                freeLine(line); /*realising memory allocated for line*/
                break;
            }
            if (line->isEffectless)
            {
                continue;
            }
            if (!handleLine(line))/*analyze the current line*/
            {
                error(0);
            }
        }
    }
    fclose(assembler);
    assembler = fopen(preAss,"r");
    fclose(assembler);
    resetMachineCodeCounter();
    return flag;
} /*end of first phase*/

static bool handleLine(input_line *line)
{
    /*int static MachineCodeCounter = 100; */       /*machine code*/
    int hasLabel = (line->label ? true : false);/*checks if line has label*/

    if (!validArgumentsAmount(line))/*checks if the line as valid arguments amount*/
        return false;

    if (hasLabel)/*if line has label add it to label table with current machineCode Int*/
    {
        if (!addLabel(line->label, MachineCodeCounter))
            return false;
    }
    else if (line->cmd == DOT_EXTERN)/*checks if the line is .extern and add the label to the extern table*/
    {
        if (line->args[1] != NULL || !addExt(line->args[0]))
            return false;
    }
    else if (line->cmd == DOT_ENTRY)/*checks if the line is .entry and add the label to the entry table*/
    {
        if (line->args[1] != NULL || !addEnt(line->args[0]))
            return false;
    }
    if (line->cmd == DOT_DATA)/*checks if the line is .data and adds the data to the data table*/
    {
        if (line->args != NULL)
        {
            char **arg = line->args;
            while (*arg != NULL)
            {
                int num = 0;
                char *rst;
                num = (int)strtol(*arg++, &rst, 10); /*cast the string to number*/
                if (strcmp(rst, "") == 0)
                {
                    if (!(addData(num)))/*adds the number to data table*/
                    {
                        printf("Insufficent memory.\n");
                        return false;
                    }
                }
                else
                {
                    printf("Unknown argument type.\n");
                    return false;
                }
            }
        }
        else
        {
            printf("Too less arguments.\n");
            return false;
        }
    }
    if (line->cmd == DOT_STRING)/*checks if line is .string and add data to data table*/
    {
        if (line->args != NULL && line->args[1] == NULL)
        {
            char *c = line->args[0];
            if (*c++ == '"')
            {
                while (*c != '"')
                {
                    if (!(addData(*c++)))/*adds character to data table and checks if its faild to do so*/
                    {
                        printf("Insufficiate memory.\n");
                        return false;
                    }
                }
                if (*++c != '\0')
                {
                    printf("Wrong string format, extra character after \"\n");
                    return false;
                }
                if (!(addData('\0')))/*adds data and checks if its failed to do so*/
                {
                    printf("Insufficiate memory.\n");
                    return false;
                }
            }
        }
        else
        {
            printf("Too many arguments.\n");
            return false;
        }
    }
    if (!validArg(line,false))/*checks if each argument in line is valid in this phase*/
        return false;
    MachineCodeCounter += MachineCodeInLine(line);/*increase the current machinecode with the amount added in current line*/
    return true;
}/*end of handleLine*/

void resetMachineCodeCounter()/*resets machine code counter after each file*/
{
    MachineCodeCounter = 100;
}/*end of resetMachineCodeCounter*/

/*caluclate how many lines are added in given line*/
int MachineCodeInLine(input_line *line)
{
    char *c;
    int i;
    int MachineCodeCounter = 0;
    switch (line->cmd)
    {
    case DOT_ENTRY:/*lines are not added*/
    case DOT_EXTERN:
        break;
    case DOT_DATA:
        i = 1;
        MachineCodeCounter++;
        while (line->args[i++] != NULL)/*calculate how many lines added in .data line*/
        {
            MachineCodeCounter++;
        }
        break;
    case DOT_STRING:
        MachineCodeCounter++;
        c = line->args[0];
        if (*c++ == '"')
        {
            while (*c++ != '"')/*calculate how many lies added in .string line*/
                MachineCodeCounter++;
        }
        break;
    case STOP:
    case RTS:
        MachineCodeCounter--;
    default:
        MachineCodeCounter++;
        if (line->args[0] != NULL)
        {
            if (line->args[1] != NULL)
            {
                if (line->args[2] != NULL)
                {
                    MachineCodeCounter++;
                    if (!(validReg(line->args[1]) && validReg(line->args[2])))/*checks if three arguments the last two are registers*/
                    {
                        MachineCodeCounter += 2;
                    }
                    else
                    {
                        MachineCodeCounter++;
                    }
                }
                else
                {
                    if (!(validReg(line->args[0]) && validReg(line->args[1])))/*checks if two argumnets are registers or not*/
                    {
                        MachineCodeCounter += 2;
                    }
                    else
                    {
                        MachineCodeCounter++;
                    }
                }
            }
            else
                MachineCodeCounter++;
        }
    }
    return MachineCodeCounter;
}/*end of MachineCodeInLine*/

/*checks if all the line arguments is valid in first phase*/
bool validArg(input_line *line,bool afterFirstScan)
{
    int i = 0;
    bool Correct = true;
    while (line->args[i] != NULL)
    {
        if (line->cmd == DOT_DATA || line->cmd == DOT_STRING)/**/
            break;
        if (line->args[i][strlen(line->args[i]) - 1] == ':')/*checks if the arguments is label*/
        {
            printf("This argument is not in correct format: %s\n", line->args[i]);
            Correct = false;
        }
        if (!afterFirstScan &&!validReg(line->args[i]) && !validNum(line->args[i]) && !validLabel(line->args[i]) && !checkIsExtLabel(line->args[i]))/*before tables checks*/
        {
            printf("This argument is not in correct format: %s\n", line->args[i]);
            Correct = false;
        }
        if(afterFirstScan && !validReg(line->args[i]) && !validNum(line->args[i]) && (checkIsLabel(line->args[i]) == NULL)
        && (!checkIsExtLabel(line->args[i])) && (!checkIsEntLabel(line->args[i])))/*after table checks*/{
            printf("This argument is not in correct format: %s\n",line->args[i]);
            Correct = false;
        }
        i++;
    }
    return Correct;
}/*end of validArg*/

/*checks if given line argument amount is correct in first phase*/
bool validArgumentsAmount(input_line *line)
{
    int i;
    switch (line->cmd)
    {
    case MOV: /*group takes two arguments or three depents on sort*/
    case CMP:
    case ADD:
    case SUB:
    case LEA:
        if (line->args[0] == NULL || line->args[1] == NULL) 
        {
            error(0);
            printf("Not enough arguments.\n");
            return false;
        }
        else
        {
            i = 0;
            while (line->args[i++] != NULL)
                ;
            if (i != 3)
            {
                error(0);
                printf("Too Many arguments.\n");
                return false;
            }
        }
        break;
    case NOT:/*grop takes one or three depants on sort*/
    case CLR:
    case INC:
    case DEC:
    case JMP:
    case BNE:
    case RED:
    case PRN:
    case JSR:
        if (line->args[0] == NULL)
        {
            error(0);
            printf("Not enough arguments.\n");
            return false;
        }
        else
        {
            i = 0;
            while (line->args[i] != NULL)
                i++;
            if (i != 3 && i != 1)
            {
                error(0);
                printf("Too Many arguments.\n");
                return false;
            }
        }
        break;
    case STOP:/*group dont take arguments*/
    case RTS:
        if (line->args[0] != NULL && line->cmd != 14 && line->cmd != 15)
        {
            error(0);
            printf("Too Many arguments5. \n");
            return false;
        }
    }
    return true;
}/*end of validArgumentsAmount*/