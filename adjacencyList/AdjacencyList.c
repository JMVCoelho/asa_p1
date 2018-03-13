#include <stdio.h>
#include <stdlib.h>

typedef struct node{
	int vertex;
	struct node* next;
}Node;


void Print(Node *list, int numberOfVertices);
void InsertArc(Node *list, int a, int b); 

int main(){
  int a,b;
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
	scanf("%d %d", &a, &b);
    InsertArc(list,a,b);
  }

  Print(list, numberOfVertices);
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

void InsertArc(Node *list, int a, int b){
  Node *aux;
  Node *tmp;
 
  aux = (Node*) malloc((int)sizeof(Node));    //retorno de um ponteiro genérico.
  aux->vertex = b;
  aux->next = NULL;
  
  if(list[a-1].next == NULL) // Caso a list estiver vazia - Insere
    list[a-1].next = aux;
  else	{ // insere como primeiro da list. O(1) 
    tmp = list[a-1].next;
    aux->next = tmp;
    list[a-1].next = aux;
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
