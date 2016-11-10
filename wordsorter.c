#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compare(const void *a, const void *b)
{
    return(*(int*)a - *(int*)b);
}

/*
int ascii_cmp(const void *a, const void *b)
{
    //puts("called");
    const char **ap = (const char**)a;
    const char **bp = (const char**)b;
    
    int a_total = 0;
    int b_total = 0;
    //printf("strlen a: %d" , (int)strlen(*ap));
    
    for(size_t i = 0; i < strlen(*ap); i++){
        a_total += (int)(*ap[i]);
    }
    
    for(size_t i = 0; i < strlen(*bp); i++){
        b_total += (int)(*bp[i]);
    }
    printf("ascii_cmp return: %d\n", a_total - b_total);
    return(a_total - b_total);
    
}
*/

/*Default Compare.*/
int str_cmp(const void *a, const void *b)
{
    const char **ap = (const char**)a;
    const char **bp = (const char**)b;
    return(strcmp(*ap, *bp));
}

/*Length Compare.*/
int len_cmp(const void *a, const void *b)
{
    const char **ap = (const char**)a;
    const char **bp = (const char**)b;
    return(strlen(*ap) - strlen(*bp));
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

/*Read in file.*/
char * read_file(int filesize, FILE *words)
{
    char *contents = malloc(filesize);
    fread(contents, sizeof(char), filesize, words);
    fclose(words);
    return(contents);
}

/*Word count. Tokenizes file based on Whitespace
or newline, increasing a counter on each word.
returns the value held in the counter.*/
int word_count(char *contents)
{
    char *word = strtok(contents, " ;,.\n\t");
    int wordcount = 0;
    while(word != NULL){
        wordcount++;
        word = strtok(NULL, " ;,.\n\t");
    }
    printf("wordcount: %d\n", wordcount);
    return(wordcount);
}


int main(void)
{
    //int testing[5] = {8, 9, 0, 5, 1};
    //qsort(testing, 5, sizeof(int), compare);
    //for(int i = 0; i < 5; i++){
    //    printf("%d, ", testing[i]);
    //}
    
    FILE *words = fopen("sorttest", "r");
    int filesize = file_size(words);
    char *contents = read_file(filesize, words);
    char *contents2 = malloc(filesize);
    strcpy(contents2, contents);
    int wordcount = word_count(contents);
    free(contents);

    
    char **content_array = {'\0'};
    content_array = malloc(wordcount * (sizeof(char*) + 1));

    //puts(contents2);
    
    char *splitstring = strtok(contents2, " \n\t");
    int i = 0;
    while(splitstring){
	//printf("sizecheck: %li, splitstring: %s\n", strlen(splitstring) + 1, splitstring);
        content_array[i] = calloc(strlen(splitstring) + 1, 1);
        strncpy(content_array[i], splitstring, strlen(splitstring));
        //printf("thingat ss: %s\n", content_array[i]);
        i++;
        splitstring = strtok(NULL, " \n\t");

    }
    
    qsort(content_array, wordcount, sizeof(char *), len_cmp);
    puts("after sort");
    //Test Print of content_array.
    int i2 = 0;
    for(;i2 < wordcount; ++i2){
        puts(content_array[i2]);
    }


    puts("\nFREE");
    //Free up memory.
    free(contents2);
    for(int i = 0; i < wordcount; ++i){
        free(content_array[i]);
    }
    free(content_array);
}
