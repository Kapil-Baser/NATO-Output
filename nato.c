#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define DELIM " \n,.!?:;\"'"

void read_phrase(const char *nato_term[]);
FILE *load_file(void);
void read_file(const char *nato_term[]);
void read_file_improved(const char *nato_term[]);
char isterm(char *term, const char *nato_term[]);
void nato_to_char(const char *nato_term[]);
int isterm_binary(char *tok, const char *nato_term[], int start_idx, int end_idx);

int main(void)
{
    const char *nato[] = {
    "Alfa", "Bravo", "Charlie", "Delta", "Echo", "Foxtrot",
    "Golf", "Hotel", "India", "Juliett", "Kilo", "Lima",
    "Mike", "November", "Oscar", "Papa", "Quebec", "Romeo",
    "Sierra", "Tango", "Uniform", "Victor", "Whiskey", "Xray", "Yankee", "Zulu"};

    while (1)
    {
        int i = 0;
        char buf[64];
        printf("\t\t---------------------------\n");
        printf("\t\tThe NATO translator program\n");
        printf("Options: 1. Translate a word or phrase to NATO phonetic\n \t 2. Convert NATO input to character output\n \t 3. Read a file and convert it to NATO phonetic\n \t 0. To quit\n");
        printf("Enter your choice: ");
        if (fgets(buf, 64, stdin) == NULL)
        {
            fprintf(stderr, "Error: Unable to read input stream\n");
            return 1;
        }
        // We read the first non white space character if any are present.
        while (buf[i] == ' ')
        {
            i++;
        }

        switch(buf[i])
        {
            case '1':
            {
                read_phrase(nato);
                break;
            }
            case '2':
            {
                nato_to_char(nato);
                break;
            }
            case '3':
            {
                read_file(nato);
                break;
            }
            case '0':
            {
                return 0;
            }
            default:
            {
                printf("Invalid option\n");
            }
        }
    }
    return 0;
}

void read_phrase(const char *nato_term[])
{
    char ch;
    char phrase[64];
    while (1)
    {
        printf("Enter a word or a phrase: ");
        if (fgets(phrase, 64, stdin) != NULL)
        {
            int i = 0;
            while (phrase[i])
            {
                if (phrase[0] == '\n')
                {
                    return;
                }
                ch = toupper(phrase[i]);
                if (isalpha(ch))
                {
                    printf("%s ", nato_term[ch - 'A']);
                }
                i++;
                if (i == 64)
                {
                    break;
                }
            }
            putchar('\n');
        }
    }
}

void read_file(const char *nato_term[])
{
    char file_name[64];
    int ch;
    int len = 0;
    printf("Enter file name: ");
    if (fgets(file_name, 64, stdin) != NULL)
    {
        // remove newline
        file_name[strlen(file_name) - 1] = '\0';

        FILE *fp = fopen(file_name, "r");
        if (fp == NULL)
        {
            printf("Error: Unable to open '%s', check if path is correct", file_name);
            exit(2);
        }
        while ((ch = fgetc(fp)) != EOF)
        {
            if (isalpha(ch))
            {
                printf("%s ", nato_term[toupper(ch) - 'A']);
            }
        }
        putchar('\n');
    }
}

FILE *load_file(void)
{
    char file_path[64];
    printf("Enter file name: ");
    if (!fgets(file_path, 64, stdin))
    {
        printf("Error: Input stream is empty.\n");
        exit(2);
    }
    else
    {
        file_path[strlen(file_path) - 1] = '\0';

        FILE *fp = fopen(file_path, "r");
        if (fp == NULL)
        {
            printf("Error: Unable to open '%s', check if path is correct", file_path);
            exit(2);
        }
        return fp;
    }
}

void read_file_improved(const char *nato_term[])
{
    int ch;
    int offset = 0;
    char word[64];

    FILE *fp = load_file();

    while ((ch = fgetc(fp)) != EOF)
    {
        if (isalpha(ch))
        {
            word[offset] = ch;
            offset++;

            if (offset >= 64)
            {
                fprintf(stderr, "Buffer overflow\n");
                exit(2);
            }
        }
        else
        {
            if (offset > 0)
            {
                word[offset] = '\0';
                printf("%c", *nato_term[isterm_binary(word, nato_term, 0, 25)]);
                offset = 0;
            }
        }
    }
    putchar('\n');
}

char isterm(char *term, const char *nato_term[])
{
    int x;
    const char *n;
    char *t;

    for (x = 0; x < 26; x++)
    {
        n = nato_term[x];
        t = term;

        while (*n != '\0')
        {
            if ((*n | 0x20) != (*t | 0x20))
            {
                break;
            }
            n++;
            t++;
        }
        if (*n == '\0')
        {
            return (*nato_term[x]);
        }
    }
    return ('\0');
}

void nato_to_char(const char *nato_term[])
{
    char buf[1024];
    char *token;
    printf("Enter nato phonetic separated by space to translate: ");
    if (!fgets(buf, 1024, stdin))
    {
        printf("Error: Input stream is empty\n");
        exit(2);
    }
    token = strtok(buf, DELIM);
    printf("Nato translation: ");
    while (token != NULL)
    {
        //printf("%c" ,isterm(token, nato_term));
        printf("%c", *nato_term[isterm_binary(token, nato_term, 0, 25)]);
        token = strtok(NULL, DELIM);
    }
    putchar('\n');
}

int isterm_binary(char *tok, const char *nato_term[], int start_idx, int end_idx)
{
    // base case
    if (start_idx <= end_idx)
    {
        int mid = start_idx + (end_idx - start_idx) / 2;

        if ((strcasecmp(nato_term[mid], tok)) == 0)
        {
            return mid;
        }
        else if (strcasecmp(nato_term[mid], tok) > 0)
        {
            return isterm_binary(tok, nato_term, 0, mid - 1);
        }
        else
        {
            return isterm_binary(tok, nato_term, mid + 1, end_idx);
        }
    }
    return -1;
}