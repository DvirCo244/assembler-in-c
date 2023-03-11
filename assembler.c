#include "assembler.h"
input_line *getLine(char currline[])
{/*all cmd*/
    const char *ops[] = {"mov", "cmp", "add", "sub", "not", "clr", "lea",
                         "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop", /* Ops until here */
                        ".data", ".string", ".extern", ".entry"};
    char *p1, *p2;
    int length;
    char *tmpStr = (char *)malloc(0);
    int strIndex = 0;
    int i, status;
    input_line *line;

    if (!(line = malloc(sizeof(input_line))))/*allocating line struct and reseting it*/
    {
        freeLine(line);
        return NULL;
    }
    line->cmd = -1;
    line->label = NULL;
    line->isEffectless = false;
    line->isEOF = false;
    line->args = malloc(sizeof(char *) * 40);

    for (i = 0; i < 40; i++) /*resets all arguments in line*/
    {
        line->args[i] = NULL;
    }

    if ((length = strlen(currline)) == 0 || currline[0] == COMMENT_SIGN) /*checks if line is effectless or nto*/
    {
        line->isEffectless = true;
        free(line->args);
        line->args = NULL;
        return line;
    }
    p1 = p2 = currline;
    while (isspace(*p2))
        p2++;
    if (*p2 == '\0') /*checks if line empty*/
    {
        free(line->args);
        line->args = NULL;
        line->isEffectless = true;
        return line;
    }
    for (; p2 - currline < length - 1; p2++)
    {
        if (isspace(*p2) && isspace(*(p2 + 1)))
            continue;
        *p1++ = isspace(*p2) ? SPACE : *p2;
    }
    *p1 = END;

    sscanf(currline, "%s", tmpStr);
    if (tmpStr[(length = strlen(tmpStr)) - 1] == LABEL_DELIM) /*gets the line label and check if legal or not*/
    {
        if (validReg(tmpStr))/*checks if label is register*/
        {
            error(0);
            printf("Illegal label name : %s.\n", tmpStr);
            freeLine(line);
            return NULL;
        }

        if (validLabel(tmpStr))/*checks if label is incorrect format*/
        {
            if (copyStr(&(line->label), tmpStr))/*adds label to to line struct*/
                strIndex += strlen(line->label) + 1;
            else
            {
                error(0);
                printf("Insufficient memory.\n");
                freeLine(line);
                return NULL;
            }
        }
        else
        {
            error(0);
            printf("Illegal label name :%s.\n", tmpStr);
            freeLine(line);
            return NULL;
        }
        /*missing instruction or opeator error*/
        if (!sscanf((currline + strIndex), "%s", tmpStr) || strlen(currline) < strlen(tmpStr) + strIndex)
        {
            error(0);
            printf("Syntax error, No operator or instruction is recognised .\n");
            freeLine(line);
            return NULL;
        }
    }
    else
    {
        line->label = NULL;
    }
    /*line opeator is not recognised error*/
    if ((line->cmd = getOp(ops, tmpStr, sizeof(ops) / sizeof(ops[0]))) == -1)
    {
        error(0);
        printf("Syntax error, The operator's or instruction's name is not recognised\n");
        freeLine(line);
        return NULL;
    }
    else
        strIndex += (strlen(tmpStr) + (strcmp(tmpStr, currline + strIndex) == 0 ? 0 : 1));

    i = 0;
    if (!(status = getNextArg(currline + strIndex, tmpStr)))/*scan for all arguments in line and add them to line arguments*/
    {
        if (!(copyStr(&(line->args[0]), tmpStr)))
        {
            freeLine(line);
            return NULL;
        }
    }
    else if (status == -1)
        return NULL;
    else
    {
        i++;
        if (!(copyStr(&(line->args[0]), tmpStr)))
        {
            freeLine(line);
            return NULL;
        }
        for (; (status = getNextArg(NULL, tmpStr)); i++)
        {
            if (status == -1)
                return NULL;
            if (!(copyStr(&(line->args[i]), tmpStr)))
            {
                freeLine(line);
                return NULL;
            }
        }
    }
    if ((length = i) >= 0)
        line->args = realloc(line->args, sizeof(char *) * length + 1);
    return line;
}/*end of get_line*/

/*return the next argument in line*/
static bool getNextArg(char *src, char *dest)
{
    static char *cmdStr;
    int inStr = 0, i;
    if (src != NULL)
    {
        cmdStr = src;
    }
    /*ignore non argument charcters*/
    while (isspace(*cmdStr) || *cmdStr == ARG_SEPERATOR || *cmdStr == OPEN_BRACE || *cmdStr == CLOSE_BRACE)
        cmdStr++;

    if (*cmdStr == END)
        return false;

    for (i = 0; *cmdStr != ARG_SEPERATOR && *cmdStr != END && *cmdStr != OPEN_BRACE && *cmdStr != CLOSE_BRACE; cmdStr++)
    {
        if (inStr)/*argument is string*/
        {
            if (*cmdStr == STR_DELIM)
            {
                inStr = 0;
            }
        }
        else if (isspace(*cmdStr))
            break;
        else if (*cmdStr == STR_DELIM)
        {
            inStr = 1;
        }
        dest[i] = *cmdStr;
        i++;
    }
    if (inStr == 1)
    {
        error(0);/*string missing " at the end error"*/
        printf("String error, An \" is missing \n");
        return -1;
    }
    if (i == 0)
    {
        error(0);/*argument is empty*/
        printf("Syntax error, An argument cannot be empty.\n");
        return -1;
    }

    while (isspace(*cmdStr))
        cmdStr++;
    if (*cmdStr != END && *cmdStr != ARG_SEPERATOR && *cmdStr != OPEN_BRACE && *cmdStr != CLOSE_BRACE)
    {
        error(0);/*syntax error in argument*/
        printf("Syntax error, The argument type could not be resolved.\n");
        return -1;
    }
    dest[i] = END;
    if (*cmdStr == ARG_SEPERATOR || *cmdStr == OPEN_BRACE)
    {
        cmdStr++;
        if (*cmdStr == END || *cmdStr == CLOSE_BRACE || *cmdStr == ARG_SEPERATOR)
        {
            error(0);/*empty argument*/
            printf("Syntax error, An argument cannot be empty\n");
            return -1;
        }
    }
    return true;
}/*end of getNextArg*/

/*return the number of opeator in given string*/
int getOp(const char **ops, const char *str, const int opsAmount)
{
    int i;
    for (i = 0; i < opsAmount; i++)
    {
        if (strcmp(str, ops[i]) == 0)
            return i;
    }
    return -1;
}/*end of getOp*/

/*copy string to given destenation*/
bool copyStr(char **dest, const char *src)
{
    int strIndex = 0;
    if (!(*dest = malloc(strIndex = (strlen(src) + 1))))/*allocating and copy string*/
    {
        return false;
    }
    strcpy(*dest, src);
    return true;
}/*end of copyStr*/

/*checks if given string is valid label*/
bool validLabel(const char *labelStr)
{
    int i;
    if (strlen(labelStr) > 30)
    {
        return false;
    }
    if (validReg(labelStr))
    {
        return false;
    }
    if (!isalpha(labelStr[0]))
    {
        return false;
    }
    else
    {
        for (i = 1; i < strlen(labelStr) - 1; i++)
        {
            if (!isalnum(labelStr[i]))/*checks each character in string if its alphabet or number or not*/
            {
                return false;
            }
        }
        if (labelStr[i] != LABEL_DELIM && !isalpha(labelStr[i]) && !isdigit(labelStr[i]) && labelStr[i] != END)  /*checks the end of string*/
        {
            return false;
        }
    }

    return true;
}/*end of validLabel*/

/*checks if given string is valid register*/
bool validReg(const char *regStr)
{
    if (regStr[2] != END)
        return false;
    if (regStr[0] == 'r' && (regStr[1] >= '0' && regStr[1] <= '7'))
        return true;
    else
        return false;
}/*end of validReg*/

/*checks if given string valid number*/
bool validNum(char *numStr)
{
    int num = 0;

    if (numStr[0] != IMD_FLAG) /*checks for # in string*/
        return false;
    numStr++;

    if (*numStr == PLUS || *numStr == MINUS)
    {
        numStr++;
    }
    while (isdigit(*numStr))/*checks if each character is digit*/
    {
        num = num * 10 + (*numStr - '0');
        numStr++;
    }
    if (num > 196 || num < 0)
        return false;
    if (*numStr != END)
        return false;
    return true;
}/*end of validNum*/

/*reales memory in line*/
void freeLine(input_line *line)
{
    int i;
    if (!line)
        return;
    free(line->label);
    if (line->args != NULL) /* Release arguments in line */
        for (i = 0; line->args[i] != NULL; i++)
            free(line->args[i]);
    free(line->args);
    free(line);
} /*end of freeLine*/

FLAG error(int status)/*status change*/
{
    flag = ASM_FAILURE;
    return ASM_FAILURE;
}/*end of eror*/