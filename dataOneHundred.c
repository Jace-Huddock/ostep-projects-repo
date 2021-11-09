#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv){

int *data = malloc(100 * sizeof(*data)); 

data[100] = 0; 


return 0;
}