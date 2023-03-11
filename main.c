#include "main.h"

int main(int argc,char *argv[]){
    int i = 1;
    if(argc < 2){  /*checking the amount of arguments is correct (at least one file name entered)*/
        printf("the amount of arguments entered is not correct\n");
        return (1);
    }
    while(argv[i] != NULL){ /*scanning through all files to run the program on them*/
size_t length = strlen(argv[i]) + EXTENSION_LENGTH +1 ;
char * currFileName = (char*) malloc(length); /*the current file name*/
char * preAssemblerFileName = (char*) malloc(length);/*preassemble file name*/
char * objectFileName = (char*) malloc(length);/*object file name*/
char * entryFileName = (char*) malloc(length);/*entry file name*/
char * externFileName = (char*) malloc(length);/*extern file name*/
/*adding the extension to each file name*/
sprintf(preAssemblerFileName,"%s%s",argv[i],PREASSEMBLER_EXTENSION);
sprintf(currFileName,"%s%s",argv[i],INPUT_EXTENSION);
sprintf(objectFileName,"%s%s",argv[i],OBJECT_EXTENSION);
sprintf(entryFileName,"%s%s",argv[i],ENTRY_EXTENSION);
sprintf(externFileName,"%s%s",argv[i],EXTERN_EXTENSION);
printf("IN FILE %d:\n",i);
if(fileExists(currFileName) == false){/*checking if the file path exists*/
    i++;
    printf("the file %s doesn't exists/incorrect path\n",currFileName);
continue;
}
else if(assembler(currFileName,preAssemblerFileName,objectFileName,entryFileName,externFileName) == ASM_FAILURE){/*calling the function that runs first and second pass*/
    remove(preAssemblerFileName);
    remove(objectFileName);
    remove(entryFileName);
    remove(externFileName);
printf("There is problem with code\n");
}
else
printf("succsess\n");
i++;
    }
return (1);
}/*end of main*/

/*check if file exists*/
bool fileExists(char *src){ 

    FILE* fp = fopen(src,"r");
    if(fp != NULL){
        fclose(fp);
        return true;
    }
    else{
    fclose(fp);
    return false;
    }
}/*end of fileExists*/

/*run each pass*/
FLAG assembler(char *src, char *preAssembler, char *objectFileName, char *entryFileName, char *externFileName)
{
flag = ASM_SUCCESS;
addMacros(src, preAssembler);/*runs preAssembly phase*/
if (first_pass(preAssembler) == 1) /*runs first phase*/
return ASM_FAILURE;
if (second_pass(preAssembler,objectFileName,entryFileName,externFileName) == 1) /*runs second phase*/
return ASM_FAILURE;
return ASM_SUCCESS;
}/*end of assembler*/
