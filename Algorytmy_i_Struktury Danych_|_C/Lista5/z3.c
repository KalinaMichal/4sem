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


void prima(int s, Graph* graf){
	int n=graf->n;
    	float key[graf->n+1];
    	int prev[graf->n+1];
    	int inMST[graf->n+1];
    	float rtr = 0;
    	for(int i = 1; i <= graf->n; i++) {
        	key[i] = INT_MAX;
        	prev[i] = INT_MIN;
        	inMST[i] = 0;
    	}
    	key[s] = 0;

    	PriorityQueue* kolejka = queue_make();
    	for(int i = 1; i <= graf->n; i++)	queue_insert(i, key[i], kolejka);

    	while(!queue_empty(kolejka)) {
        	int u = queue_pop(kolejka);
        	inMST[u] = 1;
        	for(int i=0; i< graf->neighbors[u]->size; i++){
        		Element* tmp;
			if(i<0 || i >= graf->neighbors[u]->size)	tmp=NULL;
    			else tmp=graf->neighbors[u]->edges[i];
            		int v = tmp->liczba;
		    	float w = tmp->priorytet;
		    	if(inMST[v] == 0 && key[v] > w) {
		    	    	key[v] = w;
		        	queue_priority(v, key[v],kolejka);
		        	prev[v] = u;           
		    	}
        	}
    	}

    	for(int i = 1; i <= n; ++i) {
		if(i == s)	continue;
        	float w = weight_getter(prev[i], i,graf);
        	rtr += w;
        	if(prev[i] < i)	printf("%d %d %f\n",prev[i],i,w);
        	else		printf("%d %d %f\n",i,prev[i],w);
    	}
    	printf("%f\n",rtr);
}


typedef struct Subset {int p, rank;} Subset;

int find(Subset subs[], int i) {
	if(subs[i].p != i)	subs[i].p = find(subs, subs[i].p);
	return subs[i].p;
}

void Union(Subset subs[], int u, int v) {
	int u_root = find(subs, u);
    	int v_root = find(subs, v);
    	if(u_root == v_root) return;

    	if (subs[u_root].rank < subs[v_root].rank)	subs[u_root].p = v_root;
    	else if (subs[u_root].rank > subs[v_root].rank) 	subs[v_root].p = u_root;
    	else {
		subs[v_root].p = u_root;
        	subs[u_root].rank++;
    	}
}


void krusk(Graph* graf) {
    	Subset subs[graf->n+1];
	int tree_size = graf->n - 1;
    	Edge result[tree_size];
    	float rtr = 0;

    	for(int i = 1; i <= graf->n; i++) {
        	subs[i].p = i;
        	subs[i].rank = 0;
    	}

    	PriorityQueue* kolejka = queue_make();
    	for(int i = 0; i < graf->m; i++)	queue_insert(i, graf->edges[i]->w, kolejka);

    	int s = 0; 
    	while(s < tree_size) {
        	int e =queue_pop(kolejka);

        	int x = find(subs, graf->edges[e]->u);
        	int y = find(subs, graf->edges[e]->v);

        	if(x != y) {
            		result[s++] = *(graf->edges[e]);
            		Union(subs, x, y);
        	}
    	}

    	for(int i = 0; i < s; i++) {
        	rtr += result[i].w;
        	if(result[i].u < result[i].v)	printf("%d %d %f\n",result[i].u,result[i].v, result[i].w);
        	else				printf("%d %d %f\n",result[i].v,result[i].u, result[i].w);
    	}
     	printf("%f\n",rtr);
}


int main(int argc, char *argv[]){
	if (argc <= 1)	{printf("\nPodano zle dane. \n Usage: %s <-p / -k>\n\n",argv[0]); return 0;}
    	srand (time(NULL));
    	int n, m,wybor;
    	scanf("%d",&n);
    	scanf("%d",&m);
    	if(!strcmp(argv[1],"-k"))	wybor=1;
    	else	if(!strcmp(argv[1],"-p"))	wybor=0;
	else {printf("zle dane"); return 0;}
//tworzenie grafu
	Graph* graf = malloc(sizeof(Graph));
	graf->n = n;
	graf->m = m;
	if(wybor){
        	graf->edges = malloc(sizeof(Edge*) * graf->m);
        	graf->neighbors = NULL;
    	}
    	else {
		graf->neighbors = malloc(sizeof(Neighbors*) * (n+1));
        	for(int i=0; i<n+1; i++)
            	graf->neighbors[i]=neighbors_make_for_p(graf->n);
        	graf->edges = NULL;
    	}
	//dodawanie krawedzi
		for(int i=0; i < (graf->m); i++){
			int u,v;
			float w;
			scanf("%d %d %f",&u, &v, &w);
			if(wybor)	graf->edges[i] = edge_make_for_kru(u,v,w);
			else
			{
		    		neighbors_insert(v, w, graf->neighbors[u]);
		    		neighbors_insert(u, w, graf->neighbors[v]);
			}
	    	}
	//...
//...
	if(wybor) 	krusk(graf);
	else	{int los = rand() % n + 1;prima(los,graf);}

	return 0;
}
