#include <stdio.h>

int * hello(){
    static int myArray[] = {1,2,3,4,5};
    return myArray;
}



int main(){

	printf("Hello World!\n");
    int my[5];
    my[0] = hello()[0];
    my[1] = hello()[1];
    my[2] = hello()[2];
    my[3] = hello()[3];
    my[4] = hello()[4];
    printf("%d",my[0]);
}

