#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    FILE *fptr;

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

    int n;
    fscanf(fptr,"%d",&n);
    
    
	int **matrix;
    matrix = malloc (sizeof (int *) * n);

	int i;
    for(i = 0; i<n; i++) 
    	*(matrix + i) = malloc (sizeof (int) * n);

  int j;
   for(i=0;i<n;i++)
       for(j=0;j<n;j++)
           fscanf(fptr,"%d",&matrix[i][j]);


int isNot=0;
int k;
for(i = 0; i<n; i++)
{
    for(j = 0; j<n;j++)
    {
      int temporary = matrix[i][j];
      for(k=j+1; k<n; k++)
        if (temporary == matrix[i][k])
          isNot =1;
    }
  }

   int diagonalsum = 0;
   for (i = 0; i < n; i++) 
      for (j = 0; j < n; j++) 
         if (i == j)
            diagonalsum += matrix[i][j];



   int diagonalsum2 = 0;
   for (i = 0; i < n; i++) 
      for (j = 0; j < n; j++) 
         if (j == n-i-1)
            diagonalsum2 += matrix[i][j];

   int ismagic = 0;
   int columnsum = 0;
   int firstcolumnsum=0;
   for (j = 0; j < n; j++) 
         firstcolumnsum +=  matrix[j][0];

   for (i = 0; i < n; i++) 
   {
       	columnsum = 0;
      	for (j = 0; j < n; j++) 
        	columnsum +=  matrix[j][i];

        if(columnsum != firstcolumnsum)
          ismagic = 0;
   }

   
   int rowsum = 0;
   int firstrowsum = 0;
   for(j=0;j<n;j++)
    	firstrowsum += matrix[0][j];

   for (i = 0; i < n; i++) 
   {
      rowsum = 0;
      for (j = 0; j < n; j++) 
         rowsum += matrix[i][j];

      if(firstrowsum != rowsum)
        ismagic =0;
   }

   if (isNot != 1&& columnsum==rowsum && diagonalsum2==columnsum && diagonalsum == diagonalsum2)
            ismagic = 1;

  if(ismagic == 0)
	printf("not-magic\n");
  else
    printf("magic\n");

   for (i = 0; i < n; i++)
       free (matrix[i]);
   free (matrix);  
   return 0;

 }
  
