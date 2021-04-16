#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const int word_list_MAX = 100;
const int string_MAX = 10000;
const int stopw_list_MAX = 300;

void read_file(FILE *file_input, char *string)
{
    char string_line[1000];
    while (!feof(file_input))
    {
        memset(string_line, 0, 1000);
        fgets(string_line, 1000, file_input);
        if (string_line[0] != '\0')
        {
            strcat(string, string_line);
        }
    }
}

typedef struct {
    int hang;
    int cot;
} toa_do;

// Working with word_list
typedef struct
{
    char word_name[100];
    toa_do coordinates[30];
    int nums;
} word;

void word_reset(word *word_input)
{
    memset(word_input->word_name, 0, 100);
    word_input->nums = 0;
    for (int j = 0; j < 30; j++)
    {
        word_input->coordinates[j].cot = 0;
        word_input->coordinates[j].hang = 0;
    }
}

void word_list_reset(word *word_list)
{
    for (int i = 0; i < word_list_MAX; i++)
    {
        word_reset(&word_list[i]);
    }
}
void word_list_arrange(word word_list[])
{
    for (int i = 0; i < word_list_MAX - 1; i++)
    {
        for (int j = i + 1; j < word_list_MAX; j++)
        {
            char str1[30];
            char str2[30];
            strcpy(str1, word_list[i].word_name);
            strcpy(str2, word_list[j].word_name);
            if(word_list[i].word_name[0] >= 'A' && word_list[i].word_name[0] <= 'Z') {
                str1[0] = str1[0] + 32;
            }

            if(word_list[j].word_name[0] >= 'A' && word_list[j].word_name[0] <= 'Z') {
                str2[0] = str2[0] + 32;
            }
            
            if (strcmp(str1, str2) > 0)
            {
                word TG = word_list[i];
                word_list[i] = word_list[j];
                word_list[j] = TG;
            }
        }
    }
}

int word_list_add(word word_list[], word word_input)
{
    for (int i = 0; i < word_list_MAX; i++)
    {
        if (word_list[i].word_name[0] == '\0')
        {
            word_list[i] = word_input;
            word_list_arrange(word_list);
            return i;
        }
    }
    return 0;
}

int word_list_update(word word_list[], char string_input[], int line, int column)
{
    for (int i = 0; i < word_list_MAX; i++)
    {
        if (strcmp(word_list[i].word_name, string_input) == 0)
        {
            word_list[i].nums++;
            int j = 0;
            while (word_list[i].coordinates[j].cot != 0)
            {
                j++;
            }

            word_list[i].coordinates[j].hang = line;
            word_list[i].coordinates[j].cot = column;
            return 1;
        }
    }

    word word_input;
    word_reset(&word_input);
    strcpy(word_input.word_name, string_input);
    word_input.nums++;
    int j = 0;
    while (word_input.coordinates[j].cot != 0)
    {
        j++;
    }
    word_input.coordinates[j].hang = line;
    word_input.coordinates[j].cot = column;
    word_list_add(word_list, word_input);

    return 0;
}

void word_list_show(word word_list[])
{
    for (int i = 0; i < word_list_MAX; i++)
    {
        if (word_list[i].word_name[0] != '\0')
        {
            printf("%s %d ", word_list[i].word_name, word_list[i].nums);
            for (int j = 0; j < 30; j++)
            {
                if (word_list[i].coordinates[j].cot != 0)
                {

                    printf("(%d %d) ", word_list[i].coordinates[j].hang, word_list[i].coordinates[j].cot);
                }
            }
            printf("\n");
        }
    }
}

void word_list_output_file(FILE *file_out, word word_list[])
{
    for (int i = 0; i < word_list_MAX; i++)
    {
        if (word_list[i].word_name[0] != '\0')
        {
            fprintf(file_out, "%s %d ", word_list[i].word_name, word_list[i].nums);
            for (int j = 0; j < 30; j++)
            {
                if (word_list[i].coordinates[j].cot != 0)
                {

                    fprintf(file_out,"(%d %d) ", word_list[i].coordinates[j].hang, word_list[i].coordinates[j].cot);
                }
            }
            fprintf(file_out, "\n");
        }
    }
}

int check_sign(char input_char)
{
    if ((input_char >= 'A' && input_char <= 'Z') || (input_char >= 'a' && input_char <= 'z'))
    {
        return 0;
    }
    return 1;
}

int check_uppercase()
{
}

//Check uppercase
int check_proper_noun(char *string_input, int start)
{
    if (start < 2)
        return 0;
    char check_point = string_input[start - 2];
    if (!check_sign(check_point) || check_point == ',')
    {
        return 1;
    }
    return 0;
}

void read_file_stopw(FILE *file_stopw, char stop_word_list[stopw_list_MAX][word_list_MAX])
{
    char stop_word[word_list_MAX];
    int index = 0;

    while (!feof(file_stopw))
    {
        fscanf(file_stopw, "%s\n", stop_word);
        strcpy(stop_word_list[index], stop_word);
        index++;
        memset(stop_word, 0, word_list_MAX);
    }
}

int check_stopw(char stop_word_list[stopw_list_MAX][word_list_MAX], char *string)
{
    for (int i = 0; i < stopw_list_MAX; i++)
    {
        if (strcmp(stop_word_list[i], string) == 0)
        {
            return 1;
        }
    }
    return 0;
}

void reset_stop_word_list(char stop_word_list[stopw_list_MAX][word_list_MAX])
{
    for (int i = 0; i < stopw_list_MAX; i++)
    {
        memset(stop_word_list[i], 0, word_list_MAX);
    }
}

void string_process(char string_input[], word word_list[], char stop_word_list[stopw_list_MAX][word_list_MAX])
{
    char string_word[100];
    int line = 1;
    int column = 1;
    int start = 0;
    int end = 0;
    while (string_input[end] != '\0')
    {
        if (string_input[end] == '\n' || string_input[end] == '\r')
        {
            line++;
            column = 1;
        }


        if ((check_sign(string_input[end]) && string_input[end] != '.' && string_input[end] != '-') || (string_input[end] == '.' && check_sign(string_input[end + 1])))
        // if(check_sign(string_input[end]))
        {
            if (start != end)
            {

                strncpy(string_word, string_input + start, end - start);
                //check uppercase
                int check = 0;

                if (check_stopw(stop_word_list, string_word))
                {
                    check = 1;
                    column++;
                }

                if (string_word[0] >= 'A' && string_word[0] <= 'Z' && check != 1)
                {
                    
                    
                    if (!check_proper_noun(string_input, start))
                    {
                        string_word[0] = string_word[0] + 32;
                        if (check_stopw(stop_word_list, string_word))
                    {
                        check = 1;
                    }
                        
                    }
                }
                if (check == 0)
                {
                    word_list_update(word_list, string_word, line, column);
                    column++;
                }
                memset(string_word, 0, 100);
            }
            start = end + 1;
            
        }
        end++;
    }
}

void main()
{
    FILE *file_input = fopen("vanban.txt", "r");
    if (file_input == NULL)
    {
        printf("Not found file\n");
        exit(0);
    }

    FILE *file_stopw = fopen("stopw.txt", "r");
    if (file_stopw == NULL)
    {
        printf("Not found file stopw\n");
        exit(0);
    }

    FILE *file_out = fopen("file_output.txt", "w");
    if (file_stopw == NULL)
    {
        printf("Not found file output\n");
        exit(0);
    }

    char string[string_MAX];
    memset(string, 0, string_MAX);
    char stop_word_list[stopw_list_MAX][word_list_MAX];

    reset_stop_word_list(stop_word_list);
    read_file_stopw(file_stopw, stop_word_list);

    read_file(file_input, string);

    word word_list[word_list_MAX];
    word_list_reset(word_list);

    string_process(string, word_list, stop_word_list);
    word_list_output_file(file_out, word_list);
    fclose(file_input);
    fclose(file_out);
    fclose(file_stopw);
}
