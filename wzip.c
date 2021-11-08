#include <stdio.h>
#include <stdlib.h>

void zip(char *argv)
{
    FILE *fp = fopen(argv, "rb");
    char prev = 0, curr = 0;
    int count = 0;

    if (fp == NULL)
    {
        printf("%s not found. \n", argv);
        exit(1);
    }
    while (1)
    {
        curr = fgetc(fp); //reading from file fp
        if (curr == EOF)
        {

            if (count > 0)
            {
                fwrite(&count, 1, 4, stdout);
                fwrite(&prev, 1, 1, stdout);
            }
            break;
        }
        if (curr != prev)
        {
            /*                       * We got a new character, so first write the count of previous                          * character.                    */
            if (count > 0)
            {
                fwrite(&count, 1, 4, stdout);
                fwrite(&prev, 1, 1, stdout);
            }
            count = 0;
        }
        count++;
        prev = curr;
    }
    fclose(fp);
}
int main(int argc, char **argv)
{
    int i = 0;
    if (argc < 2)
    {
        printf("wzip: file1 [file2 ...]\n");
        exit(1);
    }
    for (i = 1; i < argc; i++) //Cycles through each provided file and zips contents
    {
        zip(argv[i]);
    }
    return 0;
}
