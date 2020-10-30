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

//-----------------------------------------------
// macro definitions

#ifdef NDEBUG
    #define PRINT_X_GENS 10
#else
    #define PRINT_X_GENS 0
#endif 

#define MAX_INPUT 80
#define MAX_GENERATIONS 251

#define MAX_COLUMNS 60
#define MAX_ROWS 60
#define MAX_NEIGHBORS 8

#define ALIVE '*'
#define DEAD '.'

//-----------------------------------------------
// fuction prototypes

void loadUniverse();
void printGen(int);
void printUniverse();
void playGame();
void nextGen(); 
int aliveNeighbors(int, int, int);
int cycleCheck();

// invariant function prototypes
void validCell(int, int, int);
void validGen(int);
void validGame();

//------------------------------------------------
// file variables 

typedef struct Generations 
{
    char board[MAX_ROWS][MAX_COLUMNS];
} gen;

gen allGens[MAX_GENERATIONS];

int totalGens = 0;
int currentGen;

int totalRows; 
int totalColumns;

int gameOver;

// --------------------------------------------------------

int main(void)
{
    char caseName[MAX_INPUT];
    char input[MAX_INPUT];

    while(fgets(caseName, MAX_INPUT, stdin) != NULL)
    {
        // read in one case at a time 
	if(caseName[0] == '*')
	{
            scanf("%d %d", &totalRows, &totalColumns); // read in second line for board dimensions
            printf("%s",caseName); 

	    fgets(input,MAX_INPUT, stdin); // read in the second line with fgets
            
            // make sure board size is valid
            assert(totalRows <= MAX_ROWS);
            assert(totalColumns <= MAX_COLUMNS);
	    
            if ((totalRows > 0) && (totalRows <= MAX_ROWS) && (totalColumns > 0) && (totalColumns <= MAX_COLUMNS))
            {
            loadUniverse();
            printGen(currentGen);
            playGame();
            printUniverse();
            }
            else 
            {
                printf("Your initial board (%iX%i) doesn't fit the proper dimensions.\n", totalRows, totalColumns);
                printf("Board minimum: 1X1 | Board maximum: 60X60\n\n");
            }
        }
        
    } // read again until end of file
    
    printf("\nend of processing\n");
    return 0;
}

// ------------------------------------------------------------------
// loadUniverse
// 
// PURPOSE: Fills the first generation for any case
// ------------------------------------------------------------------
void loadUniverse()
{
    // PRECONDITION: 0 < totalRows <= MAX_ROWS, 0 < totalColumns <= MAX_COLUMNS  
    // POSTCONDITION: 
    // allGens[0] is valid, totalGen is exactly 1
    // each cell is either ALIVE (= '*') or DEAD (='.')

    // note: I'm not checking that the input is correct (input[c] == 'X' or ' ') before 
    // inserting it because we are told that there will be no errors in input file format
    
    char input[MAX_INPUT];
    int r; // row
    int c; // column

    totalGens = 0;
    currentGen = 0; // set current gen as one
    gameOver = 0; // initialize gameOver as false

    // traverse every cell to fill the first generation
    for (r = 0; r < totalRows; r++)
    {
        // read one row at a time
	fgets(input, MAX_INPUT, stdin);

        // fill all the columns in that row
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
            assert(allGens[currentGen].board[r][c] != ' ');
            assert(allGens[currentGen].board[r][c] != 'X');
            validCell(currentGen, r, c);
	}
    }
    
    totalGens++; // update totalGens 
    
    // invariant checks
    assert(totalGens==1);
    assert(currentGen==0);
    validGen(0);

} // loadUniverse

// --------------------------------------------------------------------
// printGen
//
// PURPOSE: Displays the generation
// INPUT PARAMETERS:
// The generation number of the generation to be displayed
// --------------------------------------------------------------------
void printGen(int gen)
{   
    // precondition: 0 <= gen <= MAX_GENERATIONS, the generation is valid 
    // postcondition: the generation is still valid 
    
    validGen(gen);

    int r; // row
    int c; // column
    char decoration[MAX_INPUT];

    // gen is within valid range
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
            // validate each cell before printing
            validCell(gen, r, c);
	    printf("%c",allGens[gen].board[r][c]);
	}
    printf("|\n");
    } 

    printf("%s\n", decoration);

    // validate invariant 
    // make sure the generation is still valid
    validGen(gen);
    
} // printGen

//--------------------------------------------------------------------
// printUniverse
//
// PURPOSE: prints the universe (game). Amount of generations printed
// is according to if compiled with NDEBUG or not.
//--------------------------------------------------------------------
void printUniverse()
{
    int currGen = 0; // current gen

    // if complied with NDEBUG
    // so we only print the last x generations
    if (PRINT_X_GENS)
    {
     currGen = totalGens - PRINT_X_GENS;
    }

    // print the universe (game)
    while ((currGen < totalGens) && (currGen >= 0))
    {
        printGen(currGen);
        currGen++;
    }

} // print Universe

//---------------------------------------------------------------------
// playGame
//
// PURPOSE: Play the game 
//---------------------------------------------------------------------
void playGame()
{
    validGame();

    while(!gameOver)
    {
        nextGen();
        if ((totalGens >= MAX_GENERATIONS) || (cycleCheck()))
        {
            gameOver = 1;
        }
        validGame();
    }
 
 } //playGame

// ---------------------------------------------------------------------
// nextGen
//
// PURPOSE: Creates the next generation of the game by the status of each
// cell in the old generation
// INPUT PARAMETERS: index of old generation
// ---------------------------------------------------------------------
void nextGen()
{
    // PRECONDITIONS: 
    // allGens[oldGen] is a valid generation, totalGens < MAX_GENERATIONS
    // currentGens >= 1
    // POSTCONDITIONS: newGen is valid, currentGen has been incremented by
    // exactly one,  totalGens has been incremented by exactly one
    
    int r; // row
    int c; // column
    int pastNeighbors; // counts how many are alive
    int oldGen = currentGen; 
    
    currentGen++;

    validGen(oldGen);
    assert(totalGens < MAX_GENERATIONS);
    
    assert(currentGen >= 1);
    assert(currentGen == oldGen + 1); 
    assert(currentGen <= MAX_GENERATIONS);

    // go through every cell
    for(r = 0; r < totalRows; r++)
    {
        for(c = 0; c < totalColumns; c++)
        {
            pastNeighbors = aliveNeighbors(oldGen, r, c); // = how many alive neightbors the past cell had 
            
            assert((pastNeighbors >= 0) || (pastNeighbors <= MAX_NEIGHBORS)); // check that it was a valid amount of ALIVE neighbors
            validCell(oldGen, r, c); // was the old cell valid?

            // Determine if current cell should be ALIVE or DEAD
            if (allGens[oldGen].board[r][c] == ALIVE)
            {
                // the old cell was ALIVE 

                if ((pastNeighbors != 2) && (pastNeighbors != 3)) // pastNeighbor = 1, 4, 5, 6, 7, or 8
                {
                    allGens[currentGen].board[r][c] = DEAD; // new cell is DEAD
                }
                else // pastNeighbor = 2 or 3
                {
                    allGens[currentGen].board[r][c] = ALIVE; // new cell is ALIVE
                }

            }
            else 
            {
                // the old cell was DEAD

                if (pastNeighbors == 3) 
                {
                    allGens[currentGen].board[r][c] = ALIVE; // new cell is ALIVE
                }
                else // (past neighbors != 3)
                {
                    allGens[currentGen].board[r][c] = DEAD; // new cell is DEAD
                }
            }
            
            // validate this new cell
            validCell(currentGen, r, c); 

        } // column
    } //row

    totalGens++;

    // is currentGen correct and valid?
    assert(currentGen >= 1);
    assert(currentGen == oldGen + 1); 
    assert(currentGen <= MAX_GENERATIONS);
    
    // check invariant, are both generations still valid?
    validGen(oldGen);
    validGen(currentGen);
    
    // is the total generation correct and valid? 
    assert(totalGens == currentGen + 1);
    assert(totalGens <= MAX_GENERATIONS);

} //nextGen

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
    // the generation is valid, 0 <= gen < MAX_GENERATIONS  
    // 0 <= row <= totalRows, 0 <= col <= totalColumns
    // POSTCONDITIONS: 
    // the generation is still valid 
    // 0 <= numAlive <= MAX_NEIGHBORS (=8)

    assert(0 <= gen);
    assert(gen < MAX_GENERATIONS);
    validGen(gen);
    
    assert(row >= 0);
    assert( row <= totalRows);

    assert(col >= 0);
    assert(col <= totalColumns);

    validCell(gen, row, col);

    int numAlive = 0; // counter to track number of neighbors that are ALIVE
    int r; // row 
    int c; // column 
    
    // check each cell
    for (r = row-1; r <= row+1; r++)
    {
	for (c = col-1; c <= col+1; c++)
	{
          
          // only check neighbors that are within the board 
	    if ((r >= 0 && c >= 0) && (r < totalRows && c < totalColumns))
            {
                validCell(gen, r, c);

                // if the neighbor is alive
                if(allGens[gen].board[r][c] == ALIVE)
                {
                    assert(allGens[gen].board[r][c] != DEAD);
                    assert(allGens[gen].board[r][c] == ALIVE);
                    numAlive++; // increment numAlive
                }

                // check invariant, cell is still valid
                validCell(gen,r,c);
	    } 
	
        } // c
    } // r	

    // if the cell we are checking was ALIVE itself
    // then we double counted so numAlive--
    if (allGens[gen].board[row][col] == ALIVE)
    {
        
        assert(allGens[gen].board[row][col] != DEAD);
        assert(allGens[gen].board[row][col] == ALIVE);
        numAlive--;
    }
    
    // make sure numAlive is within valid range
    assert(numAlive >= 0);
    assert(numAlive <= MAX_NEIGHBORS);

    // check invariants 
    validGen(gen);

    return numAlive;

} // aliveNeighbors

//----------------------------------------------------------------------
// cycle
//
// PURPOSE: find out if a cycle has occured and print the range it 
// occured between
// OUTPUT PARAMETER: 
// returns 1 if cycle is found, 0 if no cycle is found  
//----------------------------------------------------------------------
int cycleCheck()
{
    // PRECONDITIONS: 
    // 0 < currentGen <= MAX_GENERATIONS, current gen is valid, validGame(),
    // POSTCONDITIONS: 
    // validGame, each generation checked is still valid
    // current generation is still valid
    
    int cycleFound = 0; // will act as a boolean
    int i = 0; // index
    int r; // row
    int c; // column

    assert(currentGen > 0);
    assert(currentGen <= MAX_GENERATIONS);

    validGame();
    validGen(currentGen);

    while((!cycleFound) && (i < currentGen))
    {
        // make sure conditions are true
        assert(cycleFound == 0);
        assert(i >= 0);
        assert(i < currentGen);

        // make sure currentGen is correct
        assert(currentGen < MAX_GENERATIONS);
        assert(currentGen == totalGens - 1);

        // validate index gen before comparing
        validGen(i);

        //assume cycle exists 
        cycleFound = 1;
        assert(cycleFound != 0);

        // check each cell. CycleFound will turn false if a cell doesn't match
        for (r = 0; r < totalRows; r++)
        {
            for (c = 0; c < totalColumns; c++)
            {
                
                if(allGens[i].board[r][c] != allGens[currentGen].board[r][c]) // contents of index cell != contents of current cell
                {
                   // make sure both cells are valid
                   validCell(i,r,c);
                   validCell(currentGen,r,c);
                    
                   cycleFound = 0;
                   assert(cycleFound == 0);
                }
            
            }
        }
        
        // invariant check. is the gen at index still valid?
        validGen(i);
        
        i++;
    }

    i--; // to account for an extra addition at the end
    
    // invariant check, is the current generation still valid?
    validGen(currentGen);

    
    if (cycleFound)
    {
        assert(cycleFound != 0);

        assert(i >= 0);
        assert(i < currentGen);
        
        printf("A cycle was found between generation %i and generation %i\n", i, currentGen);
    }
    
    return cycleFound; 
} //cycleCheck

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
    // row and column are within valid range
    assert(row >= 0);
    assert(row <= totalRows);
    assert(row <= MAX_ROWS);

    assert(column >= 0);
    assert(column <= totalColumns);
    assert(column <= MAX_COLUMNS);

    // the cell is either ALIVE OR DEAD
    assert(allGens[gen].board[row][column] == ALIVE || allGens[gen].board[row][column] == DEAD);

} // validCell

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
    int r; // row
    int c; // column

    // genNum is withing valid range
    assert(genNum >= 0);
    assert(genNum <= MAX_GENERATIONS);

    // total rows and columns should be greater than 0
    assert(totalRows > 0);
    assert(totalColumns > 0);

    // total rows and columns should not exceed MAX_ROWS or MAX_COLUMNS
    assert(totalRows <= MAX_ROWS);
    assert(totalColumns <= MAX_COLUMNS);
    
    // check that all the cells have valid entries
    for(r = 0; r < totalRows; r++)
    {
        for(c = 0; c < totalColumns; c++)
        {
            assert(allGens[genNum].board[r][c] == ALIVE || allGens[genNum].board[r][c] == DEAD);
        }
    }
} // validGen 

//---------------------------------------------------------------------------
// validGame
//
// PURPOSE: Invariant function. Validates that game meets certain 
// conditions to play another round using asseritons. 
//---------------------------------------------------------------------------
void validGame()
{
    assert(totalGens >= 0);
    assert(totalGens <= MAX_GENERATIONS);
    assert(currentGen >= 0);
    assert(currentGen < MAX_GENERATIONS);
} // validGame

