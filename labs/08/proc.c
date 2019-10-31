#include <stdio.h>
#include <stdlib.h>

//Overleaf pdf: https://www.overleaf.com/read/bbgmpwcqpbfn


int proc(int a[]) {
	int sum = 0, i;
	for (i=0; i < 1000000; i++)
		sum += a[i];
	return sum;
}

int main(){
    int sum;
    int* arr =  (int*)malloc(sizeof(int)*1000000);
    sum = proc(arr);
    printf("Sum: %d\n",sum);
    return 0;
}