#include <stdio.h>
#include <stdlib.h>

struct node
{
    int num;
    struct node *next;
};

struct node *delete (struct node *front, int val);
struct node *insert(struct node *front, int val);
struct node *createNode(int val);

int main(int argc, char *argv[])
{
    FILE *fptr = NULL;

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
 
	struct node *front = NULL;
    int val;
    char action;
    while (fscanf(fptr, "%c %d", &action, &val) != EOF)
    {
        if (action == 'd')
		{
			if(front == NULL)
				continue;
			if(front->num == val)
				front = delete(front, val);
			else
				delete(front, val);
		}
        else if (action == 'i') 
		{
				if(front == NULL)
					front = createNode(val);
				else if(front->num > val)
				{
					struct node *newNode = createNode(val);
					newNode->next = front;
					front = newNode;
				}
				else
					front = insert(front, val);
		}
    } 
	    
	struct node *counter = front;
	int length;
    for (length = 0; counter != NULL; length++)
        counter = counter->next;

    printf("%d\n", length);
    
	struct node *printer = front;
    if (printer != NULL)
    {
        printf("%d", printer->num);
        printer = printer->next;
    }
    while (printer != NULL)
    {
        printf("\t%d", printer->num);
        printer = printer->next;
    }

    if (front != NULL) 
		printf("\n");
	
	struct node * cur;
	while(front != NULL)
	{
			cur = front;
			front = front->next;
			free(cur);
	}
	fclose(fptr);

    return 0;
}

struct node *delete (struct node *front, int val)
{
	struct node *cur = front;
	if(cur == NULL)
		return NULL;
	if(cur->num > val)
		return NULL;
	if(cur->num < val)
	{
		struct node *nextNode = cur->next;
		if(nextNode == NULL)
			return NULL;
		int nextNodeData = nextNode->num;
		if(nextNodeData == val)
		{
				cur->next = nextNode->next;
				return cur;
		}
		delete(nextNode,val);
	}
	if(cur->num == val)
		return cur->next;
	return NULL;
}

struct node *insert(struct node *front, int val)
{
	if(front == NULL)
		return createNode(val);
	if(front->num == val)
		return front;
	if(front->num < val)
		front->next = insert(front->next, val);
	else
	{
		struct node *nextNode = front->next;
		if(nextNode == NULL)
		{
			if(front->num < val)
				front->next = createNode(val);
			else
			{
					struct node *newNode = createNode(val);
					newNode->next = front;
					front = newNode;
			}
		}
		else if(nextNode->num > val)
		{
				struct node *newNode = createNode(val);
				front->next = newNode;
				newNode->next = nextNode;
		}
	}
	return front;
}

struct node *createNode(int val)
{
		struct node *newNode = (struct node *)malloc(sizeof(struct node));
    	newNode->num = val;
    	newNode->next = NULL;
		return newNode;
}
