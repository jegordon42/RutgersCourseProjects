#include <stdio.h>
#include <stdlib.h>

struct HASHnode 
{ 
    int num; 
    struct HASHnode * next;
};

struct HASHnode * HASHarray[10000];

struct HASHnode * search(int num);
struct HASHnode * createHASHnode(int num);
int hashFunction(int num);
void insert(int num);

int main(int argc, char** argv){
    FILE *fptr;
    fptr = fopen(argv[1], "r");

    
    struct HASHnode * node;
    char action[10];
    char stringNum[10];
    int num;
    while(feof(fptr) == 0)
    {
        fscanf(fptr, "%s", action);
        fscanf(fptr, "%s", stringNum);
        num = atoi(stringNum);

        if(action[0] == 'i')
        {
            if(search(num) != NULL)
            {
                printf("duplicate\n");
                continue;
            }
            insert(num); 
            printf("inserted\n");
        }
        else
        {
            if(search(num) == NULL)
                printf("absent\n");
            else
                printf("present\n");
        }
    } 
    return 0;
}

struct HASHnode * search(int num) { 
    int index = hashFunction(num);
    struct HASHnode * node = HASHarray[index];
    while(node != NULL)
    {
        if(node->num == num)
            return node;
        node = node->next;
    }
    return node;
} 

struct HASHnode * createHASHnode(int num) { 
    struct HASHnode * newHASHnode =  (struct HASHnode *) malloc(sizeof(struct HASHnode)); 
    newHASHnode->num = num; 
    newHASHnode->next = NULL; 
    return newHASHnode; 
} 

int hashFunction(int num){
    return num % 10000;
}

void insert(int num){ 
    struct HASHnode * node = createHASHnode(num);
    int index = hashFunction(num);
    node->next = HASHarray[index];
    HASHarray[index] = node;
} 

