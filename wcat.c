#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SZ 200
#define ARR_SZ 50

int main(int argc, char *argv[]){

    FILE *fp;
    char buffer[BUFFER_SZ];
    int i;

    for(i = 1; i < argc; i++){

        if((fp = fopen(argv[i], "rb")) == NULL){
            printf("wcat: cannot open file\n");
            exit(1); 
        }

        while (fgets(buffer, BUFFER_SZ + 1, fp) != NULL){ //for wgrep, change fgets to getline
             printf("%s", buffer); //get line, compare with strstr, print if there is a match for wgrep.
        }
        fclose(fp); 
        }
 
        exit(0);

}