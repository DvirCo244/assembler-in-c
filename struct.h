#ifndef STRUCT_H
#define STRUCT_H
#include "constants.h"
/*line struct*/
typedef struct
{
    char *label;
    int cmd;
    char **args;
    bool isEffectless;
    bool isEOF;
} input_line;
/*macro struct*/
struct Macro
{
    char *name;
    int count;
    char **strings;
    struct Macro *next;
};
/*label struct*/
typedef struct
{
    char *labelName;
    int address;
} label;
/*label node struct*/
typedef struct labelNode
{
    label *this;
    struct labelNode *next;
} labelNode;
/*label list struct*/
typedef struct
{
    labelNode *head;
} labelList;
/*extern node struct*/
typedef struct externNode
{
    struct
    {
        char *labelName;
    } this;
    struct externNode *next;
} externNode;
/*entry node struct*/
typedef struct entryNode
{
    struct
    {
        char *labelName;
    } this;
    struct entryNode *next;
} entryNode;
/*extern list struct*/
typedef struct
{
    externNode *head;
} externList;
/*entry list struct*/
typedef struct
{
    entryNode *head;
} entryList;
/*data node struct*/
typedef struct dataNode
{
    unsigned int value;
    struct dataNode *next;
} dataNode;
/*data list struct*/
typedef struct
{
    dataNode *head;
    int length;
} dataList;
#endif