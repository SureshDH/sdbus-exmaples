#include <stdio.h>
#include <stdlib.h>

int main() {
	int *array[6];
	int i, j;
	*array = (int *)malloc(10*sizeof(int));
	for(i=0; i<10; i++) {
		array[i] = (int *)malloc(6*sizeof(int));
	}
	for(i=0; i<10; i++) {
		for(j=0; j<6; j++){
			array[i][j] = 500+i+j;
		}
	}

	for(i=0; i<10; i++) {
		for(j=0; j<6; j++){
			printf("%5d", array[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	return 0;
}
