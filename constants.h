#ifndef CONSTANTS_H
#define CONSTANTS_H

#define EXTENSION_LENGTH 4
#define ERROR_MAX_LEN 200
#define INPUT_EXTENSION ".as"
#define OBJECT_EXTENSION ".ob"
#define EXTERN_EXTENSION ".ext"
#define PREASSEMBLER_EXTENSION ".am"
#define ENTRY_EXTENSION ".ent"
/*register enum*/
typedef enum
{
    R0,
    R1,
    R2,
    R3,
    R4,
    R5,
    R6,
    R7
} reg;
/*cmd enum*/
typedef enum
{
    MOV,
    CMP,
    ADD,
    SUB,
    NOT,
    CLR,
    LEA,
    INC,
    DEC,
    JMP,
    BNE,
    RED,
    PRN,
    JSR,
    RTS,
    STOP,
    DOT_DATA,
    DOT_STRING,
    DOT_EXTERN,
    DOT_ENTRY
} OPCODE;
/*opeartors defines*/
#define NEWLINE '\n'
#define COMMENT_SIGN ';'
#define LABEL_DELIM ':'
#define STR_DELIM '"'
#define REG_FLAG 'r'
#define IMD_FLAG '#'
#define RND_FLAG '*'
#define ARG_SEPERATOR ','
#define SPACE ' '
#define OPEN_BRACE '('
#define CLOSE_BRACE ')'
#define END '\0'
#define PLUS '+'
#define MINUS '-'
/*flag enum*/
typedef enum
{
    ASM_SUCCESS,
    ASM_FAILURE
} FLAG;
/*boolean enum*/
typedef enum
{
    false = 0,
    true = 1
} bool;
#endif