#include<stdlib.h>
#include<stdio.h>

char * DeleteSpace(char * field, int index);

int Compare(char value1[], char value2[], char datatype[])
{
	int i;
	int j;
	for (i=0;value1[i] !='\0';i++);
	for (j=0;value2[j] !='\0';j++);
	value1 = DeleteSpace(value1, i- 1);
	value2 = DeleteSpace(value2, j-1);
	if(strcmp(value1, "") == 0)
		return 1;
	if(strcmp(value2, "") == 0)
		return 0;

    if(strcmp(datatype, "string") == 0)
	{
        if(strcmp(value1, value2) <= 0)
            return 1;

        return 0;
    }

    if(strcmp(datatype, "numeric") == 0)
	{
        if(atof(value1) <=  atof(value2))
            return 1;

        return 0;
    }

    if(strcmp(datatype, "dateTime") == 0)
	{
        if(atoi(value1) <= atoi(value2))
            return 1;

        return 0;
    }
	return 0;
}

struct movieNode * Merge(struct movieNode * a, struct movieNode * b, char type[]) {
	if (a == NULL) return b;
	if (b == NULL) return a;

	struct movieNode * curr;
	if (Compare(a->movie[columnIndex], b->movie[columnIndex], type) == 1)
	{
		curr = a;
		curr->next = Merge(a->next, b, type);
	}
	else
	{
		curr = b;
		curr->next = Merge(a, b->next, type);
	}

	return curr;
}

struct movieNode * MergeSort (struct movieNode * curr, char type[]) {
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

	s = MergeSort(s, type);
	mid = MergeSort(curr, type);

	s = Merge(mid, s, type);

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
