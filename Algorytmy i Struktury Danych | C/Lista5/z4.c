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
	PriorityQueue* kolejka = malloc(sizeof(PriorityQueue));
	kolejka->size=0;
	kolejka->size_max=128;
	kolejka->e = calloc(kolejka->size_max, sizeof(Element*));
	return kolejka;
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

int expandQueue(PriorityQueue* kolejka){
	kolejka->size_max *= 2;
	void* tmp;
    	if((tmp = realloc(kolejka->e, kolejka->size_max * sizeof(Element*))) == NULL){
        	free(kolejka->e);
        	return 0;
    	}
    	kolejka->e = tmp;
    	return 1;
}


int right(int i)	{return 2*i+2;}
int left(int i)		{return 2*i+1;}
int parent(int i) 	{return (i-1)/2;}
int queue_empty(PriorityQueue* q){if(q->size) return 0; else return 1;}
int queue_top(PriorityQueue* q){if(q->size) return q->e[0]->liczba; else return INT_MIN;}
void queue_print(PriorityQueue* kolejka){for(int i=0; i < kolejka->size; i++)printf("(%d, %f) ",kolejka->e[i]->liczba,kolejka->e[i]->priorytet);}

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



int queue_insert(int v, float p, PriorityQueue* kolejka){
    	kolejka->e[kolejka->size] = malloc(sizeof(Element));
    	kolejka->e[kolejka->size]->liczba = v;
    	kolejka->e[kolejka->size]->priorytet = p;
    	int i = kolejka->size++;
    	shift_up(i, kolejka);
}

int queue_pop(PriorityQueue* kolejka){
	void help(int i, PriorityQueue* kolejka){
		int l = left(i);
		int r = right(i);
		int smallest;
		if(l < kolejka->size && kolejka->e[l]->priorytet < kolejka->e[i]->priorytet)	smallest = l;
		else 	smallest = i;
		if(r < kolejka->size && kolejka->e[r]->priorytet < kolejka->e[smallest]->priorytet)	smallest = r;
		if(smallest != i) {
			queue_swap(kolejka->e[i], kolejka->e[smallest]);
			help(smallest, kolejka);
		}
	}
	if(kolejka->size){
		int result = kolejka->e[0]->liczba;
		kolejka->e[0] = kolejka->e[--kolejka->size];
		help(0, kolejka);
   	     	return result;
    	}	
    	return INT_MIN;
}

void queue_priority(int licz, int p, PriorityQueue* kolejka) {
	List* list= list_make();
	for(int i = 0; i < kolejka->size; i++) {
		if(kolejka->e[i]->priorytet > p && kolejka->e[i]->liczba == licz) {
			kolejka->e[i]->priorytet = p;
			list_insert(i,list);
		}
	}
   	 int tmp;
    	while((tmp = list_pop(list)) != INT_MIN)	shift_up(tmp, kolejka);
}

/*********************************************************************************************************************/
/********************************************** GRAF *****************************************************************/
/*********************************************************************************************************************/

typedef struct Edge{
    	int u;
    	int v;
    	float w;
} Edge;

Edge* edge_make_for_kru(int u, int v, float w){
    	Edge* rtr = malloc(sizeof(Edge));
    	rtr->u=u;
    	rtr->v=v;
   	rtr->w=w;
    	return rtr;
}

typedef struct Sasiedzi{
	Element** edges;
	int size;
	int size_max;
} Neighbors;

typedef struct Graf{
    	Neighbors** neighbors;
    	Edge** edges;
    	int n;
    	int m;
} Graph;

Neighbors* neighbors_make_for_p(int n){
    	Neighbors* a = malloc(sizeof(Neighbors));
    	a->size =0;
    	a -> size_max = n;
    	a->edges = malloc(sizeof(Element*) * a->size_max);
    	return a;
}

int expandNeighbors(Neighbors* a){
    a->size_max *= 2;
    void* tmp;
    if((tmp = realloc(a->edges, a->size_max * sizeof(Element*))) == NULL){
        free(a->edges);
        return 0;
    }
    a->edges = tmp;
    return 1;
}

void neighbors_insert(int v, float p, Neighbors* a){
    	a->edges[a->size] = malloc(sizeof(Element));
    	a->edges[a->size]->liczba = v;
    	a->edges[a->size]->priorytet = p;
    	a->size++;
}

float weight_getter(int u, int v, Graph* graf) {
	for(int i=0; i < graf->neighbors[u]->size; i++){
        	Element* tmp;
		if(i<0 || i >= graf->neighbors[u]->size)	tmp=NULL;
    		else 						tmp=graf->neighbors[u]->edges[i];
        	if(tmp->liczba == v)	return tmp->priorytet;
    	}
    	return INT_MIN;
}

/*********************************************************************************************************************/
/********************************************** RESZTA ***************************************************************/
/*********************************************************************************************************************/

void rand_t(int s, int* k, float* W, int* M, Graph* graf) {
    	(*k)=(*W)=(*M) = 0;
    	int amm_vist = 1;
    	int n = graf->n;
        int visited[n+1];
	visited[s] = 1;
    	for(int i = 1; i <= n; i++)	visited[i] = 0;

    	printf("  %d",s);
    	while(amm_vist < n) {
        	int adj[n-1];
        	int j = 0;
        	for(int i = 1; i <= n; i++) {
            		if(i == s)	continue;
            		adj[j++] = i;
        	}
        	int index = rand() % (n - 1);
        	int next = adj[index];
        	(*k)++;
        	(*W) += weight_getter(s, next, graf);
        	if(!visited[next]) {
            		visited[next] = 1;
            		amm_vist++;
        	}
        	s = next;
        	printf(" %d",s);
    	}
    	printf("\n");
    	(*M) += sizeof(visited) + (5 + n - 1)*sizeof(int);
}

void najw_trawel(int s, int* k, float* W, int* M, Graph* graf) {
    	int index = 1;
    	int n = graf->n;
    	int visited[n+1];
	visited[s] = 1;
    	(*k)=(*W)=(*M)=0;
    	for(int i = 1; i <= n; i++)	visited[i] = 0;

    	printf("  %d",s);
    	while(index < n) {
        	int next;
        	float w = INT_MAX;
        	for(int i=0; i< graf->neighbors[s]->size; i++){
			Element* tmp = NULL;
    			if(!(i<0 || i >= graf->neighbors[s]->size))	tmp = graf->neighbors[s]->edges[i];

           		int v = tmp->liczba;
            		float tmpw = tmp->priorytet;
			if(!visited[v] && tmpw < w) {
				next = v;
                		w = tmpw;
            		}
        	}

        	(*k)++;
        	(*W) += w;
        	visited[next] = 1;
        	index++;

        	s = next;
        	printf(" %d",s);
    	}
    	printf("\n");
    	(*M) += sizeof(Element*) + 5*sizeof(int) + sizeof(visited) ;
}
void walk(Neighbors* result[], int u, int* k) {
	printf("  %d ",u);
    	for(int i=0; i< result[u]->size; i++){
		Element* tmp = NULL;
    		if(!(i<0 || i >= result[u]->size))	tmp = result[u]->edges[i];
 		int v = tmp->liczba;
        	(*k) += 2;
        	walk(result, v, k);
    	}
}

void traversal_mst(int s, int* k, float* W, int* M, Graph* graf) {
	int n=graf->n;
    	(*k)=(*W)=(*M) = 0;

    	float key[n+1];
    	int prev[n+1];
    	int inMST[n+1];
    	for(int i = 1; i <= n; i++) {
        	key[i] = INT_MAX;
        	prev[i] = INT_MIN;
        	inMST[i] = 0;
    	}
    	key[s] = 0;

    	PriorityQueue* pq = queue_make();
    	for(int i = 1; i <= n; i++)	queue_insert(i, key[i], pq);

	while(!queue_empty(pq)) {
        	int u = queue_pop(pq);
        	inMST[u] = 1;
		for(int i=0; i< graf->neighbors[u]->size; i++){
			Element* tmp = NULL;
    			if(!(i<0 || i >= graf->neighbors[u]->size))	tmp = graf->neighbors[u]->edges[i];
			int v = tmp->liczba;
            		float w = tmp->priorytet;
            		if(inMST[v] == 0 && key[v] > w) {
                		key[v] = w;
                		queue_priority(v, key[v],pq);
                		prev[v] = u;           
            		}
        	}
    	}

    	Neighbors** result = malloc(sizeof(Neighbors*) *(n+1));
    	for(int i=0; i<n+1; i++){
    		Neighbors* tmp = malloc(sizeof(Neighbors));
    		tmp->size =0;
    		tmp -> size_max = n;
    		tmp->edges = malloc(sizeof(Element*) * tmp->size_max);
		result[i]=tmp;
	}
    	for(int i = 1; i <= n; ++i) {
        	if(i == s)	continue;
        	float w = weight_getter(prev[i], i, graf);
        	(*W) += 2*w;
        	neighbors_insert(i, w,result[prev[i]]);
    	}
    	walk(result, s, k);
    	printf("\n");
    	(*M) += sizeof(key) + sizeof(prev) + sizeof(inMST) + sizeof(result) + sizeof(Element*) + 4*sizeof(int);
}

/*********************************************************************************************************************/
/********************************************** MAIN *****************************************************************/
/*********************************************************************************************************************/

int main(void){
//dane - init
    	int n, m, M,random, k;
   	clock_t start, end; 
	float W;
	srand (time(NULL));

//wprowadzanie danych
	printf("n: ");
    	scanf("%d",&n);
    	m = n*(n-1)/2;
	printf("m = %d\n",m);
    	Graph* graf = malloc(sizeof(Graph));
   	graf->n = n;
    	graf->m = m;
    	graf->neighbors = malloc(sizeof(Neighbors*) * (n+1));
    	for(int i=0; i<n+1; i++)
	{
	    	Neighbors* tmp = malloc(sizeof(Neighbors));
	    	tmp->size =0;
	    	tmp -> size_max = n;
	    	tmp->edges = malloc(sizeof(Element*) * tmp->size_max);

        	graf->neighbors[i]=tmp;
	}
	//dodawane krawedzi
	for(int i=0; i < graf->m; i++){
        	int u,v;
        	float w;
        	scanf("%d %d %f",&u, &v, &w);
        	neighbors_insert(v, w, graf->neighbors[u]);
        	neighbors_insert(u, w, graf->neighbors[v]);
    	}  
//...

    	random = rand() % n + 1; 
//wyswietlanie
	printf("------------------------------\n");	
	
    	start = clock();
    	rand_t(random, &k, &W, &M,graf);
    	end = clock();
    	printf(" | k: %d  | W: %f  | M: %d  | t: %f\n\n",k,W,M,((double) ((end-start) / 1000.0)));
    	
    	start = clock();
    	najw_trawel(random, &k, &W, &M,graf);
    	end = clock();
    	printf(" | k: %d  | W: %f  | M: %d  | t: %f\n\n",k,W,M,((double) ((end-start) / 1000.0)));
	
    	start = clock();
    	traversal_mst(random, &k, &W, &M,graf);
    	end = clock();
    	printf(" | k: %d  | W: %f  | M: %d  | t: %f\n",k,W,M,((double) ((end-start) / 1000.0)));
//...
    	return 0;
}
