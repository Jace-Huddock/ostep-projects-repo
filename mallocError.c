#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv){

   const int NUM_APPLES = 5; 
   int *apples = malloc(NUM_APPLES * sizeof(*apples));

   for (int i = 0; i < NUM_APPLES; i++) {
       apples[i] += i; 
       printf("%d: %d\n", i, apples[i]);
   }

   free(apples); //normally you would do this, but not this time.
    return 0;
}