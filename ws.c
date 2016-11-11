#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>
#include <unistd.h>

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

/*If no file is given as argv[1], takes words from
stdin until Ctrl-D is pressed.*/
char ** take_stdin()
{
    char **content_array = {'\0'};
    content_array = calloc(50 * (sizeof(char*) + 1),1);    
    
    int i = 0;
    char *user_in;
    char *temp = calloc(35,1);
    user_in = fgets(temp, 34, stdin);
    //char *splitstring = strtok(user_in, " \n\t");
    while(user_in != NULL){
        //puts("inside while");
        for(size_t i2 = 0;i2 < strlen(user_in); ++i2){
            if(user_in[i2] == '\n'){
                user_in[i2] = '\0';
            }
        }
        //printf("%s",user_in);
        content_array[i] = calloc(strlen(user_in) + 1, 1);
        strncpy(content_array[i], user_in, strlen(user_in));
        user_in = fgets(user_in, 34, stdin);
        i++;
        //splitstring = strtok(NULL, " \n\t");

    }
    free(temp);
    return(content_array);
}

/*Returns scrabble letter score.*/
int scrabble_convert(char letter)
{
    int score = 0;
    switch(letter){   
        case 'A': 
            score += 1; 
            break;
        case 'B': 
            score +=  3;
            break;
        case 'C': 
            score +=  3;
            break;
        case 'D': 
            score +=  2;
            break;
        case 'E':
            score +=  1;
            break;
        case 'F':
            score +=  4;
            break;
        case 'G':
            score +=  2;
            break;
        case 'H':
            score +=  4;
            break;
        case 'I':
            score +=  1;
            break;
        case 'J': 
            score +=  8;
            break;
        case 'K':
            score +=  5;
            break;
        case 'L':
            score +=  1;
            break;
        case 'M':
            score +=  3;
            break;
        case 'N':
            score +=  1;
            break;
        case 'O':
            score +=  1;
            break;
        case 'P':
            score +=  3;
            break;
        case 'Q':
            score +=  10;
            break;
        case 'R':
            score +=  1;
            break;
        case 'S':
            score +=  1;
            break;
        case 'T':
            score +=  1;
            break;
        case 'U':
            score +=  1;
            break;
        case 'V':
            score +=  4;
            break;
        case 'W':
            score +=  4;
            break;
        case 'X':
            score +=  8;
            break;
        case 'Y':
            score +=  4;
            break;
        case 'Z':
            score +=  10;
            break;
    }
    return(score);
}

/*Scrabble compare.*/
int scr_cmp(const void *a, const void *b)
{
    char **ap = (char**)a;
    char **bp = (char**)b;
    //puts(*ap);
    //puts(*bp);
    int total_score_a = 0;
    int total_score_b = 0;
    for(size_t i = 0; i < strlen(ap[0]); ++i){
        if(isalpha(bp[0][i])){
            char letter_a = toupper(ap[0][i]);
            //printf("letter a: %c to A: %c index: %li\n",ap[0][i], letter_a, i);
            total_score_a += scrabble_convert(letter_a);
        }
    }
    for(size_t i2 = 0; i2 < strlen(bp[0]); ++i2){
        if(isalpha(bp[0][i2])){
            char letter_b = toupper(bp[0][i2]);
            //printf("letter b: %c\n", letter_b);
            total_score_b += scrabble_convert(letter_b);
        }
    }
    //printf("totals %d - %d = %d\n", total_score_a, total_score_b, (total_score_a - total_score_b));
    return(total_score_a - total_score_b);
}


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

/*Get size of file*/
int file_size(FILE *words)
{

    long start = ftell(words);
    fseek(words, 0, SEEK_END);
    long end = ftell(words);
    int filesize = (end - start) + 1;
    //printf("\nfilesize: %i\n", filesize);
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
    //printf("file total wordcount: %d\n\n", wordcount);
    return(wordcount);
}

/*Reads the file passed, adjustst wordcount and 
returns an array of tokenized words from the file*/
char ** setup(int *wordcount, const char *filename)
{
    puts("setup");
    puts(filename);
    FILE *words = fopen(filename, "r");
    int filesize = file_size(words);
    char *contents = read_file(filesize, words);
    char *contents2 = malloc(filesize);
    strcpy(contents2, contents);
    *wordcount = word_count(contents);
    free(contents);
    char **content_array = {'\0'};
    content_array = malloc(*wordcount * (int)(sizeof(char*) + 1));    
    char *splitstring = strtok(contents2, " \n\t");
    int i = 0;
    while(splitstring){

        content_array[i] = calloc(strlen(splitstring) + 1, 1);
        strncpy(content_array[i], splitstring, strlen(splitstring));

        i++;
        splitstring = strtok(NULL, " \n\t");

    }
    free(contents2);
    return(content_array);
}

/*Prints items in array in order based on flag from args.*/
void print_sorted(int r_flag, int lines_to_print, char **content_array)
{
    puts("Sorted:");
    printf("lines: %d\n", lines_to_print);
    //Print forward.
    if((!r_flag) || (r_flag % 2 == 0)){
        int i2 = 0;
        for(;i2 < lines_to_print; ++i2){
            puts(content_array[i2]);
        }

    //Print reversed.
    }else{
        int i2 = lines_to_print - 1;
        for(;i2 >= 0; --i2){
            puts(content_array[i2]);
        }
    }
}


/*Filters array so a word only appears once.*/
char ** make_unique(char **content_array, int *wordcount)
{
    char **unique_array = malloc(*wordcount * (int)(sizeof(char*) + 1));
    int i = 0;
    int index = 0;
    int match_flag = 0;
    while(i < *wordcount){
        for(int i2 = 0; unique_array[i2] != '\0'; ++i2){
            //printf("i: %d i2: %d\n", i, i2);
            if(strcmp(content_array[i], unique_array[i2]) == 0){
                //puts("match found");
                match_flag = 1;
                continue;
            }
        }
        if(match_flag == 0){
            //puts("no matches");
            unique_array[index] = calloc(strlen(content_array[i]) + 1, 1);
            strncpy(unique_array[index], content_array[i], strlen(content_array[i]));
            //printf("added: %s\n", unique_array[index]);
            index++;
        }
        match_flag = 0;
        i++;
    }
    *wordcount = index;
    //print_sorted(0, index, unique_array);
    //exit(0); 
    return(unique_array);
}

/*Frees allocated space in array, then array itself.*/
void array_free(char **content_array, int *wordcount)
{
    for(int i = 0; i < *wordcount; ++i){
        free(content_array[i]);
    }
    free(content_array);
}

int main(int argc, char *argv[])
{
    char filename[32];
    char *path = getenv("HOME");
    snprintf(filename, sizeof(filename), argv[1], path);
    if((access(filename, F_OK) == -1) && (argc > 1)){
        printf("Number of args: %d\n", argc);
        perror("First argument must be a file");
        exit(1);
    }

    
    int wordcount = 0;
    int lines_to_print = wordcount;
    int no_flags_flag = 0;
    int r_flag = 0;
    int u_flag = 0;
    int optflag = 0;
    int sort_type = 0;
    printf("Number of args: %d\n", argc);
    for(int i = argc; ((optflag = getopt(argc, argv, "c:rnlsauh")) != -1); --i){ //flag check adopted from: https://linux.die.net/man/3/optarg
        printf(" SWITCHES! switch{%c}-%d" ,optflag, i);
        switch(optflag){

            case 'h':
                print_help();
                exit(0);

            case 'r':
                r_flag += 1;
                no_flags_flag = 1;
                break;

            case 'c':
                lines_to_print = strtol(optarg, NULL, 10);
                break;

            case 'n':
                sort_type = 2;
                no_flags_flag = 1;
                break;

            case 'l':
                sort_type = 1;
                no_flags_flag = 1;
                break;

            case 's':
                sort_type = 3;
                no_flags_flag = 1;
                break;

            case 'a':
                sort_type = 0;
                no_flags_flag = 1;
                break;

            case 'u':
                u_flag = 1;
                break;
        }
    }

    printf("nff: %d\n", no_flags_flag);
    //Handle run if no args passed.    
    if((no_flags_flag == 0) && (argc < 2)){

            puts("Enter words to sort. Ctrl-D to stop, and sort.");
            char **content_array;
            content_array = take_stdin();
            int i2 = 0;
            //Loop only exists to increment i2.
            for(;content_array[i2] != NULL; ++i2);
            lines_to_print = i2;
            wordcount = i2;
            qsort(content_array, i2, sizeof(char *), str_cmp);
            puts("\n\n");
            print_sorted(r_flag, lines_to_print, content_array);
            array_free(content_array, &wordcount);
            exit(0);
    }/*else if(no_flags_flag == 0){
        puts("no flags");
        for(int i = 0; i < argc; ++i){
            filename = malloc(strlen(argv[i]));
            strcpy(filename, optind);
	    puts(filename);
        }
    }*/
    puts("prepping to sort");
    if(filename == NULL){
        puts("NO FILENAME");
        exit(1);
        //memcpy(filename, "sorttest", strlen("sorttest") + 1);
        //puts(filename);
    }
    printf("filename: %s\n",filename);
    char **content_array = setup(&wordcount, filename);
    if(u_flag == 1){
        content_array = make_unique(content_array, &wordcount);
    }
    lines_to_print = wordcount;
    if(sort_type == 0){
        puts("default sort");
        qsort(content_array, wordcount, sizeof(char *), str_cmp);
    }
    else if(sort_type == 1){
        puts("len sort");
        qsort(content_array, wordcount, sizeof(char *), len_cmp);
    }
    else if(sort_type == 2){
        puts("num sort");
        qsort(content_array, wordcount, sizeof(char *), num_cmp);
    }
    else if(sort_type == 3){
        puts("scrabble sort");
        qsort(content_array, wordcount, sizeof(char *), scr_cmp);
    }

    //Call Print
    printf("lines: %d\n", lines_to_print);
    print_sorted(r_flag, lines_to_print, content_array);

    //Free up memory.
    array_free(content_array, &wordcount);
}
