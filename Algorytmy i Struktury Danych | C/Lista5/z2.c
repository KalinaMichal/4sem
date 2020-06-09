#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

/*********************************************************************************************************************/
/********************************************* KOLEJKA ***************************************************************/
/*********************************************************************************************************************/
//----
typedef struct Element_kolejki{
	int liczba;
	float priorytet;
} Element;

typedef struct Kolejka{
	Element** e;
	int size;
	int size_max;
} PriorityQueue;

//----
typedef struct Element_listy{
	int liczba;
	struct Element_listy* next;
} ListElement;

typedef struct Lista{
	ListElement* head;
	ListElement* body;
} List;


List* list_make(void){
	List* new = malloc(sizeof(List));
	new->head = new->body = NULL;
	return new;
}

PriorityQueue* queue_make(void){
	PriorityQueue* pq = malloc(sizeof(PriorityQueue));
	pq->size=0;
	pq->size_max=128;
	pq->e = calloc(pq->size_max, sizeof(Element*));
	return pq;
}

void list_insert(int licz, List* lista){
	ListElement* tmp = malloc(sizeof(ListElement));
	tmp->next = NULL;
	tmp->liczba = licz;
	if(lista->body){
		lista->body->next = tmp;
		lista->body = tmp;
		return;
	}
	lista->head = lista->body = tmp;
}

int list_pop(List* lista){
	if(lista->head){
		ListElement* tmp = lista->head;
		int licz = tmp->liczba;
    	    	lista->head = tmp->next;
    	    	if(!lista->head)	lista->body=NULL;
		free(tmp);
        	return licz;
	}
	return INT_MIN;
}

int expandQueue(PriorityQueue* pq){
	pq->size_max *= 2;
	void* tmp;
    	if((tmp = realloc(pq->e, pq->size_max * sizeof(Element*))) == NULL){
        	free(pq->e);
        	return 0;
    	}
    	pq->e = tmp;
    	return 1;
}


int right(int i)	{return 2*i+2;}
int left(int i)		{return 2*i+1;}
int parent(int i) 	{return (i-1)/2;}
int queue_empty(PriorityQueue* q){if(q->size) return 0; else return 1;}
int queue_top(PriorityQueue* q){if(q->size) return q->e[0]->liczba; else return INT_MIN;}
void queue_print(PriorityQueue* pq){for(int i=0; i < pq->size; i++)printf("(%d, %f) ",pq->e[i]->liczba,pq->e[i]->priorytet);}

void queue_swap(Element* a, Element* b){
	Element tmp = *a;
	*a = *b;
    	*b = tmp;
}

void shift_up(int x, PriorityQueue* q){
	while(x > 0 && q->e[parent(x)]->priorytet > q->e[x]->priorytet){
		queue_swap(q->e[x], q->e[parent(x)]);
		x = parent(x);
	}
}



int queue_insert(int v, float p, PriorityQueue* pq){
    pq->e[pq->size] = malloc(sizeof(Element));
    pq->e[pq->size]->liczba = v;
    pq->e[pq->size]->priorytet = p;
    int i = pq->size++;
    shift_up(i, pq);
}

int queue_pop(PriorityQueue* pq){
	void help(int i, PriorityQueue* pq){
		int l = left(i);
		int r = right(i);
		int smallest;
		if(l < pq->size && pq->e[l]->priorytet < pq->e[i]->priorytet)	smallest = l;
		else 	smallest = i;
		if(r < pq->size && pq->e[r]->priorytet < pq->e[smallest]->priorytet)	smallest = r;
		if(smallest != i) {
			queue_swap(pq->e[i], pq->e[smallest]);
			help(smallest, pq);
		}
	}
	if(pq->size){
		int result = pq->e[0]->liczba;
		pq->e[0] = pq->e[--pq->size];
		help(0, pq);
   	     	return result;
    	}	
    	return INT_MIN;
}

void queue_priority(int licz, int p, PriorityQueue* pq) {
	List* list= list_make();
	for(int i = 0; i < pq->size; i++) {
		if(pq->e[i]->priorytet > p && pq->e[i]->liczba == licz) {
			pq->e[i]->priorytet = p;
			list_insert(i,list);
		}
	}
   	 int tmp;
    	while((tmp = list_pop(list)) != INT_MIN)	shift_up(tmp, pq);
}

/**************************/
/******* graf help	***/
/**************************/

typedef struct Sasiedzi{
	Element** edges;
	int size;
	int size_max;
} Neighbors;

typedef struct Graf_skierowany{
	int n;
	int m;
	Neighbors** neighbors;
} Directed_Graph;

Neighbors* neighbors_make(int n){
	Neighbors* a = malloc(sizeof(Neighbors));
	a->size = 0;
	a-> size_max = n;
	a->edges = malloc(sizeof(Element*) * a->size_max);
	return a;
}

Element* getElementAt(int i, Neighbors* a){
    	if(i<0 || i >= a->size)	return NULL;
	return a->edges[i];
}

clock_t start, end;
int s,n,m;
/*********************************************************************************************************************/
/********************************************** GRAF *****************************************************************/
/*********************************************************************************************************************/

/*int neighbors_expand(Neighbors* a){
	a->size_max *= 2;
	void* tmp;
	if((tmp = realloc(a->edges, a->size_max * sizeof(Element*))) == NULL){
		free(a->edges);
        	return 0;
    	}
    	a->edges = tmp;
    	return 1;
}
*/
void neighbors_insert(int v, float p, Neighbors* a){
	//if(a->size == a->size_max)	if(!neighbors_expand(a))	return 0;
    	a->edges[a->size] = malloc(sizeof(Element));
    	a->edges[a->size]->liczba = v;
    	a->edges[a->size]->priorytet = p;
    	a->size++;
    	//return 1;
}

/*
Directed_Graph* make_Directed_Graph(int n, int m){
    	Directed_Graph* dg = malloc(sizeof(Directed_Graph));
    	dg->n = n;
    	dg->m = m;
    	dg->neighbors = malloc(sizeof(Neighbors*) * (n+1));
    	for(int i=0; i<n+1; i++)	dg->neighbors[i]=neighbors_make(dg->n);
    	return dg;
}

void edges_add(DirectedGraph* dg){
    for(int i=0; i < (dg->m); i++){
        int u,v;
        float w;
        scanf("%d %d %f",&u, &v, &w);
        insertAdjacents(v, w, dg->adjacents[u]);
    } 
}*/

float weight_getter(int u, int v, Directed_Graph* graf) {
	for(int i=0; i < graf->neighbors[u]->size; i++){
        	Element* tmp = getElementAt(i, graf->neighbors[u]);
        	if(tmp->liczba == v)	return tmp->priorytet;
    	}
    	return INT_MIN;
}

void shortestPath(int s, Directed_Graph* graf) {
    	int n= graf->n;
    	float dist[n+1];
    	int prev[n+1];
    	for(int i = 1; i <= n; i++) {
        	dist[i] = INT_MAX;
        	prev[i] = INT_MIN;
    	}
    	dist[s] = 0;
    	PriorityQueue* pq = queue_make();
    	for(int i = 1; i <= n; i++)	queue_insert(i, dist[i], pq);
    	while(!queue_empty(pq)) {
        	int u = queue_pop(pq);
        	for(int i=0; i< graf->neighbors[u]->size; i++){
            		Element* tmp = getElementAt(i, graf->neighbors[u]); 
            		int v = tmp->liczba;
            		float w = tmp->priorytet;
            		if(dist[v] > dist[u] + w) {
                		dist[v] = dist[u] + w;
                		prev[v] = u;
                		queue_priority(v, dist[v],pq);
            		}
        	}
    	}

	printf("-------------------------------\n");
    	for(int i = 1; i <= n; i++)
        	if(dist[i] == INT_MAX)	printf("[%d]: Nie ma sciezki\n",i);
        	else			printf("[%d]: %f\n",i,dist[i]);
	printf("-------------------------------\n");
    	for(int i = 1; i <= n; i++) {
		int tmp = i;	int p = prev[i];
        	fprintf(stderr,"[%d] ",i);
        	while(p != INT_MIN) {
           		printf("<---(%f)--- [%d] ",weight_getter(p, tmp,graf),p);
            		tmp = p;
            		p = prev[p];
        	}
        	printf("\n");
    	}
	printf("-------------------------------\n");
}

/*********************************************************************************************************************/
/********************************************** MAIN *****************************************************************/
/*********************************************************************************************************************/

int main(void){
	printf("n: ");
    	scanf("%d",&n);
	printf("m: ");
    	scanf("%d",&m);
//tworzenie grafu skierowanego
    	Directed_Graph* dg = malloc(sizeof(Directed_Graph));
    	dg->n = n;
    	dg->m = m;
    	dg->neighbors = malloc(sizeof(Neighbors*) * (n+1));
    	for(int i=0; i<n+1; i++)	dg->neighbors[i]=neighbors_make(dg->n);
//----
//dodawanie
	for(int i=0; i < (dg->m); i++){
		int u,v;
        	float w;
        	scanf("%d %d %f",&u, &v, &w);
        	neighbors_insert(v, w, dg->neighbors[u]);
    	}    
//----
	printf("start: ");
    	scanf("%d",&s);
//zegar i program
    	start = clock();
    	shortestPath(s,dg);
    	end = clock();
//.....

    	printf("time: %f ms\n",((double) (end - start)) / CLOCKS_PER_SEC * 1000);
    	return 0;
}
