#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* struct used for the linked lists */
typedef struct node{
	int vertex;
	int scc_id;
	int isInStack;
	struct node *next;
} Node;

/* struct used for the Tarjan algorithm stack */
typedef struct stack{
	int top;
	int *vertices;
} Stack;

void InsertEdge(Node *list, int or, int dst);
void SCCTarjan(Node *list, int numberOfVertices, int numberOfEdges, int *minSCC);
void TarjanVisit(int vertex, int numberOfVertices, int *d, int *low, int *visited, Node *list, int *minSCC);
void countSort(int **arr, int numberOfVertices, int numberOfEdges, int u);
int freeLLandCreateEdgeList(Node *list, int numberOfVertices, int **orderedEdgeList, int *minSCC);

Stack vertices_stack;
int numberOfSCCs = 0;

int main(){
  int numberOfVertices;
  int numberOfEdges;
	int or,dst;
  int i;

  if (scanf("%d", &numberOfVertices) != 1) {
		printf("Erro ao ler o numero de vertices");
		return 1;
	}

	if (scanf("%d", &numberOfEdges) != 1) {
		printf("Erro ao ler o numero de arestas");
		return 1;
	}

	Node *list = (Node*) malloc(sizeof(Node)*numberOfVertices);
	/* minSCC stores the identifier for each SCC, starting at index 0 for the
	first SCC and so on */
	int *minSCC = (int*) malloc(sizeof(int)*numberOfVertices);
	int **orderedEdgeList = (int**) malloc(sizeof(int*)*numberOfEdges);

  for(i=0; i<numberOfVertices; i++){
    list[i].vertex = i+1;
    list[i].next = NULL;
		list[i].isInStack = 0;
  }

  for(i=0; i<numberOfEdges; i++){
		if (scanf("%d %d", &or, &dst) != 2) {
			printf("Erro ao ler arestas");
			return 1;
		}
		if (or != dst) {
			InsertEdge(list, or, dst);
		}
  }

	SCCTarjan(list, numberOfVertices, numberOfEdges, minSCC);

	printf("%d\n", numberOfSCCs);

  int possibleBridges = freeLLandCreateEdgeList(list, numberOfVertices, orderedEdgeList, minSCC);
	free(minSCC);

	/* sort the list of edges twice, first by its destiny, then by its origin */
	countSort(orderedEdgeList, numberOfVertices, possibleBridges, 1);
	countSort(orderedEdgeList, numberOfVertices, possibleBridges, 0);

	/* count the number of bridges between SCCs ignoring possible repetitions */
	int prevOr = -1;
	int prevDst = -1;
	int bridges = 0;
	int printOr, printDst;
	for (i=0; i<possibleBridges; i++) {
		if (orderedEdgeList[i][0] != orderedEdgeList[i][1]) {
			printOr = orderedEdgeList[i][0];
			printDst = orderedEdgeList[i][1];
			if (printOr != prevOr || printDst != prevDst) {
				bridges++;
			}
			prevOr = printOr;
			prevDst = printDst;
		}
	}
	printf("%d\n", bridges);

	/* print the bridges between SCCs, also ignoring possible repetitions */
	prevOr = -1;
	prevDst = -1;
	for (i=0; i<possibleBridges; i++) {
		if (orderedEdgeList[i][0] != orderedEdgeList[i][1]) {
			printOr = orderedEdgeList[i][0];
			printDst = orderedEdgeList[i][1];
			if (printOr != prevOr || printDst != prevDst) {
				printf("%d %d\n", printOr, printDst);
			}
			prevOr = printOr;
			prevDst = printDst;
		}
	}

	for(i=0; i<possibleBridges; i++){
		free(orderedEdgeList[i]);
	}
	free(orderedEdgeList);

	return 0;
}

void InsertEdge(Node *list, int or, int dst){
  Node *aux = (Node*) malloc(sizeof(Node));
  aux->vertex = dst;
	aux->isInStack = 0;

  if(list[or-1].next == NULL) {
		aux->next = NULL;
	} else {
    aux->next = list[or-1].next;
  }
	list[or-1].next = aux;
}

void SCCTarjan(Node *list, int numberOfVertices, int numberOfEdges, int *minSCC) {
	int i;
	/* int that keeps track of visiting times for Tarjan algorithm
	and a pointer to it to update it easily */
	int visited = 0;
	int *ptr_visited = &visited;

	int *d = (int*) malloc(sizeof(int)*numberOfVertices);
	int *low = (int*) malloc(sizeof(int)*numberOfVertices);

	vertices_stack.vertices = (int*)malloc(sizeof(int)*numberOfVertices);
	/* saves the size of the stack-1 so that we can access to the first element
	using index 0 */
	vertices_stack.top = -1;

	for(i=0; i<numberOfVertices; i++){
		d[i] = -1;
		minSCC[i] = 0;
	}

	for(i=0; i<numberOfVertices; i++){
		if (d[i] == -1) {
			TarjanVisit(i, numberOfVertices, d, low, ptr_visited, list, minSCC);
		}
	}

	free(vertices_stack.vertices);
	free(d);
	free(low);
}

void TarjanVisit(int vertex, int numberOfVertices, int *d, int *low, int *visited, Node *list, int *minSCC) {
	Node *tmp;
	d[vertex] = *visited;
	low[vertex] = *visited;

	*visited = *visited+1;

	/* increase stack size and add the vertex to the list */
	vertices_stack.top++;
	vertices_stack.vertices[vertices_stack.top] = vertex;

	list[vertex].isInStack = 1;

	tmp = list[vertex].next;
	while(tmp != NULL) {
		if(d[(tmp->vertex)-1] == -1 || list[(tmp->vertex)-1].isInStack) {
			if(d[(tmp->vertex)-1] == -1) {
				TarjanVisit((tmp->vertex)-1, numberOfVertices, d, low, visited, list, minSCC);
			}
			if(low[(tmp->vertex)-1] < low[vertex]) {
				low[vertex] = low[(tmp->vertex)-1];
			}
		}
		tmp = tmp->next;
	}
	if(d[vertex] == low[vertex]) {
		int v;
		int min = 0;

		while ((v = vertices_stack.vertices[vertices_stack.top--]) != -1){
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

void countSort(int **arr, int numberOfVertices, int numberOfEdges, int u) {
	int i;
  int output[numberOfEdges][2];
  int count[numberOfVertices+1];

  memset(count, 0, sizeof(count));

  for (i=0; i<numberOfEdges; ++i) {
		++count[arr[i][u]];
	}

  for (i=1; i <= numberOfVertices; ++i) {
		count[i] += count[i-1];
	}

  if (u == 1) {
		for (i=0; i<numberOfEdges; ++i) {
			output[count[arr[i][u]]-1][1] = arr[i][1];
			output[count[arr[i][u]]-1][0] = arr[i][0];
			--count[arr[i][u]];
		}
	} else if (u == 0) {
		for (i=numberOfEdges-1; i>=0; --i) {
			output[count[arr[i][u]]-1][1] = arr[i][1];
			output[count[arr[i][u]]-1][0] = arr[i][0];
			--count[arr[i][u]];
		}
	}

	for (i=0; i<numberOfEdges; ++i){
		arr[i][0] = output[i][0];
    arr[i][1] = output[i][1];
	}
}

/* This function creates a list of edges that connect two different SCCs.
It simultaneously creates that list while freeing the memory allocated
for the adjacency list, thus reducing the total memory needed */
int freeLLandCreateEdgeList(Node *list, int numberOfVertices, int **orderedEdgeList, int *minSCC) {
	int i, j, or, dst;
	for (i=0, j=0; i<numberOfVertices; i++) {
		Node *head = &list[i];
		while (head->next != NULL) {
			Node *tmp = head->next;
      or = minSCC[head->scc_id];
      dst = minSCC[list[(tmp->vertex)-1].scc_id];
			/* if the origin is different from the destiny, this edge connects two different SCCs */
      if (or != dst) {
        orderedEdgeList[j] = (int*) malloc(sizeof(int)*2);
        orderedEdgeList[j][0] = or;
        orderedEdgeList[j][1] = dst;
				/* j keeps track of how many edges between different SCCS
				we added to the list */
        j++;
      }
			head->next = tmp->next;
			free(tmp);
		}
	}
	free(list);
  return j;
}
