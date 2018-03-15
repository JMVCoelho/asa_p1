#include <stdio.h>
#include <stdlib.h>

int compFirstEl(const void * a, const void * b){
	int *u = (int*) a;
	int *v = (int*) b;
	return (u[0] - v[0]);
}

int compSecondEl(const void * a, const void * b){
	int *u = (int*) a;
	int *v = (int*) b;
	return (u[1] - v[1]);
}


int main(){
	int v;
	int e;
	int i, j;

	scanf("%d", &v);
	scanf("%d", &e);

	int list[e][2];
	int listN[v];

	for(i=0; i<v; i++){
		listN[i]=0;
	}

	for(i=0; i<e; i++){
		scanf("%d %d", &list[i][0], &list[i][1]);
		listN[list[i][0]-1]++;
	}

	printf("\n");
	printf("lista:\n");
	for(i=0; i<e; i++){
		printf("%d %d\n", list[i][0] , list[i][1]);
	}

	printf("vizinhos:\n");
	for(i=0; i<v; i++){
		printf("%d ", listN[i]);
	}

	qsort(list, e, sizeof(int)*2, compFirstEl);

	printf("lista apos 1o sort:\n");
	for(i=0; i<e; i++){
		printf("%d %d\n", list[i][0], list[i][1]);
	}

	for (i=0, j=0; i<v; i++) {
		int len = listN[i];
		if (len)
			qsort(list+j, len, sizeof(int)*2, compSecondEl);
		j = j + len;
	}

	printf("lista apos 2o sort:\n");
	for(i=0; i<e; i++){
		printf("%d %d\n", list[i][0], list[i][1]);
	}

}
