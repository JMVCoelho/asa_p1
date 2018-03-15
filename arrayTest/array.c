#include <stdio.h>
#include <stdlib.h>


int compfunc(const void * a, const void * b){
	printf("%d", *(int*)a);
	printf("\n");
	return ( *(int*)a - *(int*)b );
}


int main(){
	
	int v;
	int e;
	int i;
	
	
	scanf("%d" , &v);
	scanf("%d" , &e);
	
	int list[e][2];
	int listN[v];
	
	for(i=0; i<v; i++){
		listN[i]=0;
	}
	
	for(i=0;i<e;i++){
		scanf("%d %d", &list[i][0] , &list[i][1]);
		listN[list[i][0]-1]++;
	}
	
	printf("\n");
	printf("lista:\n");
	for(i=0; i<e; i++){
		
		printf("%d %d\n", list[i][0] , list[i][1]);	
	}
	
	printf("vizinhos:\n");
	for(i=0; i<v; i++){
		printf("%d " , listN[i]);
	}
	
	qsort(list,e,sizeof(int)*2,compfunc);
	
	printf("lista sorted:\n");
	for(i=0; i<e; i++){
		
		printf("%d %d\n", list[i][0] , list[i][1]);	
	}
	
		
		
}	

