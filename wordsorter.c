#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compare(const void *a, const void *b)
{
    return(*(int*)a - *(int*)b);
}

int str_cmp(const void *a, const void *b)
{
    const char **ap = (const char**)a;
    const char **bp = (const char**)b;
    return(strcmp(*ap, *bp));
}


    /*Get size of file*/
int file_size(FILE *words)
{

    long start = ftell(words);
    fseek(words, 0, SEEK_END);
    long end = ftell(words);
    int filesize = (end - start) + 1;
    printf("\nfilesize: %i\n", filesize);
    fseek(words, 0, SEEK_SET);
    return(filesize);
}

    //Read in file.
char * read_file(int filesize, FILE *words)
{
    char *contents = malloc(filesize);
    //char *contents2 = malloc(filesize);
    fread(contents, sizeof(char), filesize, words);
    //strcpy(contents2, contents);
    //printf("contents: %s\n", contents);
    fclose(words);
    return(contents);
}

    //WORDCOUNT
int word_count(char *contents)
{
    char *word = strtok(contents, " \n");
    int wordcount = 0;
    while(word != NULL){
        wordcount++;
        word = strtok(NULL, " \n");
    }
    printf("wordcount: %d\n", wordcount);
    return(wordcount);
}


int main(void)
{
    int testing[5] = {8, 9, 0, 5, 1};
    qsort(testing, 5, sizeof(int), compare);
    for(int i = 0; i < 5; i++){
        printf("%d, ", testing[i]);
    }
    
    FILE *words = fopen("sorttest", "r");
    int filesize = file_size(words);
    char *contents = read_file(filesize, words);
    char *contents2 = malloc(filesize);
    strcpy(contents2, contents);
    int wordcount = word_count(contents);
    free(contents);

    
    char **content_array;
    content_array = malloc(wordcount * (sizeof(char*)));

    puts(contents2);
    
    char *splitstring = strtok(contents2, " \n");
    int i = 0;
    while(splitstring){
        content_array[i] = malloc(strlen(splitstring) + 1);
        strncpy(content_array[i], splitstring, strlen(splitstring));
        printf("thingat ss: %s\n", content_array[i]);
        i++;
        splitstring = strtok(NULL, " \n");

    }
    //printf("~~thingat: %s\n", content_array[5]);
    
    qsort(content_array, wordcount, sizeof(char *), str_cmp);

    //Test Print of content_array.
    int i2 = 0;
    for(;i2 < wordcount; ++i2){
        puts(content_array[i2]);
    }


    puts("\nFREE");
    //Free up memory.
    free(contents2);
    free(content_array);
}
