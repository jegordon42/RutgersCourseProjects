#include <stdio.h>
#include <stdlib.h>

int isVowel(char letter);

int main(int argc, char** argv){
    
    int i;
    for(i = 1; i <= argc - 1; i++)
    {
        char * string = argv[i];

        int firstVowelIndex;
        for(firstVowelIndex = 0; isVowel(string[firstVowelIndex]) == 0 && string[firstVowelIndex] != '\0'; firstVowelIndex++);

        int length;
        for(length = 0; string[length] != '\0'; length++);

        if(firstVowelIndex == 0)
           printf("%syay ", string);
        else 
        {
            int j;
            for(j = 0; j < firstVowelIndex; j++)
            {
                char temp = string[0];
                int k;
                for(k = 0; k < length - 1; k++)
                    string[k] = string[k + 1];
                string[k] = temp;
            }
            printf("%say ", string);  
        }
    }
        
    return 0;
}

int isVowel(char letter){
    if(letter == 'a' || letter == 'e' ||letter == 'i' || letter == 'o' ||letter == 'u')
        return 1;
    return 0;
}
