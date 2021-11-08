#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void readFromFile(int argc, char*argv[])
{
    
    FILE *fp;

    size_t bufsize = 0;
    ssize_t characters;
    char *line = NULL;
    int i;
 

    for (i = 2; i < argc; i++)
    {
        fp = fopen(argv[i], "rb");
        if (fp == NULL)
        {
            printf("wgrep: cannot open file\n");
            exit(1);
        }

        while ((characters = getline(&line, &bufsize, fp)) != -1)
        { //for wgrep, change fgets to getline

            if ((strstr(line, argv[1]) != NULL))
                printf("%s", line); //get line, compare with strstr, print if there is a match for wgrep.
            //memset(line,0,sizeof(line));
        }

        fclose(fp);
    }
};

void readFromStdin(char searchTerm[])
{
    size_t bufsize = 0;
    ssize_t characters;
    char *line = NULL;

    while ((characters = getline(&line, &bufsize, stdin)) != -1)
    { //for wgrep, change fgets to getline

        if ((strstr(line, searchTerm) != NULL))
            printf("%s", line); //get line, compare with strstr, print if there is a match for wgrep.
        //memset(line,0,sizeof(line));
    }
}

int main(int argc, char *argv[])
{

    //checking if right # of arguments were passed

    if (argc >= 3){
        readFromFile(argc,argv);
    }
    else if (argc == 2){
        readFromStdin(argv[1]);
    }
    else
    {
        printf("wgrep: searchterm [file ...]\n");
        exit(1);
    }


    exit(0);
}
