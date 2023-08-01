/**
 * Project 1
 * Assembler code fragment for LC-2K
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#define MAXLINELENGTH 1000

int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);
int count_op = 0;
struct labelAdd {
    char* labelname;
    int labelAddress;
    
};


int opcodeTransfer (char *opcode, char *arg2);
int arg2transfer (struct labelAdd lab[MAXLINELENGTH],int innercounter,
                  char *opcode, char*arg0, char* arg2, int currLinecounter);
int regtransfer (char* opcode,char* arg0, char* arg1);
int errorchecking (char* label,char* opcode, char* arg0,
                   char* arg1, char* arg2, struct labelAdd lab[MAXLINELENGTH],
                   int innercounter, int linecounter);



int
main(int argc, char *argv[])
{
    char *inFileString, *outFileString;
    FILE *inFilePtr, *outFilePtr;
    char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
    arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];
    
    if (argc != 3) {
        printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
               argv[0]);
        exit(1);
    }
    
    inFileString = argv[1];
    outFileString = argv[2];
    
    inFilePtr = fopen(inFileString, "r");
    if (inFilePtr == NULL) {
        printf("error in opening %s\n", inFileString);
        exit(1);
    }
    outFilePtr = fopen(outFileString, "w");
    if (outFilePtr == NULL) {
        printf("error in opening %s\n", outFileString);
        exit(1);
    }
    
    //for array inside
    int innercount = 0;
    //for the address counter
    int counter = 0;
    
    struct labelAdd lab[MAXLINELENGTH];
    for (int i = 0; i < MAXLINELENGTH; i++) {
        lab[i].labelname = malloc(MAXLINELENGTH);
    }
    
    
    //first read
    while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2) ) {
        //new add
        
        if (label[0] != '\0') {
            lab[innercount].labelAddress = counter;
            strcpy(lab[innercount].labelname, label);
            
            //test
            //                        printf("%s \n",lab[innercount].labelname);
            //
            //                        printf("%d \n",lab[innercount].labelAddress);
            //test end
            innercount++;
        }
        counter++;
    }
    
    
    rewind(inFilePtr);
    
    int currentline = 0;
    
    while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
        
        errorchecking(label, opcode, arg0, arg1, arg2, lab, innercount, currentline);
        int result = 0;
        int op = 0;
        int ag2 = 0;
        int reg = 0;
        reg = regtransfer(opcode, arg0, arg1);
        op = opcodeTransfer(opcode, arg2);
        ag2 = arg2transfer(lab, innercount, opcode, arg0, arg2, currentline);
        result = reg + op + ag2;
        //        result = regtransfer(opcode, arg0, arg1) + opcodeTransfer(opcode, arg2)+
        //        arg2transfer(lab, innercount, opcode, arg0, arg2, currentline);
        //
        // test
        //        printf("%d \n", op);
        //        printf("%d \n", reg);
        //        printf("%d \n", ag2);
        //        printf("which leads to = %d \n \n", result);
        // end test
        
        fprintf(outFilePtr,"%d\n", result);
        
        currentline++;
    }
    
    
    /* here is an example for how to use readAndParse to read a line from
     inFilePtr */
    //        if (! readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2) ) {
    //            /* reached end of file */
    //            return 0;
    //        }
    
    /* this is how to rewind the file ptr so that you start reading from the
     beginning of the file */
    //    rewind(inFilePtr);
    //
    //    /* after doing a readAndParse, you may want to do the following to test the
    //        opcode */
    //    if (!strcmp(opcode, "add")) {
    //        /* do whatever you need to do for opcode "add" */
    //    }
    
    //    return(0);
    
    for (int i = 0; i < MAXLINELENGTH; i++){
        free(lab[i].labelname);
    }
}

/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if successfully read
 *
 * exit(1) if line is too long.
 */
int
readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
             char *arg1, char *arg2)
{
    char line[MAXLINELENGTH];
    
    /* delete prior values */
    label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';
    
    /* read the line from the assembly-language file */
    if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
        /* reached end of file */
        return(0);
    }
    
    /* check for line too long (by looking for a \n) */
    count_op++;
    if (strchr(line, '\n') == NULL) {
        /* line too long */
        printf("error: line too long\n");
        exit(1);
    }
    
    /* is there a label? */
    char *ptr = line;
    if (sscanf(ptr, "%[^\t\n\r ]", label)) {
        /* successfully read label; advance pointer over the label */
        ptr += strlen(label);
    }
    
    /*
     * Parse the rest of the line.  Would be nice to have real regular
     * expressions, but scanf will suffice.
     */
    sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]",
           opcode, arg0, arg1, arg2);
    return(1);
}

int
isNumber(char *string)
{
    /* return 1 if string is a number */
    int i;
    return( (sscanf(string, "%d", &i)) == 1);
}


int regtransfer (char* opcode,char* arg0, char* arg1) {
    //new add
    if(strcmp(opcode, ".fill") == 0) {
        if(isNumber(arg0) == 1){
            int num = atoi(arg0);
            return num;
        }
        else {
            return 0;
        }
    }
    else if (strcmp(opcode, "noop") == 0 || strcmp(opcode, "halt") == 0){
        return 0;
    }
    
    else {
        
        int regA = 0;
        int regB = 0;
        regA = atoi(arg0);
        regB = atoi(arg1);
        
        regA = regA << 19;
        
        regB = regB << 16;
        
        int reg = regA + regB;
        //        printf("%d \n",regA);
        //        printf("%d \n", regB);
        return reg;
    }
    
    /*
     regA = regA << 19;
     regB = regB << 16;
     reg = regA|regB;
     return reg;
     */
    
}

int arg2transfer (struct labelAdd lab[MAXLINELENGTH],int innercounter,
                  char *opcode, char*arg0, char* arg2, int currLinecounter) {
    //R type
    int labelocat;
    char *op[8] = {"add" , "nor" ,"lw","sw","beq","jalr","halt","noop" };
    if (strcmp(opcode, op[0]) == 0 || strcmp(opcode, op[1]) == 0) {
        int destreg = 0;
        destreg = atoi(arg2);
        return destreg;
    }
    
    //I and J type
    if(strcmp(opcode, op[2]) == 0 || strcmp(opcode, op[3]) == 0
       || strcmp(opcode, op[4]) == 0) {
        
        
        if(isNumber(arg2) == 1) {
            labelocat = atoi(arg2);
            if (labelocat < 0) {
                labelocat = (labelocat & 0xFFFF);
            }
            return labelocat;
        }
        
        for (int i = 0; i < innercounter; i++) {
            
            if(strcmp(lab[i].labelname,arg2) == 0) {
                labelocat = lab[i].labelAddress;
                
                
                //beq //new add
                if (strcmp(opcode, op[4]) == 0 ){
                    int offsetNum = 0;
                    offsetNum = labelocat - currLinecounter -1;
                    
                    if (offsetNum < 0){
                        offsetNum = (offsetNum & 0xFFFF);
                        return offsetNum;
                    }
                    else{
                        return offsetNum;
                    }
                    
                }
                
                return labelocat;
            }
        }
        
    }
    if (strcmp(opcode, op[5]) == 0) {
        return 0;
    }
    
    //.fill
    if (strcmp(opcode, ".fill") == 0){
        if (isNumber(arg0) == 1){
            return 0;
        }
        else {
            for (int i = 0; i < innercounter; i++ ) {
                if (strcmp(lab[i].labelname, arg0) == 0) {
                    return lab[i].labelAddress;
                }
            }
        }
    }
    
    return 0;
}//arg2


int opcodeTransfer (char *opcode, char *arg2) {
    
    char *op[8] = {"add" , "nor" ,"lw","sw","beq","jalr","halt","noop" };
    
    if (strcmp(opcode, op[0]) == 0) {
        return 0;
    }
    else if (strcmp(opcode, op[1]) == 0) {
        return pow(2,22);
    }
    
    else if (strcmp(opcode, op[2]) == 0) {
        return pow(2, 23);
    }
    else if (strcmp(opcode, op[3]) == 0) {
        return pow(2,23) + pow(2, 22);
    }
    else if (strcmp(opcode, op[4]) == 0) {
        return pow(2,24);
    }
    
    else if (strcmp(opcode, op[5]) == 0) {
        return pow(2, 24) + pow(2, 22);
    }
    
    else if (strcmp(opcode, op[6]) == 0) {
        return pow(2, 23) + pow(2, 24);
    }
    else if (strcmp(opcode, op[7]) == 0) {
        return pow(2, 22) + pow(2, 23) + pow(2, 24);
        
    }
    else {
        return 0;
    }
}//opcodetransfer


//Duplicate labels
//Unrecognized opcodes
int errorchecking (char* label,char* opcode, char* arg0,
                   
                   char* arg1, char* arg2, struct labelAdd lab[MAXLINELENGTH],
                   int innercounter, int linecounter) {
    char *op[8] = {"add" , "nor" ,"lw","sw","beq","jalr","halt","noop" };
    int offsetnum = 0;
    char* name = "";
    int address = 0;
    
    
    //undefined labels
    if (strcmp(opcode, op[2]) == 0 || strcmp(opcode, op[3]) == 0
        || strcmp(opcode, op[4]) == 0) {

        //is number
        if (isNumber(arg2) == 1) {
            int num = atoi(arg2);
            //-32768 to 32767
            if (num < -32768 || num > 32767 ) {
                printf("arg2 too big for offset");
                exit(1);
            }
        }
    
//
//        //undefined labels, when there is arg2, check offsetfiles for beq
        else if (strcmp(arg2, "") != 0) {
            
            for (int i = 0; i < innercounter; i++){
                char *tempname = lab[i].labelname;
                if (strcmp(arg2, tempname ) == 0) {
                    name = lab[i].labelname;
                    address = lab[i].labelAddress;
                    offsetnum = address;
                    if (strcmp(opcode, op[4]) == 0) {
                    offsetnum = address - 1 - linecounter;
                    }
                    break; //find similar
                }
            }

            //after all the rounds, no similar
            if (strcmp(name, "") == 0) {
                printf("undefined label name \n");
                exit(1);
            }
        }
    }
    
        if (offsetnum < -32768 || offsetnum > 32767 ) {
            printf("offsetnum too big\n");
            exit(1);
        }

    

    
    //这上有问题！！！！
    //duplicates label
    for (int i = 0; i < innercounter; i++) {
        for (int j = i + 1; j < innercounter; j++) {
            if (strcmp(lab[i].labelname, lab[j].labelname) == 0) {
                printf("duplicates label \n");
                exit(1);
            }
        }
    }
    
    //Unrecognized opcodes
    if (strcmp(opcode, "add") != 0 && strcmp(opcode, "nor") != 0  &&
        strcmp(opcode, "lw") != 0  && strcmp(opcode, "sw") != 0  &&
        strcmp(opcode, "beq") != 0  && strcmp(opcode, "jalr") != 0  &&
        strcmp(opcode, "halt") != 0 && strcmp(opcode, "noop") != 0  &&
        strcmp(opcode, ".fill") ) {
        printf("unrecognized opcodes \n");
        exit(1);
    }
    
    
    
    return 0;
}
