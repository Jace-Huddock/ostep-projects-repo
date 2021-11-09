#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct myVector{

    int* data; //array
    int size;  //size of array
    int items; //array items

};
typedef struct myVector VECTOR; 


void printVector(VECTOR* pVector);
void addItems(VECTOR* pVector);
static void vector_Resize(VECTOR* pVector, int size); 



int main(int argc, char **argv){

    VECTOR* pV1 = (VECTOR*) malloc(sizeof(VECTOR)); //pointer to address of v1

    pV1->size = 1; //Initial size of vector
    pV1->items = 0; 

    pV1->data = (int*) malloc(pV1->size * sizeof(int)); 
    if(pV1->data == NULL){
        printf("No memory allocated"); 
        exit(1); 
    }

    addItems(pV1);
    printVector(pV1);


    free(pV1->data); 
    free(pV1); 

return 0;
}

void printVector(VECTOR* pVector){
        int i = 0; 
        while(i < pVector->items){
        printf("%d %d\n", pVector->data[i], pVector->items);
        i++;
        }
    return;
}
//Function that adds items to vector from user input and refactors array size
void addItems(VECTOR* pVector){
    int flag = 1; 
    int num = 0; 
    
    do{
    if(pVector->size == pVector->items)
        vector_Resize(pVector, pVector->size * 2); 

    printf("Please enter a number: "); 
    pVector->data[pVector->items++] = scanf("%d", &num); 

    printf("Enter 1 to continue, or 0 to exit: ");
    scanf("%d", &flag); 

    }while(flag == 1); 
    return;
}

static void vector_Resize(VECTOR* pVector, int size){
    #ifdef DEBUG_ON
    printf("vector_Resize: %d to %d\n", pVector->size, size); 
    #endif
    int* data = realloc(pVector->data, sizeof(int*) * size);
    if(data){
        pVector->data = data; 
        pVector->size = size; 
    }
}