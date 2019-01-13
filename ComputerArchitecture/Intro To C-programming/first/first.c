#include <stdio.h>
#include <stdlib.h>
void EvenOddSort(int arr[], int length);

int main(int argc, char** argv){
    FILE *fptr;
    fptr = fopen(argv[1], "r");

    int length = 0;
    fscanf(fptr, "%d", &length);
    int arr[length];

    int i;
    for (i = 0; i < length; i++)
    {
        fscanf(fptr, "%d", &arr[i]);
    }

    fclose(fptr);

    EvenOddSort(arr, length);

    for (i = 0; i < length; i++)
    {
        printf("%d\t", arr[i]);
    }

    return 0;
}

void EvenOddSort(int arr[], int length)
{
    int i;
    int j;
    int smallestIndex; 
    for (i = 0; i < length - 1; i++) 
    { 
        smallestIndex = i; 
        for (j = i+1; j < length; j++) 
        {
            if (arr[j] < arr[smallestIndex]) 
            smallestIndex = j; 
        }
        int temp = arr[smallestIndex];
        arr[smallestIndex] = arr[i];
        arr[i] = temp;
    } 

    int evenCount = 0;
    for (i = 0; i < length; i++)
    {
        if (arr[i] % 2 == 0)
            evenCount++;
    }
    int oddIndex = evenCount;
    int evenIndex = 0;

    int tempArr[length];
    for (i = 0; i < length; i++)
    {
        if (arr[i] % 2 == 1)
        {
            tempArr[oddIndex] = arr[i];
            oddIndex++;
        }
        else
        {
            tempArr[evenIndex] = arr[i];
            evenIndex++;
        }
    }

    for (i = 0; i < length; i++)
        arr[i] = tempArr[i];
}