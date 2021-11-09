#include <stdio.h>
#include <stdlib.h>

void unzip(char *argv)
{
    char curr = 0;
    int count = 0;
    FILE *fp = fopen(argv, "rb");
    if (fp == NULL)
    {
        printf("%s not found.\n", argv);

        exit(1);
    }
    while (1)
    {
        int bytes = fread(&count, 1, 4, fp); //reading in compressed file from wzip
        if (bytes != 4)
        {
            break;
        }
        fread(&curr, 1, 1, fp);
        {
            int i = 0;
            for (i = 0; i < count; i++)
            {
                fwrite(&curr, 1, 1, stdout);//writing to stdoutput
            }
        }
    }
    fclose(fp);
}
int main(int argc, char **argv)
{
    int i = 0;
    if (argc < 2) //checking for correct arguments
    {
        printf("wunzip: file1 [file2 ...]\n");
        exit(1);
    }
    for (i = 1; i < argc; i++)
    {
        unzip(argv[i]);
    }
    return 0;
}