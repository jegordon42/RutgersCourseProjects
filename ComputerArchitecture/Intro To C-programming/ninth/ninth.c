#include <stdio.h>
#include <stdlib.h>
struct BSTnode 
{ 
    int num; 
    struct BSTnode *leftChild;
    struct BSTnode *rightChild; 
};
struct BSTnode * createBSTnode(int num);
struct BSTnode * insert(struct BSTnode * node, int num); 
struct BSTnode * deleteBSTnode(struct BSTnode * node, int num);
struct BSTnode * search(struct BSTnode* node, int num);
struct BSTnode * getSmallestNode(struct BSTnode * node);
int getHeight(struct BSTnode* node, int num);
void freeTheTree(struct BSTnode * node);

int main(int argc, char** argv){
    FILE *fptr;
    fptr = fopen(argv[1], "r");

    struct BSTnode * BSTroot = NULL; 
    struct BSTnode * node;

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
            if(search(BSTroot, num) != NULL)
            {
                printf("duplicate\n");
                continue;
            }
            if(BSTroot == NULL)
                BSTroot = insert(BSTroot, num); 
            else
                insert(BSTroot, num);
            printf("inserted %d\n", getHeight(BSTroot, num));
        }
        else if (action[0] == 's')
        {
            node = search(BSTroot, num);
            if(node == NULL)
                printf("absent\n");
            else
                printf("present %d\n", getHeight(BSTroot, num));
        }
        else if (action[0] == 'd')
        {
            if(search(BSTroot, num) == NULL)
            {
                printf("fail\n");
                continue;
            }
            deleteBSTnode(BSTroot, num);
            printf("success\n");
        }
    } 
    freeTheTree(BSTroot);
    return 0;
}

struct BSTnode * createBSTnode(int num) { 
    struct BSTnode * newBSTnode =  (struct BSTnode *) malloc(sizeof(struct BSTnode)); 
    newBSTnode->num = num; 
    newBSTnode->leftChild = NULL;
    newBSTnode->rightChild = NULL; 
    return newBSTnode; 
} 

struct BSTnode * insert(struct BSTnode * node, int num){ 
    if (node == NULL) 
        return createBSTnode(num); 
  
    if (num < node->num) 
        node->leftChild = insert(node->leftChild, num); 
    else if (num > node->num) 
        node->rightChild = insert(node->rightChild, num);    
  
    return node; 
} 

struct BSTnode * deleteBSTnode(struct BSTnode * node, int num){ 
    if (node == NULL) 
        return node; 
    if(num == node->num)
    {
        if (node->rightChild == NULL) 
        { 
            struct BSTnode * left = node->leftChild; 
            free(node); 
            return left; 
        } 
        if (node->leftChild == NULL) 
        { 
            struct BSTnode * right = node->rightChild; 
            free(node); 
            return right; 
        } 

        struct BSTnode * smallestInRight = getSmallestNode(node->rightChild); 
        node->num = smallestInRight->num; 
        node->rightChild = deleteBSTnode(node->rightChild, smallestInRight->num); 
    }
    else if (num < node->num) 
        node->leftChild = deleteBSTnode(node->leftChild, num); 
    else if (num > node->num) 
        node->rightChild = deleteBSTnode(node->rightChild, num); 

    return node; 
} 

struct BSTnode * search(struct BSTnode * node, int num) { 
    if (node == NULL || node->num == num) 
       return node; 
     
    if (node->num < num) 
       return search(node->rightChild, num); 

    return search(node->leftChild, num); 
} 

struct BSTnode * getSmallestNode(struct BSTnode * node){ 
    if(node == NULL)
        return node;
    if(getSmallestNode(node->leftChild) == NULL)
        return node;
    return getSmallestNode(node->leftChild); 
} 

int getHeight(struct BSTnode * node, int num){ 
    if (node == NULL) 
        return 0;
    if(node->num == num)
       return 1; 
     
    if (node->num < num) 
       return 1 + getHeight(node->rightChild, num); 

    return 1 + getHeight(node->leftChild, num); 
} 

void freeTheTree(struct BSTnode * node){
    if (node != NULL) 
    { 
        freeTheTree(node->leftChild); 
        freeTheTree(node->rightChild); 
        free(node);
    }   
}