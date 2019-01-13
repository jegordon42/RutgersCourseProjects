#include <stdio.h>
#include <stdlib.h>
int main(int argc, char** argv){
    
    int i;
    for(i = 1; i <= argc - 1; i++)
    {
        char * string = argv[i];

        int j;
        for(j = 0; string[j] != '\0'; j++);

        string[0] = string[j-1];
        string[1] = '\0';
        printf("%s", string);
    }
        
    return 0;
}
