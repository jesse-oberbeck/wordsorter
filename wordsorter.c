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
    
    //Read in file.
    char *contents = malloc(filesize);
    char *contents2 = malloc(filesize);
    fread(contents, sizeof(char), filesize, words);
    strcpy(contents2, contents);
    printf("contents: %s\n", contents);
    //fclose(words);

    //WORDCOUNT
    char *word = strtok(contents, " \n");
    int wordcount = 0;
    while(word != NULL){
        wordcount++;
        word = strtok(NULL, " \n");
    }
    printf("wordcount: %d\n", wordcount);
    
    //printf("1 contents: %s\n", contents);
    
    
    
    
    
    char **content_array = (char**)malloc(wordcount + 1);
    //TODO: Nullify last byte of array of strings.
    *content_array[wordcount] = '\0';
    puts("test");
    char *splitstring = strtok(contents2, " \n");
    //printf("\n2 contents: %s\n", contents2);
    
    int i = 0;
    while(splitstring){
        //puts("Before copy");
        content_array[i] = malloc(strlen(splitstring) + 1);
        strncpy(content_array[i], splitstring, sizeof(content_array));
        //puts("after copy");
        i++;
        printf("thingat ss: %s\n", splitstring);
        splitstring = strtok(NULL, " \n");
        //("%s", contents2);
        //("2 contents: %s\n", contents2);
   }
    //printf("thingat ss: %s\n", content_array[0]);
    
    
    puts("FREE");
    //Free up memory.
    free(contents);
    free(contents2);
    while(i){
        free(content_array[i]);
        i--;
    }
    free(content_array);
}
