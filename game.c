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
void validGen(int);
void validCell(int, int, int);
void validGame();
int aliveNeighbors(int, int, int);

// macro definitions
#define MAX_INPUT 80

#define MAX_COLUMNS 60
#define MAX_ROWS 60
#define MAX_GENERATIONS 250

#define ALIVE '*'
#define DEAD '.'

// file variables 
typedef struct Generations 
{
    char board[MAX_ROWS][MAX_COLUMNS];
} gen;

gen allGens[MAX_GENERATIONS];
int totalGens;
int totalRows; 
int totalColumns;

int main(void)
{
    char caseName[MAX_INPUT];
    char input[MAX_INPUT];
    
    while(fgets(caseName, MAX_INPUT, stdin) != NULL)
    {
	if(caseName[0] == '*')
	{
	    // initialize totalGen 
            totalGens = 0;
            scanf("%d %d", &totalRows, &totalColumns);// read in second line for board dimensions
            
            // make sure board size is valid
            assert(totalRows <= MAX_ROWS);
            assert(totalColumns <= MAX_COLUMNS);
	    
            printf("%s",caseName);

	    fgets(input,MAX_INPUT, stdin); // read in the second line with fgets
	    
            fillInitGen();
            printf("Rows: %i, colums:%i \n", totalRows, totalColumns); // testing 
	    displayGen(0);
            
            /*
            // testing alive neighbors
            // prints out the alive neighbors for each cell            
            printf("Number of neighbors for each cell:\n");
            for (r = 0; r < totalRows; r++)
            {
                for(c = 0; c < totalColumns; c++)
                {
                    int num = aliveNeighbors(0, r, c);
                    if (num!=0)
                    {
                        printf("(%i, %i): %i\n", r+1, c+1, num);
                    }
                }
            }
            */

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
    
    printf("end of processing.");
    return(0);
}

// ---------------------------------------------------------------------
// aliveNeighbors 
// 
// PURPOSE: Counts how many of the surrounding cells are ALIVE (= "*") 
// INPUT PARAMETERS: 
// Generation number, cell row number, cell column number
// OUTPUT PARAMETERS:
// Total number of alive neighbors 
// --------------------------------------------------------------------

int aliveNeighbors(int gen, int row, int col)
{
    // PRECONDITIONS: 
    // the generation is valid (validGen()) 
    // 0 <= row <= totalRows, 0 <= col <= totalColumns
    // POSTCONDITIONS: 
    // numAlive is the amount of alive neighbors 
    // 0 <= numAlive <= 8

    validGen(gen);
    
    assert(row >= 0);
    assert( row <= totalRows);

    assert(col >= 0);
    assert(col <= totalColumns);

    int numAlive = 0; // counter
    int r; // row to check
    int c; // column to check
    
    for (r = row-1; r <= row+1; r++)
    {
	for (c = col-1; c <= col+1; c++)
	{
	    if ((r >= 0 && c >= 0) && (r < totalRows && c < totalColumns))
            {
                validCell(gen, r, c);

                if(allGens[gen].board[r][c] == ALIVE)
                {
                    assert(allGens[gen].board[r][c] != DEAD);
                    assert(allGens[gen].board[r][c] == ALIVE);
                    numAlive++;
                }
	    }
	}
    }	

    // so it does not count itself
    if (allGens[gen].board[row][col] == ALIVE)
    {
        
        assert(allGens[gen].board[row][col] != DEAD);
        assert(allGens[gen].board[row][col] == ALIVE);
        numAlive--;
    }
    
    assert(numAlive > 0);
    assert(numAlive <= 8);

    return numAlive;

} // alive Neighbors

// --------------------------------------------------------------------
// displayGen
//
// PURPOSE: Displays the generation
// INPUT PARAMETERS:
// The generation number of the generation to be displayed
// --------------------------------------------------------------------
void displayGen(int gen)
{   
    // precondition: the generation is valid (genValid())
    
    validGen(gen);

    int r;
    int c;
    char decoration[MAX_INPUT];

    assert(gen >= 0);
    assert(gen <= MAX_GENERATIONS);
    printf("Generation %i:\n", gen);

    // make the decorative line
    decoration[0]='+';
    for (c = 1; c <= totalColumns; c++)
    {
	decoration[c] = '-';
    }
    decoration[totalColumns+1] = '+';
    decoration[totalColumns+2] = '\0';

    /** print the generation **/

    printf("%s\n",decoration);
    
    for (r=0; r<totalRows; r++)
    {	
	printf("|");
	for (c = 0; c < totalColumns; c++)
	{
            validCell(gen, r, c);
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
    // PRECONDITION: totalGen is exactly 0  
    // POSTCONDITION: 
    // allGens[0] is valid, totalGen is exactly 1
    // each cell is either ALIVE (= '*') or DEAD (='.')

    // note: I'm not checking that the input is correct (input[c] == 'X' or ' ') before 
    // inserting it because we are told that there will be no errors in input file format
    
    char input[MAX_INPUT];
    int r;
    int c;

    assert(totalGens==0);

    // traverse every cell to fill the first generation
    for (r=0; r<totalRows; r++)
    {
	fgets(input, MAX_INPUT, stdin);
	for (c = 0; c < totalColumns; c++)
	{	
            // make sure r and c are within the valid range
 	    assert(r >= 0);
            assert(r <= totalRows);
            assert(r <= MAX_ROWS);

            assert(c >= 0);
            assert(c <= totalColumns);
            assert(c <= MAX_COLUMNS);
            
            if (input[c] == 'X') 
	    {
		allGens[0].board[r][c] = ALIVE; // = '*'
	    }
	    else // (input[c] == ' ')
	    {
		allGens[0].board[r][c] = DEAD; // = '.'
	    }
            
            // make sure the cell was changed and has valid entries
            assert(allGens[0].board[r][c] != ' ');
            assert(allGens[0].board[r][c] != 'X');
            validCell(0, r, c);
	}
    }
    
    totalGens++; 

    assert(totalGens==1);
    validGen(0);

} // fillInitGen

//---------------------------------------------------------------------------
// validGen
//
// PURPOSE: Invariant function. Validates that given generation meets 
// certain conditions using assertions.
// INPUT PARAMETER: 
// index of the generation to validate.
//--------------------------------------------------------------------------
void validGen(int genNum)
{
    // genNum is withing valid range
    assert(genNum >= 0);
    assert(genNum <= MAX_GENERATIONS);

    // total rows and columns should be greater than 0
    assert(totalRows > 0);
    assert(totalColumns > 0);

    // total rows and columns should not exceed MAX_ROWS or MAX_COLUMNS
    assert(totalRows <= MAX_ROWS);
    assert(totalColumns <= MAX_COLUMNS);

    // make sure the board has been created
    assert(allGens[genNum].board[0][0] == ALIVE || allGens[genNum].board[0][0] == DEAD);

} // validGen 


//---------------------------------------------------------------------------
// validCell
//
// PURPOSE: Invariant function. Validates that given cell meets certain 
// conditions using assertions.
// INPUT PARAMETER: 
// index of the generation to validate, index of cell row and column
//--------------------------------------------------------------------------
void validCell(int gen, int row, int column)
{
    assert(row >= 0);
    assert(row <= totalRows);
    assert(row <= MAX_ROWS);

    assert(column >= 0);
    assert(column <= totalColumns);
    assert(column <= MAX_COLUMNS);

    assert(allGens[gen].board[row][column] == ALIVE || allGens[gen].board[row][column] == DEAD);
} // validCell


//---------------------------------------------------------------------------
// validGame
//
// PURPOSE: Invariant function. Validates that game meets certain 
// conditions to play another round using asseritons. 
//---------------------------------------------------------------------------
void validGame()
{
    assert(totalGens >= 0);
    assert(totalGens < MAX_GENERATIONS);
    
    // totalGens actually reflects the amount of generations
    
    // if generations = 0, then the arrray of all generations should be empty? 
} // validGame

