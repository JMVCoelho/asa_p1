#include <stdio.h>
#include <stdlib.h>

typedef struct node{
	int vertex;
	int scc_id;
	int isInStack;
	struct node *next;
} Node;

typedef struct stack{
	int top;
	int *vertices;
} Stack;

Stack vertices_stack;
int numberOfSCCs = 0;

void InsertArc(Node *list, int or, int dst);
void freeList(Node *list, int numberOfVertices);

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

void stack_push(int v, int numberOfVertices){
	vertices_stack.top++;
	if(vertices_stack.top < numberOfVertices){
		vertices_stack.vertices[vertices_stack.top] = v;
	}
	else{
		printf("Stack cheia, erro algures. APAGA-ME");
	}
}

void TarjanVisit(int vertex, int numberOfVertices, int *d, int *low, int *visited, Node *list, int *minSCC){
	Node *tmp;
	d[vertex] = *visited;
	low[vertex] = *visited;

	*visited=*visited+1;
	stack_push(vertex, numberOfVertices);
	list[vertex].isInStack = 1;

	tmp = list[vertex].next;
	while(tmp != NULL){
		if(d[(tmp->vertex)-1]==-1 || list[(tmp->vertex)-1].isInStack){
			if(d[(tmp->vertex)-1]==-1){
				TarjanVisit((tmp->vertex)-1, numberOfVertices, d, low, visited, list, minSCC);
			}
			if(low[(tmp->vertex)-1]<low[vertex]){
				low[vertex]=low[(tmp->vertex)-1];
			}
		}
		tmp=tmp->next;
	}
	if(d[vertex]==low[vertex]){
		int v;
		int min = 0;

		while( (v = vertices_stack.vertices[vertices_stack.top--]) != -1){
			list[v].isInStack = 0;
			if (min == 0 || v < min) {
				min = v;
			}
			list[v].scc_id = numberOfSCCs;

			if(vertex == v){
				break;
			}
		}
		minSCC[numberOfSCCs] = min+1;
		numberOfSCCs++;
	}
}

int SCCTarjan(Node *list, int numberOfVertices, int numberOfEdges, int *minSCC){
	int i;
	int visited = 0;
	int *ptr_visited = &visited;

	int *d = (int*) malloc(sizeof(int)*numberOfVertices);
	int *low = (int*) malloc(sizeof(int)*numberOfVertices);

	vertices_stack.vertices = (int*)malloc(sizeof(int)*numberOfVertices);
	vertices_stack.top = -1;

	for(i=0; i<numberOfVertices; i++){
		d[i] = -1;
		minSCC[i] = 0;
	}

	for(i=0; i<numberOfVertices; i++){
		if (d[i] == -1){
			TarjanVisit(i, numberOfVertices, d, low, ptr_visited, list, minSCC);
		}
	}

	free(d);
	free(low);
	return 0;
}

int main(){
  int or,dst;
  int numberOfVertices;
  int numberOfEdges;
  int i, j;

  if (scanf("%d",&numberOfVertices) != 1) {
		printf("Erro ao ler o numero de vertices");
		return 1;
	}

	if (scanf("%d", &numberOfEdges) != 1) {
		printf("Erro ao ler o numero de arestas");
		return 1;
	}

	Node *list = (Node*)malloc(sizeof(Node)*numberOfVertices);
	int orderedEdgeList[numberOfEdges][2];
	int neighborList[numberOfVertices];
	int *minSCC = (int*) malloc(sizeof(int)*numberOfVertices);

  for(i=0; i<numberOfVertices; i++){
    list[i].vertex = i+1;
    list[i].next = NULL;
		list[i].isInStack = 0;
		neighborList[i]=0;
  }

  for(i=0; i<numberOfEdges; i++){

		if (scanf("%d %d", &or, &dst) != 2) {
			printf("Erro ao ler o numero de arestas");
			return 1;
		}

		orderedEdgeList[i][0] = or;
		orderedEdgeList[i][1] = dst;
		neighborList[or-1]++;
    InsertArc(list,or,dst);
  }
	qsort(orderedEdgeList, numberOfEdges, sizeof(int)*2, compFirstEl);
	int len;
	for (i=0, j=0; i<numberOfVertices; i++) {
		len = neighborList[i];
		if (len)
			qsort(orderedEdgeList+j, len, sizeof(int)*2, compSecondEl);
		j = j + len;
	}

	SCCTarjan(list, numberOfVertices, numberOfEdges, minSCC);

	printf("%d\n", numberOfSCCs);


	int prevOr = -1;
	int prevDst = -1;
	int bridges = 0;
	int printOr, printDst;
	for (i = 0; i<numberOfEdges; i++) {
		if ((list[orderedEdgeList[i][0]-1].scc_id != list[orderedEdgeList[i][1]-1].scc_id)) {
			printOr = minSCC[list[orderedEdgeList[i][0]-1].scc_id];
			printDst = minSCC[list[orderedEdgeList[i][1]-1].scc_id];
			if (printOr != prevOr || printDst != prevDst) {
				bridges++;
			}
			prevOr = printOr;
			prevDst = printDst;
		}
	}
	printf("%d\n", bridges);

	prevOr = -1;
	prevDst = -1;
	for (i = 0; i<numberOfEdges; i++) {
		if ((list[orderedEdgeList[i][0]-1].scc_id != list[orderedEdgeList[i][1]-1].scc_id)) {
			printOr = minSCC[list[orderedEdgeList[i][0]-1].scc_id];
			printDst = minSCC[list[orderedEdgeList[i][1]-1].scc_id];
			if (printOr != prevOr || printDst != prevDst) {
				printf("%d %d\n", printOr, printDst);
			}
			prevOr = printOr;
			prevDst = printDst;
		}
	}

	free(minSCC);
	freeList(list, numberOfVertices);
	return 0;
}

void InsertArc(Node *list, int or, int dst){
  Node *aux = (Node*) malloc(sizeof(Node));
  aux->vertex = dst;

  if(list[or-1].next == NULL)
		aux->next = NULL;
  else	{
    aux->next = list[or-1].next;
  }
	list[or-1].next = aux;
}

void freeList(Node *list, int numberOfVertices) {
	int i;
	for (i=0; i<numberOfVertices; i++) {
		Node *head = &list[i];
		while (head->next != NULL) {
			Node *tmp = head->next;
			head->next = tmp->next;
			free(tmp);
		}
	}
	free(list);
}