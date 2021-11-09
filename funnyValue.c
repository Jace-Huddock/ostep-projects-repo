#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv){

int *data = malloc(100 * sizeof(*data)); 
data[1] = 10; 
data[2] = 15;
free(data[50]); 

printf("%d\n %d\n", data[1], data[2]); 


return 0;
}