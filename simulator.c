/**
 * Project 1
 * EECS 370 LC-2K Instruction-level simulator
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8 /* number of machine registers */
#define MAXLINELENGTH 1000


int num_inst_counter = 0;
int halt = 0;

void printState(stateType *);
int convertNum(int);
void is_trans(stateType *state);

int
main(int argc, char *argv[])
{
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;

    if (argc != 2) {
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        exit(1);
    }

    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
        printf("error: can't open file %s", argv[1]);
        perror("fopen");
        exit(1);
    }


    
    for (int i = 0; i < NUMREGS; i++) {
        state.reg[i] = 0;
    }
    state.pc = 0;

    //int address = 0//;//current line of address
    printState(&state);
    int counter = 0;
    while (halt == 0) {
        if (halt != 0) {
            break;
        }
        is_trans(&state);
        counter++;
        printState(&state);
    }
    return(0);
}


int
convertNum(int num)
{
    /* convert a 16-bit number into a 32-bit Linux integer */
    if (num & (1<<15) ) {
        num -= (1<<16);
    }
    return(num);
}


//only one line transformate
void is_trans(stateType *state) {
    int store_num = state->mem[state->pc];
    int opcode = store_num >> 22 & 0x7;
   
    num_inst_counter++;
    //halt
    if (opcode == 6) {
        printf("\n\nmachine halted\ntotal of %d instructions executed\nfinal state of machine:\n\n", num_inst_counter);
        //num_inst_counter++;
        state->pc++;
        halt = -1;
        return;
    }
    //noop
    if (opcode == 7) {
        state->pc++;
        //num_inst_counter++;
        return;
    }
    //number is the register number, not the content

    

    if (opcode == 4) {
        int regA = (store_num >> 19) & 0x7;
      
        if (state->reg[regA] == state->reg[regB]) {
            state->pc = state->pc + 1 + offset;
            return;
        }
        else {
             state->pc++;
            return;
        }
           //num_inst_counter++;
          
    }
    
 
    if (opcode == 3) {
        int regA = (store_num >> 19) & 0x7;
      
          // num_inst_counter++;
        return;
    }
  if (opcode == 2) {
        int regA = (store_num >> 19) & 0x7;
        int regB = (store_num >> 16) & 0x7;
        int offset = 0;
           offset = convertNum(store_num & 0xffff);
        state->reg[regB] = state->mem[offset + state->reg[regA]];
        state->pc++;
       // num_inst_counter++;
        return;
    }
    

    //add
    if (opcode == 0) {
        int regA = (store_num >> 19) & 0x7;
      
        state->reg[dest] = state->reg[regA] + state->reg[regB];
        state->pc++;
         //  num_inst_counter++;
        return;
    }
    
}
