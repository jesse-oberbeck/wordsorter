#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compare(const void *a, const void *b)
{
    
    return(*(int*)a - *(int*)b);
}


int main(void)
{
    int testing[5] = {8, 9, 0, 5, 1};
    qsort(testing, 5, sizeof(int), compare);
    for(int i = 0; i < 5; i++){
        printf("%d, ", testing[i]);
    }
    
    /*Get size of file*/
    FILE *words = fopen("sorttest", "r");
    long start = ftell(words);
    fseek(words, 0, SEEK_END);
    long end = ftell(words);
    int filesize = (end - start) + 1;
    printf("\nfilesize: %i\n", filesize);
    fseek(words, 0, SEEK_SET);
    
    char *contents = malloc(filesize);
    fread(contents, sizeof(char), filesize, words);
    fclose(words);
    
    //Make empty array for divided string.
    char *content_array = malloc(filesize);

    //WORDCOUNT
    char *splitstring = strtok(contents, " \n");
    int wordcount = 0;
    while(splitstring != NULL){
        wordcount++;
        splitstring = strtok(NULL, " \n");
    }
    printf("wordcount: %d\n", wordcount);

}
