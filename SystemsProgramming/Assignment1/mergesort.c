#include<stdlib.h>
#include<stdio.h>

char * DeleteSpace(char * field, int index);

int Compare(char value1[], char value2[])
{
	int i;
	int j;

	for (i=0;value1[i] !='\0';i++);
	for (j=0;value2[j] !='\0';j++);

	if(strcmp(value1, "") == 0)
		return 1;
	if(strcmp(value2, "") == 0)
		return 0;
	
    if(searchColumnType == STRING_TYPE)
	{
		char str1[200];
		char str2[200];
		strcpy(str1, value1);
		strcpy(str2, value2);
		DeleteSpace(str1, i- 1);
		DeleteSpace(str2, j-1);
        if(strcmp(str1, str2) <= 0)
            return 1;

        return 0;
    }

    if(searchColumnType == NUMERIC_TYPE)
	{
        if(atof(value1) <=  atof(value2))
            return 1;

        return 0;
    }

    if(searchColumnType == DATETIME_TYPE)
	{
        if(atoi(value1) <= atoi(value2))
            return 1;
        return 0;
    }

	return 0;
}

struct movieNode * Merge(struct movieNode * a, struct movieNode * b, int sortColumnIndex) {
	if (a == NULL) return b;
	if (b == NULL) return a;

	struct movieNode * curr;
	if (Compare(a->movie[sortColumnIndex], b->movie[sortColumnIndex]) == 1)
	{
		curr = a;
		curr->next = Merge(a->next, b, sortColumnIndex);
	}
	else
	{
		curr = b;
		curr->next = Merge(a, b->next, sortColumnIndex);
	}
	
	return curr;
}

struct movieNode * MergeSort (struct movieNode * curr, int sortColumnIndex) {
	struct movieNode * s = curr;
	struct movieNode * mid = curr;
	int num = 0;
	
	if (curr->next == NULL) return s;
	if (curr == NULL) return NULL;
	
	while (s != NULL)
	{
		s = s->next;
		num++;

		if ((num % 2 == 0 && s != NULL) || (num == 1 && s == NULL))
			mid = mid->next;
	}

	s = mid->next;
	mid->next = NULL;

	s = MergeSort(s, sortColumnIndex);
	mid = MergeSort(curr, sortColumnIndex);

	s = Merge(mid, s, sortColumnIndex);

	return s;
}
char * DeleteSpace(char * field, int index) {
	int j = 0, k = 0, s = 0;

    while(index>=0 && field[index] == ' ')
    {
		if (field[index]!= '\0'){
			field[index ] = '\0';
		}
    	index--;
	}

	while(field[j] != '\0' && field[j] == ' ')
    {
	    j++;
		s = j;
	}

	for(k = 0; field[k+s] != '\0'; k++)
    {
		field[k] = field[k+s];
	}
    field[k] = '\0';

	return field;
}
