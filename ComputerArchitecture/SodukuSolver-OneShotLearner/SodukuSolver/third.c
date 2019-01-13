#include <stdio.h>
#include <stdlib.h>

int Solve(char **gameBoard);
int IsValid(char **gameBoard);
void GetEmptyCell(char **gameBoard, int *row, int *col);
void PrintGameBoard(char **gameBoard);

int main(int argc, char *argv[])
{
    FILE *fptr;
    fptr = fopen(argv[1], "r");

	char **gameBoard = malloc (sizeof (char*) * 9);

    int row;
    int col;

    for(row = 0; row < 9; row++) 
    	*(gameBoard + row) = malloc (sizeof (char*) * 9);

    char curCell[2];

    row = 0;
    for(col = 0; fscanf(fptr,"%s", curCell) == 1; col++)
    {
        if(col == 9)
        {
            col = 0;
            row++;
        }
        
        gameBoard[row][col] = curCell[0];
    }

    if(Solve(gameBoard) == 1)
    	PrintGameBoard(gameBoard);
	else
		printf("no-solution");
    return 0;
}

int Solve(char **gameBoard)
{
    if(IsValid(gameBoard) == 0)
        return 0;

    int row;
    int col;
    GetEmptyCell(gameBoard, &row, &col);

    if(row == -1 && col == -1)
        return 1;

    int testInput;
    char testInputString[2];
    for(testInput = 1; testInput <= 9; testInput++)
    {
        sprintf(testInputString, "%d", testInput);
        gameBoard[row][col] = testInputString[0];

        if(Solve(gameBoard) == 1)
            return 1;
    }
    gameBoard[row][col] = '-';

    return 0;
}

int IsValid(char **gameBoard)
{
    int row;
    int col;
    int i;
    int j;

    for(row = 0; row < 9; row++)
        for(col = 0; col < 9; col++)
            if(gameBoard[row][col] != '-')
                for(j = col + 1; j < 9; j++)
                    if(gameBoard[row][col] == gameBoard[row][j])
                        return 0;
            
    for(col = 0; col < 9; col++)
        for(row = 0; row < 9; row++)
            if(gameBoard[row][col] != '-')
                for(i = row + 1; i < 9; i++)
                    if(gameBoard[row][col] == gameBoard[i][col])
                        return 0;
           
    
    int rowOffSet;
    int colOffSet;
    for(rowOffSet = 0; rowOffSet < 9; rowOffSet = rowOffSet + 3)
        for(colOffSet = 0; colOffSet < 9; colOffSet = colOffSet + 3)
            for(row = 0; row < 3; row++)
                for(col = 0; col < 3; col++)
                    if(gameBoard[row + rowOffSet][col + colOffSet] != '-')
                        for(i = 0; i < 3; i++)
                            for(j = 0; j < 3; j++)
                                if(i != row || j != col)
                                    if(gameBoard[row + rowOffSet][col + colOffSet] == gameBoard[i + rowOffSet][j + colOffSet])
                                        return 0;

    return 1;
}

void GetEmptyCell(char **gameBoard, int *row, int *col)
{
	for(*row = 0; *row < 9; *row = *row + 1)
       for(*col = 0; *col < 9; *col = *col + 1)
       {
           if(gameBoard[*row][*col] == '-')
                return;
       }
    
    *row = -1;
    *col = -1;
    return;
}

void PrintGameBoard(char **gameBoard)
{
	int row;
	int col;
	for(row = 0; row < 9; row++)
	{
       for(col = 0; col < 9; col++)
       {
            if(col == 8)
                printf("%c", gameBoard[row][col]);
            else
                printf("%c\t", gameBoard[row][col]);
       }
       if(row != 8)
		    printf("\n");
	}
}
