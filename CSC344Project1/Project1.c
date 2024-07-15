//Program by Jack Wilcox

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Instruction Instruction;
typedef struct Cell Cell;

FILE* file         = NULL;
Cell* rwHead       = NULL;
Cell* tape         = NULL;
Instruction* table = NULL;
int numCells       = 0;

struct Instruction
{
    char writeVal;
    char moveDirection;
    int newState;
};

struct Cell
{
    char value;
    Cell* nextCell;
    Cell* prevCell;
};

void initializeTape(char* tapeContent)
{
    //Linked List of cells
    tape = (Cell*) malloc(numCells * sizeof(Cell));

    //For each cell, give it its value and its pointers to the next and previous cell
    for(int i = 0; i < numCells; i++)
    {
        tape[i].value = tapeContent[i];

        if(i == 0)
        {
            tape[i].prevCell = NULL;

            //Initialize the read/write head
            rwHead = &tape[i];
        }

        else
        {
            tape[i].prevCell = &tape[i - 1];
        }

        if(i == strlen(tapeContent) - 1)
        {
            tape[i].nextCell = NULL;
        }

        else
        {
            tape[i].nextCell = &tape[i + 1];
        }
    }
}

void initializeTable(int numStates)
{
    int state          = 0;
    char readVal       = ' ';
    char writeVal      = ' ';
    char moveDirection = ' ';
    int newState       = 0;

    int checkEOF = 0;

    /**************************************************************************************************************************************************/

    //2D Array of table instructions
    table = (Instruction*) malloc((numStates * 128) * sizeof(Instruction) * 128);

    while(1)
    {
        //Grab the formatted instructions from each line
        checkEOF = fscanf(file, "(%d,%c)->(%c,%c,%d)\n", &state, &readVal, &writeVal, &moveDirection, &newState);

        //If the end of the file is reached, break out
        if(checkEOF == EOF)
        {
            break;
        }

        else
        {
            //Store the instructions at the correct part of the table
            table[state * 128 * readVal].writeVal = writeVal;
            table[state * 128 * readVal].moveDirection = moveDirection;
            table[state * 128 * readVal].newState = newState;
        }
    }
}

void readTape(int startState, int endState)
{
    int curState = 0;
    char curRVal = ' ';
    char curWVal = ' ';
    char curDir  = ' ';

    /**************************************************************************************************************************************************/

    //Set the start state to begin parsing the tape
    curState = startState;

    while(1)
    {
        //If the head is at the end of the tape, allocate a new cell
        if(rwHead == NULL)
        {
            numCells++;
            tape = (Cell*) realloc(tape, sizeof(Cell) * numCells);

            tape[numCells - 1].value = 'B';
            tape[numCells - 1].nextCell = NULL;
            tape[numCells - 1].prevCell = &tape[numCells - 2];

            rwHead = &tape[numCells - 1];
        }

        curRVal = rwHead->value;

        //Grab the instruction from the table based on the current state and current read value
        curWVal = table[curState * 128 * curRVal].writeVal;
        curDir = table[curState * 128 * curRVal].moveDirection;
        curState = table[curState * 128 * curRVal].newState;

        rwHead->value = curWVal;

        //Break out when the end state is reached
        if(curState == endState)
        {
            break;
        }

        //Check to see what direction to move
        if(curDir == 'R')
        {
            rwHead = rwHead->nextCell;
        }

        else
        {
            rwHead = rwHead->prevCell;
        }
    }
}

void printTape()
{
    //Print out the new contents of the tape
    for(int i = 0; i < numCells; i++)
    {
        printf("%c", tape[i].value);
    }
}

int main()
{
    char inputFile[255] = {};

    char initTapeContent[255] = {};
    char tapeContent[255]     = "A";

    int numStates  = 0;
    int startState = 0;
    int endState   = 0;

    /**************************************************************************************************************************************************/

    //Get input file from the user
    printf("Input file: ");
    scanf("%s", &inputFile);

    //Open the file
    file = fopen(inputFile, "r");

    //If the file doesn't exist, end program
    if(file != NULL)
    {
        printf("Writing tape...\n");

        //Grab contents of tape
        fscanf(file, "%s\n", initTapeContent);
        strcat(tapeContent, initTapeContent);
        printf("Initial tape contents: %s\n", tapeContent);

        numCells = strlen(tapeContent);

        initializeTape(tapeContent);

        //Grab the number of states, start state, and end state
        fscanf(file, "%d\n", &numStates);
        fscanf(file, "%d\n", &startState);
        fscanf(file, "%d\n", &endState);

        initializeTable(numStates);

        readTape(startState, endState);

        printf("Final tape contents: ");
        printTape();
        printf("\n");
    }

    else
    {
        printf("No file with that name exists\n");
    }

    fclose(file);

    free(table);
    free(tape);

    return 0;
}
