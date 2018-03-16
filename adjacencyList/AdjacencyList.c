#include <stdio.h>
#include <stdlib.h>

typedef struct node{
	int vertex;
	struct node *next;
} Node;


typedef struct stack{
	int top;
	int* vertices;
}Stack;


Stack vertices_stack;


void Print(Node *list, int numberOfVertices);
void InsertArc(Node *list, int or, int dst);


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

void TarjanVisit(int vertex,int numberOfVertices,  int* d, int* low, int* visited, Node* list){
	
	Node *tmp;
	
	d[vertex] = *visited;
	low[vertex] = *visited;	
	
	*visited=*visited+1;
	printf("%d\n", *visited);
	stack_push(vertex, numberOfVertices);
	
	tmp = list[vertex].next;
	while(tmp != NULL){
		if(d[tmp->vertex]==-1 || inStack(tmp->vertex)){
			if(d[tmp->vertex]==-1){
				TarjanVisit(tmp->vertex, numberOfVertices, d, low, visited, list);
			}
			if(low[tmp->vertex]<low[vertex]){
				low[vertex]=low[tmp->vertex];
			}
		}
		tmp=tmp->next;
	}
	if(d[vertex]==low[vertex]){
		int v;
		printf("SSC:\n");
		while((v =stack_pop()) != -1){
			printf("%d\n", v+1);
			if(vertex == v){
				printf("NEW SCC\n");
				break;
			}
		}			
	}
}	
	
	


int SCCTarjan(Node* list, int numberOfVertices){ 
	int i;
	int visited = 0;
	int* ptr_visited = &visited;
	

	int d[numberOfVertices];
	int low[numberOfVertices];
	
	vertices_stack.vertices = (int*)malloc(sizeof(int)*numberOfVertices);
	vertices_stack.top = -1;
	
	
	for(i=0; i<numberOfVertices; i++){
		d[i] = -1;
	}
	
	for(i=0; i<numberOfVertices; i++){
		if (d[i] == -1){
			TarjanVisit(i,numberOfVertices, d, low, ptr_visited, list);
		}
	}
	
	
	return 0;
}

int main(){
  int or,dst;
  int numberOfVertices;
  int numberOfArcs;
  int i;

  scanf("%d",&numberOfVertices);
  Node list[numberOfVertices];

  //Inicialização da lista.
  for(i=0; i<numberOfVertices; i++){
    list[i].vertex = i+1;
    list[i].next = NULL;
  }

  //Leitura do numero de arestas
  scanf("%d", &numberOfArcs);

  //leitura dos vertices (a,b) - passagem para a função que insere um arco.
  for(i=0; i<numberOfArcs; i++){
	scanf("%d %d", &or, &dst);
    InsertArc(list,or,dst);
  }
	SCCTarjan(list, numberOfVertices);
}

void InsertArc(Node *list, int or, int dst){
  Node *aux = (Node*) malloc(sizeof(Node));    //retorno de um ponteiro genérico.
  aux->vertex = dst;
  aux->next = NULL;

  if(list[or-1].next == NULL) // Caso a list estiver vazia - Insere
    list[or-1].next = aux;
  else	{ // insere como primeiro da list. O(1)
    Node *tmp = list[or-1].next;
    aux->next = tmp;
    list[or-1].next = aux;
  }
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
