#include <stdio.h>
#include <stdlib.h>

typedef struct node{
	int vertex;
	struct node *next;
} Node;

typedef struct stack{
	int top;
	int *vertices;
} Stack;

typedef struct scc{
	int size;
	int min;
	int *vertices;
	struct scc *next;
} SCC;

Stack vertices_stack;
int numberOfSCCs = 0;
SCC *listSCCs = NULL;

void Print(Node *list, int numberOfVertices);
void InsertArc(Node *list, int or, int dst);
void freeList(Node *list, int numberOfVertices);

void stack_push(int v, int numberOfVertices){
	vertices_stack.top++;
	if(vertices_stack.top < numberOfVertices){
		vertices_stack.vertices[vertices_stack.top] = v;
	}
	else{
		printf("Stack cheia, erro algures. APAGA-ME");
	}
}

int inStack(int vertex){
	int i;
	if (vertices_stack.top == -1){
		return 0;
	}
	else{
		for(i=0; i < vertices_stack.top; i++){
			if(vertices_stack.vertices[i]==vertex){
				return 1;
			}
		}
	}
	return 0;
}

int stack_pop(){
	return vertices_stack.top < 0 ? -1 : vertices_stack.vertices[vertices_stack.top--];
}

void TarjanVisit(int vertex, int numberOfVertices, int *d, int *low, int *visited, Node *list, int *vertexBelongsToSCC, int *minSCC){
	Node *tmp;
	d[vertex] = *visited;
	low[vertex] = *visited;

	*visited=*visited+1;
	stack_push(vertex, numberOfVertices);

	tmp = list[vertex].next;
	while(tmp != NULL){
		if(d[(tmp->vertex)-1]==-1 || inStack((tmp->vertex)-1)){
			if(d[(tmp->vertex)-1]==-1){
				TarjanVisit((tmp->vertex)-1, numberOfVertices, d, low, visited, list, vertexBelongsToSCC, minSCC);
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

		while( (v = stack_pop()) != -1){
			if (min == 0 || v < min) {
				min = v;
			}
			vertexBelongsToSCC[v] = numberOfSCCs;
			if(vertex == v){
				//this SCC ends here
				break;
			}
		}
		minSCC[numberOfSCCs] = min+1;
		numberOfSCCs++;
	}
}

int SCCTarjan(Node *list, int numberOfVertices){
	int i;
	int visited = 0;
	int *ptr_visited = &visited;

	int *d = (int*) malloc(sizeof(int)*numberOfVertices);
	int *low = (int*) malloc(sizeof(int)*numberOfVertices);

	int *vertexBelongsToSCC = (int*) malloc(sizeof(int)*numberOfVertices);
	int *minSCC = (int*) malloc(sizeof(int)*numberOfVertices);

	vertices_stack.vertices = (int*)malloc(sizeof(int)*numberOfVertices);
	vertices_stack.top = -1;

	for(i=0; i<numberOfVertices; i++){
		d[i] = -1;
		vertexBelongsToSCC[i] = 0;
		minSCC[i] = 0;
	}

	for(i=0; i<numberOfVertices; i++){
		if (d[i] == -1){
			TarjanVisit(i, numberOfVertices, d, low, ptr_visited, list, vertexBelongsToSCC, minSCC);
		}
	}

	printf("SSCs:%d\n ", numberOfSCCs);
	printf("belongs: ");
	for(i=0; i<numberOfVertices; i++){
		printf("%d  ", vertexBelongsToSCC[i]+1);
	}
	printf("\nmins: ");
	for(i=0; i<numberOfVertices; i++){
		printf("%d  ", minSCC[i]);
	}


	free(vertexBelongsToSCC);
	free(minSCC);
	free(d);
	free(low);
	return 0;
}

int main(){
  int or,dst;
  int numberOfVertices;
  int numberOfEdges;
  int i;

  scanf("%d",&numberOfVertices);
  //Node list[numberOfVertices];
	Node *list = (Node*)malloc(sizeof(Node)*numberOfVertices);

  //Inicialização da lista.
  for(i=0; i<numberOfVertices; i++){
    list[i].vertex = i+1;
    list[i].next = NULL;
  }

  //Leitura do numero de arestas
  scanf("%d", &numberOfEdges);

  //leitura dos vertices (a,b) - passagem para a função que insere um arco.
  for(i=0; i<numberOfEdges; i++){
	scanf("%d %d", &or, &dst);
    InsertArc(list,or,dst);
  }
	SCCTarjan(list, numberOfVertices);
	freeList(list, numberOfVertices);
}

void InsertArc(Node *list, int or, int dst){
  Node *aux = (Node*) malloc(sizeof(Node));    //retorno de um ponteiro genérico.
  aux->vertex = dst;

  if(list[or-1].next == NULL) // Caso a list estiver vazia - Insere
		aux->next = NULL;
  else	{ // insere como primeiro da list. O(1)
    aux->next = list[or-1].next;
  }
	list[or-1].next = aux;
}

void Print(Node *list, int numberOfVertices){
  int i;
  Node *tmp;
  for(i=0; i<numberOfVertices; i++) {
    tmp = list[i].next;
    printf("%2d: (%d) ==>", i, list[i].vertex);
    while (tmp != NULL) {
      printf("%d  ", tmp->vertex);
      tmp = tmp->next;
    }
    printf("\n");
  }
}

void freeList(Node *list, int numberOfVertices) {
	for (int i=0; i<numberOfVertices; i++) {
		Node *head = &list[i];
		while (head->next != NULL) {
			Node *tmp = head->next;
			head->next = tmp->next;
			free(tmp);
		}
	}
	free(list);
}

//Para inserir ordenadamente:
/* 		          //insere os vértices ordenados
    else if (tmp->next == NULL) {
      aux->next = tmp->next;
      tmp->next = aux;
    }
    else {
      while((tmp->next != NULL) &&(tmp->next->vertex < b))
	tmp = tmp->next;
      aux->next = tmp->next;
      tmp->next = aux;
    }*/
//e adicionar um if(tmp->vertex > b) antes de aux->next = temp (l68~)
