// Homemade interpreter of "Verlfuck"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>

// ERROR CODES
#define ERROR_NO_FILE 1
#define ERROR_UNKNOWN_OPTION 2
#define ERROR_FILE_NOT_FOUND 3
#define ERROR_BUFFER_NULL 4
#define ERROR_FILE_READ 5
#define ERROR_NO_END_LOOP 6
#define ERROR_TOO_MANY_ARGS 7

// GLOBAL VARIABLES
static int debugMode = 0;
static char *contents;

int errorHandle(int errorNum)
{
    printf("ERROR: ");
    switch(errorNum)
    {
        case ERROR_NO_FILE: 
            printf("No file provided\n");
            printf("Usage: verler [file] [-option]\n");
            return 1;
        case ERROR_UNKNOWN_OPTION:
            printf("Unknown option provided, or option provided instead of file argument\n");
            printf("Usage: verler [file] [-option]\n");
            return 1;
        case ERROR_FILE_NOT_FOUND:
            printf("Specified file was not found\n");
            return 1;
        case ERROR_BUFFER_NULL:
            printf("Buffer returned NULL during memory allocation\n");
            return 1;
        case ERROR_FILE_READ:
            printf("Could not read file\n");
            return 1;
        case ERROR_NO_END_LOOP:
            printf("Loop did not end in file\n");
            printf("When placing a loop, you must contain it with []\n");
            return 1;
        case ERROR_TOO_MANY_ARGS:
            printf("Too many arguments provided.\n");
            printf("Usage: verler [file] [-option]\n");
        default:
            printf("Unknown error\n");
            return 1;
    }
}

void optionsHandle(char *option)
{
    if (strcmp(option, "-debug") == 0)
    {
        debugMode = 1;
        printf ("Debug enabled.\n");
        return;
    } else {
        errorHandle(ERROR_UNKNOWN_OPTION);
    }
}

void dPrint(const char *message, ...)
{
    va_list args;

    if (debugMode)
    {
        va_start(args, message);
        vprintf(message, args);
        va_end(args);
        return;
    }
    else {
        return;
    }
}

char *fileRead(const char *fileName)
{
    FILE *file;
    char *buffer;
    long fileSize;
    size_t result;

    file = fopen(fileName, "r");
    if (file == NULL)
    {
        errorHandle(ERROR_FILE_NOT_FOUND);
    }
    dPrint("File opened succesfully\n");

    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    rewind(file);

    buffer = (char *)malloc(sizeof(char) * fileSize);
    if (buffer == NULL)
    {
        errorHandle(ERROR_BUFFER_NULL);
    }
    dPrint("Memory for buffer allocated successfully\n");

    result = fread(buffer, 1, fileSize, file);
    if (result != fileSize)
    {
        errorHandle(ERROR_FILE_READ);
    }
    dPrint("File read successfully\n");

    return buffer;
}

uint8_t *arrayCreator()
{
    uint8_t *buffer = (uint8_t *)malloc(30000 * sizeof(char));
    if (buffer == NULL)
    {
        errorHandle(ERROR_BUFFER_NULL);
    }
    for (int i = 0; i <= 30000 ; i++)
    {
        buffer[i] = 0;
    }
    return buffer;

}

void interpreter(uint8_t *array, char *contents)
{
    int pointer = 0;
    int contentsSize = strlen(contents);
    char *input;
    int startLoc;
    int endLoc;

    dPrint("contentsSize: %d\n", contentsSize);

    for (int i = 0; i < contentsSize; i++)
    {
        dPrint("pointer: %d\n", pointer);
        dPrint("pointer value: %d\n", array[pointer]);
        dPrint("i: %d\n", i);
        dPrint("current byte: %c\n", contents[i]);
        switch (contents[i])
        {
            case '>':
                pointer++;
                break;
            case '<':
                pointer--;
                break;
            case '+':
                array[pointer]++;
                break;
            case '-':
                array[pointer]--;
                break;
            case '.':
                printf("%c", array[pointer]);
                dPrint("int: %d\n", (int) array[pointer]);
                break;
            case ',':
                scanf("%c", input);
                char x = input[0];
                array[pointer] = (uint8_t) x;
                break;
            case '[':
                if (array[pointer] == 0)
                {
                    int bracketNum = 0;
                    int j = i;
                    for (j; j < contentsSize; j++)
                    dPrint("bracketNum: %d\n", bracketNum);
                    {
                        if (contents[j] == '[')
                        {
                            bracketNum++;
                        } else if (contents[j] == ']') {
                            bracketNum--;
                        }
                        if (bracketNum == 0)
                        {
                            i = j;
                            break;
                        }
                    }
                } 
                break;
            case ']':
                if (array[pointer] != 0)
                {
                    int bracketNum = 0;
                    int l = i;
                    for (l; l >= 0; l--)
                    {
                        dPrint("bracketNum: %d\n", bracketNum);
                        if (contents[l] == '[')
                        {
                            bracketNum--;
                        } else if (contents[l] == ']') {
                            bracketNum++;
                        }
                        if (bracketNum == 0)
                        {
                            i = l;
                            break;
                        }
                    }
                }
                break;
            default:
                break;
        }
    }

    return;
}

int main (int argc, char *argv[])
{
    /*
        argv = 
            "verler",
            "file name",
            "option"
    */
    printf("Verler 1.0\n");
    if (argc < 2)
    {
        errorHandle(ERROR_NO_FILE);
    }
    if (argc >= 2)
    {
        if (argc == 3)
        {
            optionsHandle(argv[2]);   
        }
        if (argc > 3)
        {
            errorHandle(ERROR_TOO_MANY_ARGS);
        }
        contents = fileRead(argv[1]);
        dPrint("The contents of the file is:\n%s\n", contents);
        uint8_t *array = arrayCreator();
        interpreter(array, contents);
    }
    printf("\n");
    // END
    return 0;
}