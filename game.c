//------------------------------------------------------------------------------
// NAME: 
// STUDENT NUMBER: 
// COURSE: , SECTION: 
// INSTRUCTOR:
// ASSIGNMENT: 2, QUESTION: 1
// 
// REMARKS: 
// This is an implementation of the Conway's Game of Life.
// The goal of the assignment is to apply the principles of Design by Contract
//-------------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

// fuction prototypes
void fillInitGen();
void displayGen(int);
int validGen (int);

// macro definitions
#define MAX_INPUT 80

#define MAX_COLUMNS 60
#define MAX_ROWS 60
#define MAX_GENERATIONS 250

#define ALIVE 'X'
#define DEAD '.'

// file variables 
typedef struct Generations 
{
    char board[MAX_ROWS][MAX_COLUMNS];
} gen;

gen allGens[MAX_GENERATIONS];
int totalGen = 0;
int columns;
int rows;

int main(void)
{
    char caseName[MAX_INPUT];
    char input[MAX_INPUT];
    int r; // row counter
    int c; // column counter
    int i;    
    while(fgets(caseName, MAX_INPUT, stdin) != NULL)
    {
	if(caseName[0] == '*')
	{
	    scanf("%d %d", &rows, &columns); // read in second line for board dimensions
	   // printf("%s",caseName);	
	    fgets(input,MAX_INPUT, stdin); // read in the second line with fgets
	    fillInitGen();
	   // printf("Valid Generation? %i\n",validGen(0));
	   // displayGen(0);
	}
	// play while(!gameOver)
	    // create new struct 
	    // iterate through each cell
	    // neighbors = numNeighbors()
	    // if (alive): if neighbors != 2 or 2 => dead
	    // else (dead): if neighbors == 3 => alive
	    // totalGen++
	    // display
	    // if genAlreadExists() || totalGen >= 250 => gameOver
	    // destroy struct
    }	

    // * Test for non-valid generations *//
    
    gen testGens[MAX_GENERATIONS];
    
    for (r = 0; r < 10; r++)
    {
	for (c = 0; c < 10; c++)
	{
	    testGens[0].board[r][c] = ALIVE; // testGens[0] valid but all the same 
	    testGens[1].board[r][c] = ALIVE; // testGens[1] valid ------> is saying invalid. why?
	    testGens[2].board[r][c] = ALIVE; // testGens[2] one cell that isn't ALIVE or DEAD
	    testGens[3].board[r][c] = ALIVE; // testGens[3] one cell that is NULL
	    if (c%5 == 0)
	    {
	    testGens[0].board[r][c] = ALIVE; // testGens[0] valid but all the same 
	    testGens[1].board[r][c] = DEAD; // testGens[1] valid
	    testGens[2].board[r][c] = 't'; // 
	    }
	}
    }

    
    for (i=0; i<=3; i++)
    {
        printf("Valid Generation? %i: %i\n",i,validGen(i));
    }

    printf("end of processing.");
    return(0);
} 

// ----------------------------------------------------------------
// displayGen
//
// PURPOSE: Displays the generation
// INPUT PARAMETERS:
// The generation number of the generation to be displayed
// ----------------------------------------------------------------
void displayGen(int gen)
{
    int r;
    int c;
    char decoration[MAX_INPUT];

    printf("Generation %i:\n", gen);

    // make the decorative line
    decoration[0]='+';
    for (c = 1; c <= columns; c++)
    {
	decoration[c] = '-';
    }
    decoration[columns+1] = '+';
    decoration[columns+2] = '\0';

    /** print the generation **/

    printf("%s\n",decoration);
    
    for (r=0; r<rows; r++)
    {	
	printf("|");
	for (c = 0; c < columns; c++)
	{
	    printf("%c",allGens[gen].board[r][c]);
	}
    printf("|\n");
    } 

    printf("%s\n", decoration);
    
} // display

// ------------------------------------------------------------------
// fillInitGen
// 
// PURPOSE: Fills the first generation for any case
// ------------------------------------------------------------------
void fillInitGen()
{
    char input[MAX_INPUT];
    int r;
    int c;

    // traverse every cell to fill the first generation
    for (r=0; r<rows; r++)
    {
	fgets(input, MAX_INPUT, stdin);
	for (c = 0; c < columns; c++)
	{	
 	    if (input[c] == 'X') 
	    {
		allGens[0].board[r][c] = ALIVE;
	    }
	    else // (input[c] == ' ')
	    {
		allGens[0].board[r][c] = DEAD;
	    }   
	}
    }
} // fillInitGen


int validGen(int genNum)
{
    int r;
    int c;
    int valid = 1; // boolean
    
    // board size is less than max
    for (r = 0; r < rows; r++)
    {
	for (c = 0; c < columns; c++)
	{
	    // each column should contain alive or dead
	    if (allGens[genNum].board[r][c] != ALIVE && allGens[genNum].board[r][c] !=DEAD)
	    {
		valid = 0;
	    }
	}
    }
    
    // check for legaly alive and dead. 
    // So the colums can't all be alive or all dead?? or can they?  

    assert(r <= MAX_ROWS);
    assert(c <= MAX_COLUMNS);

    assert(rows <= MAX_ROWS);
    assert(columns <= MAX_COLUMNS);

    return valid;
} // validGen
