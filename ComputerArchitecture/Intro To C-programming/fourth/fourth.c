#include <stdio.h>
#include <stdlib.h>
void multiply(int* first, int* second, int r1, int c1, int r2, int c2);
int * getMatrix(FILE * fptr, int rows, int cols);

int main(int argc, char * argv[])
{
    FILE *fptr = NULL;
    int *first;
    int *second;
    int row1, col1, row2, col2;
    

    if (argc != 2)
    {
        printf("not enough args\n");
        return 1;
    }
    if ((fptr = fopen(argv[1], "r")) == NULL)
    {
        printf("error");
        return 1;
    }    
    
    fscanf(fptr, "%d %d", &row1, &col1);
    first = getMatrix(fptr, row1, col1);
    
	fscanf(fptr, "%d %d", &row2, &col2);
    second = getMatrix(fptr, row2, col2);

    if (col1 != row2) 
	{
		printf("bad-matrices");
		return 0;
	}

    multiply(first, second, row1, col1, row2, col2);
	
	free(first);
	free(second);
    return 0;
}

int * getMatrix(FILE * fptr, int rows, int cols)
{
	int i, j;
	int * matrix = (int *)malloc(rows * cols * sizeof(int));
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
        {
            fscanf(fptr, "%d", (matrix + i*cols +j));
        }
    }
	return matrix;
}
void multiply(int* first, int* second, int row1, int col1, int row2, int col2)
{
    int i, j, k;
    int * result;
    result = (int *)malloc(row1 * col2 * sizeof(int));
    for (i = 0; i < row1; i++)
        for (j = 0; j < col2; j++)
            for (k = 0; k < col1; k++)
               *(result + i*col2+ j) += *(first + i*col1 +k) * *(second + k*col2 +j);

    for (i = 0; i < row1; i++)
	{
        for (j = 0; j < col2; j++)
            printf("%d\t", *(result + i*col2 +j));
   
        printf("\n");
    }

	free(result);
}
