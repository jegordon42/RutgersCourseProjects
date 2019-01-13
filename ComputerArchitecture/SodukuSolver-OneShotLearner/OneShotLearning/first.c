#include <stdio.h>
#include <stdlib.h>

void GetXandY(FILE *fptr, double **matX, double **matY, int n, int k);
void GetTestMat(FILE *fptr, double **testMat, int n, int k);
double **Transpose(double **matrix, int n, int k);
double ** Multiply(double ** matrix1, double ** matrix2, int row1, int col1, int row2, int col2);
double **Augment(double **matrix, int n);
double **UnAugment(double **matrix, int n);
void Inverse(double **matrix, int n);
void DivideRowByConstant(double **matrix, int row, double constant, int n);
void SubtractRows(double **matrix, int rowToChange, int rowToSubtract, double constant, int n);
void PrintMatrix(double **matrix, int n, int k);
void FreeMatrix(double **matrix, int size);

int main(int argc, char *argv[]) {
    FILE *fptr;
    fptr = fopen(argv[1], "r");

	int k;
    int n;
    fscanf(fptr,"%d",&k);
    fscanf(fptr,"%d",&n);
	k++;

	double **matX = malloc (sizeof (double) * n);
	double **matY = malloc(sizeof (double) * n);
	
	int i;
    for(i = 0; i<n; i++) 
    	*(matX + i) = malloc (sizeof (double) * k);

	for(i = 0; i<n; i++) 
    	*(matY + i) = malloc (sizeof (double));

	GetXandY(fptr, matX, matY, n, k);
	double **transMat = Transpose(matX, n, k);
	double **prodXandXtranspose = Multiply(transMat, matX, k, n, n, k);
	double **augProd = Augment(prodXandXtranspose,k);
	Inverse(augProd, k);
    double **invProd = UnAugment(augProd, k);
	double **invProdTimesXtranspose = Multiply(invProd, transMat, k, k, k, n);
	double **matW = Multiply(invProdTimesXtranspose, matY, k, n, n, 1);

	int numTests;
	fptr = fopen(argv[2], "r");
	fscanf(fptr,"%d",&numTests);
	
	double **testMat = malloc(sizeof (double) * numTests);
	
    for(i = 0; i<numTests; i++) 
    	*(testMat + i) = malloc (sizeof (double) * k);

	GetTestMat(fptr, testMat, numTests, k);
	double **result = Multiply(testMat, matW, numTests, k, k, 1);

	PrintMatrix(result, numTests, 1);

	FreeMatrix(matX, k);
	FreeMatrix(matY, 1);
	FreeMatrix(transMat, k);
	FreeMatrix(prodXandXtranspose, k);
	FreeMatrix(augProd, k);
	FreeMatrix(invProd, k);
	FreeMatrix(invProdTimesXtranspose, k);
	FreeMatrix(matW, 1);
	FreeMatrix(result, 1);

	return 0;
 }

void GetXandY(FILE *fptr, double **matX, double **matY, int n, int k)
{
	int i;
	int j;

	char *curRow = malloc(sizeof(char) * 10000);
	char *curNum = malloc(sizeof(char) * 100);
	int rowIndex;
	int numIndex;

   	for(i=0;i<n;i++)
	{
		fscanf(fptr,"%s", curRow);
		rowIndex = 0;
		matX[i][0] = 1;
		j = 1;
    	while(j < k)
		{
			for(numIndex = 0; curRow[rowIndex] != ','; numIndex++)
			{
				curNum[numIndex] = curRow[rowIndex];
				rowIndex ++;
			}
			curNum[numIndex] = '\0';
			matX[i][j] = atof(curNum);
			rowIndex++;
			j++;
		}
		for(numIndex = 0; curRow[rowIndex] != '\0'; numIndex++)
		{
			curNum[numIndex] = curRow[rowIndex];
			rowIndex ++;
		}
		curNum[numIndex] = '\0';
		matY[i][0] = atof(curNum);
	}

	free(curRow);
	free(curNum);

	return;
}

void GetTestMat(FILE *fptr, double **testMat, int n, int k)
{
	int i;
	int j;

	char *curRow = malloc(sizeof(char) * 1000);
	char *curNum = malloc(sizeof(char) * 100);
	int rowIndex;
	int numIndex;
	
   	for(i=0;i<n;i++)
	{
		fscanf(fptr,"%s", curRow);
		rowIndex = 0;
		testMat[i][0] = 1;
		
		j = 1;
    	while(j < k - 1)
		{
			for(numIndex = 0; curRow[rowIndex] != ','; numIndex++)
			{
				curNum[numIndex] = curRow[rowIndex];
				rowIndex ++;
			}
			curNum[numIndex] = '\0';
			testMat[i][j] = atof(curNum);
			rowIndex++;
			j++;
		}
		for(numIndex = 0; curRow[rowIndex] != '\0'; numIndex++)
		{
			curNum[numIndex] = curRow[rowIndex];
			rowIndex ++;
		}
		curNum[numIndex] = '\0';
		testMat[i][j] = atof(curNum);
	}
	
	free(curRow);
	free(curNum);

	return;
}



double **Transpose(double **matrix, int n, int k)
{
	int i;
	int j;

	double **transMat = malloc (sizeof (double) * k);
    for(i = 0; i<k; i++) 
    	*(transMat + i) = malloc (sizeof (double) * n);
	
	for(i=0;i<n;i++)
       for(j=0;j<k;j++)
       		transMat[j][i] = matrix[i][j];

	return transMat;
}

double ** Multiply(double ** matrix1, double ** matrix2, int row1, int col1, int row2, int col2)
{
    int i;
	int j;
	int k;

    double **prodMat = malloc (sizeof (double) * row1);
	for(i = 0; i < row1; i++) 
    	*(prodMat + i) = malloc (sizeof (double) * col2);

    for (i = 0; i < row1; i++)
        for (j = 0; j < col2; j++)
		{
			prodMat[i][j] = 0;
            for (k = 0; k < col1; k++)
               prodMat[i][j] += matrix1[i][k] * matrix2[k][j];
		}
	return prodMat;
}

double **Augment(double **matrix, int n)
{
	int i;
	int j;

	double **augMat = malloc (sizeof (double) * n);
    for(i = 0; i < n; i++) 
    	*(augMat + i) = malloc (sizeof (double) * n * 2);
	
	for(i=0;i<n;i++)
	{
       for(j=0;j<n;j++)
       		augMat[i][j] = matrix[i][j];

		for(j = n; j < n * 2; j++)
		{
				if(j - n == i)
					augMat[i][j] = 1;
				else
					augMat[i][j] = 0;
		}
	}

	return augMat;
}

double **UnAugment(double **matrix, int n)
{
	int i;
	int j;

	double **unAugMat = malloc (sizeof (double) * n);
    for(i = 0; i < n; i++) 
    	*(unAugMat + i) = malloc (sizeof (double) * n);
	
	for(i=0;i<n;i++)
       for(j=0;j<n;j++)
       		unAugMat[i][j] = matrix[i][j + n];

	return unAugMat;
}

void Inverse(double **matrix, int n)
{
	int i;
	int j;

	for(i = 0; i < n ; i++)
	{
		for(j=0; j< n; j++)
		{
			if(j > i)
				continue;
			else if(j == i)
				DivideRowByConstant(matrix, j, matrix[i][j], n * 2);
			else
				SubtractRows(matrix, i, j, matrix[i][j], n * 2);
		}
	}
	
	for(i = n - 2; i >= 0 ; i--)
		for(j = n - 1; j > i; j--)
			SubtractRows(matrix, i, j, matrix[i][j], n * 2);

	return;
}

void DivideRowByConstant(double **matrix, int row, double constant, int n)
{
	int i;
	for(i = 0; i < n; i++)
		matrix[row][i] = matrix[row][i] / constant;

	return;
}

void SubtractRows(double **matrix, int rowToChange, int rowToSubtract, double constant, int n)
{
	int i;
	for(i = 0; i < n; i++)
		matrix[rowToChange][i] -= matrix[rowToSubtract][i] * constant;

	return;
}

void PrintMatrix(double **matrix, int n, int k)
{
	int i;
	int j;
	for(i=0;i<n;i++)
	{
       for(j=0;j<k;j++)
		{
           	printf("%0.0lf", matrix[i][j]);
		}
		printf("\n");
	}
}
  
void FreeMatrix(double **matrix, int size)
{
	int i = 0;
	for(i = 0; i < size; i++)
    	free(matrix[i]);
	free(matrix);
}
