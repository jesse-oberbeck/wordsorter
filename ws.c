#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

void print_help(void)
{
    puts("Usage: wc <file(s)> <options>");
    puts("\tIf no options are supplied, will sort from stdin.");
    puts("\t-c <n> print only n results of sorted content.");
    puts("\t-r print in reverse order.");
    puts("\t-n numeric sort.");
    puts("\t-l sort words by length.");
    puts("\t-s sort by Scrabble score.");
    puts("\t-a sort to lexicographical order.");
    puts("\t-u only print unique words.");
    puts("\t-h print this help message.");
}
/*
void take_stdin()
{
    char **content_array = {'\0'};
    content_array = malloc(sizeof(char*) + 1);    

    int i = 0;
    while(strcmp(user_in, "end") != 0){
//USE FGETS TO TAKE STD IN, DO UNTIL "end" is recieved.
        content_array[i] = calloc(strlen(splitstring) + 1, 1);
        strncpy(content_array[i], splitstring, strlen(splitstring));

        i++;
        splitstring = strtok(NULL, " \n\t");

    }
}
*/
//TODO: Scrabble Compare

/*Numeric Compare.*/
int num_cmp(const void *a, const void *b)
{
    char **ap = (char**)a;
    char **bp = (char**)b;
    char *a_buf = (char*)malloc(strlen(*ap) + 1);
    char *b_buf = (char*)malloc(strlen(*bp) + 1);
    int a_num = strtol(*ap, &a_buf, 10);
    int b_num = strtol(*bp, &b_buf, 10);

    if(a_num && (!b_num)){
        return(1);
    }else if(b_num && (!a_num)){
        return(-1);
    }else if(!b_num && !a_num){
        return(strcmp(*ap, *bp));
    }else{
        return(a_num - b_num);
    }
}

/*Default Compare.*/
int str_cmp(const void *a, const void *b)
{
    char **ap = (char**)a;
    char **bp = (char**)b;
    return(strcmp(*ap, *bp));
}

/*Length Compare.*/
int len_cmp(const void *a, const void *b)
{
    char **ap = (char**)a;
    char **bp = (char**)b;
    return(strlen(*ap) - strlen(*bp));
}

/*Reverse Default Compare.*/
int rev_str_cmp(const void *a, const void *b)
{
    char **ap = (char**)a;
    char **bp = (char**)b;
    return(strcmp(*bp, *ap));
}

/*Reverse Length Compare.*/
int rev_len_cmp(const void *a, const void *b)
{
    char **ap = (char**)a;
    char **bp = (char**)b;
    return(strlen(*bp) - strlen(*ap));
}

/*Reverse Numeric Compare.*/
int rev_num_cmp(const void *a, const void *b)
{
    char **ap = (char**)a;
    char **bp = (char**)b;
    char *a_buf = (char*)malloc(strlen(*ap) + 1);
    char *b_buf = (char*)malloc(strlen(*bp) + 1);
    int a_num = strtol(*ap, &a_buf, 10);
    int b_num = strtol(*bp, &b_buf, 10);

    if(a_num && (!b_num)){
        return(-1);
    }else if(b_num && (!a_num)){
        return(1);
    }else if(!b_num && !a_num){
        return(strcmp(*bp, *ap));
    }else{
        return(b_num - a_num);
    }
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
    printf("wordcount: %d\n\n", wordcount);
    return(wordcount);
}


int main(int argc, char *argv[])
{
    FILE *words = fopen("sorttest", "r");
    int filesize = file_size(words);
    char *contents = read_file(filesize, words);
    char *contents2 = malloc(filesize);
    strcpy(contents2, contents);
    int wordcount = word_count(contents);
    free(contents);
    char **content_array = {'\0'};
    content_array = malloc(wordcount * (sizeof(char*) + 1));    
    char *splitstring = strtok(contents2, " \n\t");
    int i = 0;
    while(splitstring){

        content_array[i] = calloc(strlen(splitstring) + 1, 1);
        strncpy(content_array[i], splitstring, strlen(splitstring));

        i++;
        splitstring = strtok(NULL, " \n\t");

    }
    int lines_to_print = wordcount;
    int no_flags_flag = 0;
    int r_flag = 0;
    int optflag = 0;
    for(int i = argc; ((optflag = getopt(argc, argv, "c:rnlsauh")) != (-1)); --i){
        no_flags_flag = 1;
        switch(optflag){

            case 'h':
                print_help();
                exit(0);

            case 'r':
                r_flag += 1;
                break;

            case 'c':
                lines_to_print = strtol(optarg, NULL, 10);
                break;

            case 'n':
                qsort(content_array, wordcount, sizeof(char *), num_cmp);
                break;

            case 'l':
                qsort(content_array, wordcount, sizeof(char *), len_cmp);
                break;

            case 's':
                puts("s");
                break;

            case 'a':
                qsort(content_array, wordcount, sizeof(char *), str_cmp);
                break;

            case 'u':
                puts("u");
                break;
        }
    }
    
    if(no_flags_flag == 0){
            puts("Enter words to sort. \"end\" to quit.");
            qsort(stdin, wordcount, sizeof(char *), str_cmp);
            return(0);
    }

    //qsort(content_array, wordcount, sizeof(char *), rev_num_cmp);
    //puts("after sort");
    //Test Print of content_array.
    //printf("r_flags: %d\n\n", r_flag);
    if((!r_flag) || (r_flag % 2 == 0)){
        int i2 = 0;
        for(;i2 < lines_to_print; ++i2){
            puts(content_array[i2]);
        }
    }else{
        int i2 = lines_to_print - 1;
        for(;i2 >= 0; --i2){
            puts(content_array[i2]);
        }
    }


    puts("\nFREE");
    //Free up memory.
    free(contents2);
    for(int i = 0; i < wordcount; ++i){
        free(content_array[i]);
    }
    free(content_array);
}
