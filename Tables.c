#include "Tables.h"
/*head ndoe of symbol list*/
labelList symbol_list = {NULL}; 
/*head node of extern list*/
static externList ext_list = {NULL};
/*head node of entry list*/
static entryList ent_list = {NULL};
/*head node of data list*/
static  dataList  data_list = {NULL,0};
/*create label struct*/
static label *createLabel(char *labelName,int address);
/*create label node stuct*/
static labelNode *createLabelNode(label *this,labelNode *next);
/*adds label to symbol label*/
bool addLabel(char *labelName,int address){
    int i = 0;
   labelNode *newLabel;
   labelNode *current = symbol_list.head;
char *labelcpy = (char *) malloc(sizeof(char)*strlen(labelName)); /*checking if label is saved value*/
strcpy(labelcpy,labelName);
labelcpy[strlen(labelcpy)-1] = '\0';
while(i <27){
    if(strcmp(labelcpy,constStrings[i++]) == 0){
        printf("the label %s has name of save value.\n",labelcpy);
    free(labelcpy);
    return false;
    }
}
free(labelcpy);

   if(labelName == NULL){ /*checking if label name is null*/
    error(0);
    printf("Unknown error with label name. Received value of NULL.\n");
    return false;
   }
   while(current){ /*checking if label is already defined*/
    if(strcmp(current->this->labelName,labelName) == 0){
        error(0);
        printf("Multiple definitions of label %s. Please define each label exactly once.\n",labelName);
        return false;
    } else if(current->next){
        current = current->next;
    }else
    break;
   }
newLabel = createLabelNode(createLabel(labelName,address),NULL); /*creating new label */
if(newLabel == NULL)
return false;
if(!symbol_list.head)/*adding label to list*/
symbol_list.head = newLabel;
else
current->next = newLabel;
return true;
}/*end of addLabel*/

/*gets symbol and adds it to extern table*/
bool addExt(char *symbol){
   externNode *node = ext_list.head;
    if(!node && !(node = malloc(sizeof(node)))){/*allocating memory for extern node*/
        error(0);
        printf("Insufficient memory.\n");
        return false;
    }
    if(ext_list.head != NULL){
        while(node->next)
        node = node->next;
        if(!(node->next = malloc(sizeof(node)))){
            error(0);
            printf("Insufficient memory.\n");
            return false;
        }/*adding node to list*/
        node = node->next;
    }else {
        ext_list.head = node;
    }/*assain vlaues to node*/
    node->this.labelName = NULL;
    node->this.labelName = symbol;
    node->next = NULL;
    return true;
}/*end of addExt*/

/*gets symbol and adds it to entry table*/
bool addEnt(char *symbol){
entryNode *node = ent_list.head;
if(!node && !(node = malloc(sizeof(node)))){/*allocating memory for node*/
    error(0);
    printf("Insufficient memory.\n");
    return false;
}
if(ent_list.head != NULL){
    while(node->next)
    node = node->next;
    if(!(node->next = malloc(sizeof(node)))){
        error(0);
        printf("Insufficient memory.\n");
        return false;
    }/*adding node to list*/
    node = node->next;
}else{
    ent_list.head = node;
}/*assign value to node*/
node->this.labelName = NULL;
node->this.labelName = symbol;
node->next = NULL;
return true;
}/*end of addEnt*/

/*creates label node with label*/
static labelNode *createLabelNode(label *this,labelNode *next){
    labelNode *newLabelNode = (labelNode*) malloc(sizeof(labelNode));/*allocating memory for node*/
    if(newLabelNode == NULL || this == NULL){
        if(this)
        free(this->labelName);
        free(this);
        free(newLabelNode);
        error(0);
        printf("Insufficient memory1.\n");
        return NULL;
    }/*assign values*/
    newLabelNode->this = this;
    newLabelNode->next = next;
    return newLabelNode;
}/*end of createLabelNode*/

/*creates label*/
static label *createLabel(char *labelName,int address){
    label *newLabel = malloc(sizeof(label));/*allocating memory for new label*/
    labelName[strlen(labelName)-1] = '\0';
    if(newLabel == NULL){
        error(0);
        printf("Insufficient memory.\n");
        return NULL;
    }/*assign values to the created label*/
    newLabel->labelName = NULL;
        newLabel->labelName = labelName;
    newLabel->address = address;
    return newLabel;
}/*end of createLabel*/

/*creates data node*/
static dataNode* createDataNode(int data){
    dataNode* newDataNode = NULL;
    if((newDataNode = malloc(sizeof(dataNode)))){/*allocating memory and assign values to data node*/
        newDataNode->value = data;
        newDataNode->next = NULL;
    }
    return newDataNode;
}/*and of createDataNode*/

/*adds data to data table*/
int addData(unsigned int data){
    dataNode *curr,*newNode;
    data_list.length++;
    newNode = createDataNode(data);/*create node with given value*/
    if(newNode == NULL)
    return false;
        if(data_list.head == NULL){/*adds the node to data list*/
            data_list.head = newNode;
        } else {
            for(curr = data_list.head; curr->next != NULL;curr = curr->next);
            curr->next = newNode;
        }
    return true;
}/*end of addData*/

/*checks if entry table empty*/
bool isEmptyEnt(){
    entryNode *entryNod = ent_list.head;
if(entryNod != NULL)
return true;
return false;
}/*and of isEmptyEnt*/

/*checks if extern table empty*/
bool isEmptyExt(){
    externNode *externNod = ext_list.head;
    if(externNod != NULL)
    return true;
    return false;
}/*end of isEmptyExt*/

/*checks if given label is extern label*/
bool checkIsExtLabel(char *label){
    externNode *externNod = ext_list.head;
    while(1){
        if(strcmp(externNod->this.labelName,label) == 0){
            return true;
        }
    if(externNod->next == NULL)
    return false;
    else
    externNod = externNod->next;
    }
}/*end of checkIsExtLabel*/

/*returns data in given place in list*/
int getData(int place){
    dataNode *dataNod = data_list.head;
    while(place != 0){
        place--;
        dataNod = dataNod->next;
    }
    return dataNod->value;
}/*end of getData*/

/*reales all memory allocated in all tables*/
void realseMemory(){
dataNode *currentD = data_list.head;
dataNode *nextD;
externNode *currentExt = ext_list.head;
externNode *nextExt;
labelNode *currentL = symbol_list.head;
labelNode *nextL;
entryNode *currentEnt = ent_list.head;
entryNode *nextEnt;
while(currentD != NULL){/*realesing memory for data table*/
    nextD = currentD->next;
    free(currentD);
    currentD = nextD;
}
data_list.head = NULL;
data_list.length = 0;

while(currentL != NULL){/*realesing memory for symbol table*/
    nextL = currentL->next;
    free(currentL->this->labelName);
    currentL->this->address = 0;
    free(currentL->this);
    free(currentL);
    currentL = nextL;
}
symbol_list.head = NULL;

while(currentEnt != NULL){/*realesing memory for entry table*/
    nextEnt = currentEnt->next;
    free(currentEnt->this.labelName);
    free(currentEnt);
    currentEnt = nextEnt;
}
ent_list.head = NULL;

while(currentExt != NULL){/*realesing memory for extern table*/
    nextExt = currentExt->next;
    free(currentExt->this.labelName);
    free(currentExt);
    currentExt = nextExt;
}
ext_list.head = NULL;
}/*end of realeseMemory*/

/*returns the length of data table*/
int getDataAmount(){
   int n = 0;
    dataNode *dataNod = data_list.head;
    if(dataNod == NULL)
    return -1;

    else{
        while(dataNod != NULL){/*calculate the length of data table*/
            n++;
            dataNod = dataNod->next;
        }
        return n;
    }
}/*end of getDataAmont*/

/*checks if given string is label*/
label *checkIsLabel(char *label)
{
    labelNode *labelNod = symbol_list.head;
    while(labelNod != NULL){/*checks if label*/
        if(strcmp(labelNod->this->labelName,label) == 0){
            return labelNod->this;
        }else
        labelNod = labelNod->next;
    }
    return NULL;
}/*end of checkIsLabel*/

/*checks if given string is entry label*/
bool checkIsEntLabel(char *label)
{
    entryNode *entryNod = ent_list.head;
    if(label[strlen(label)-1] == ':')
    label[strlen(label)-1] = '\0';
    while(1){/*check if label is entry*/
        if(strcmp(entryNod->this.labelName,label) == 0){
        return true;
        }
        if(entryNod->next == NULL)
        return false;
        else
        entryNod = entryNod->next;
    }
}/*end of checkIsEntLabel*/