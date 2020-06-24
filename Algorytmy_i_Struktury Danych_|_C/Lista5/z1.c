#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>

/*********************************************************************************************************************/
/********************************************* KOLEJKA ***************************************************************/
/*********************************************************************************************************************/

//----
typedef struct element{
	int liczba;
	int priorytet;
} PriorityQueueElement;

typedef struct kolejka{
	PriorityQueueElement** e;
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

//----
List* list_make(void){
	List* new = malloc(sizeof(List));
	new->head = new->body = NULL;
	return new;
}

PriorityQueue* queue_make(void){
	PriorityQueue* pq = malloc(sizeof(PriorityQueue));
	pq->size=0;
	pq->size_max=128;
	pq->e = calloc(pq->size_max, sizeof(PriorityQueueElement*));
	return pq;
}

//----
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

//----
int right(int i)	{return 2*i+2;}
int left(int i)		{return 2*i+1;}
int parent(int i) 	{return (i-1)/2;}
int queue_empty(PriorityQueue* q){if(q->size) return 0; else return 1;}
int queue_top(PriorityQueue* q){if(q->size) return q->e[0]->liczba; else return INT_MIN;}
void queue_print(PriorityQueue* pq){for(int i=0; i < pq->size; i++)printf("(%d, %d) ",pq->e[i]->liczba,pq->e[i]->priorytet);}

void queue_swap(PriorityQueueElement* a, PriorityQueueElement* b){
	PriorityQueueElement tmp = *a;
	*a = *b;
    	*b = tmp;
}

void shift_up(int x, PriorityQueue* q){
	while(x > 0 && q->e[parent(x)]->priorytet > q->e[x]->priorytet){
		queue_swap(q->e[x], q->e[parent(x)]);
		x = parent(x);
	}
}

int queue_insert(int liczba, int p, PriorityQueue* q){
	q->e[q->size] = malloc(sizeof(PriorityQueueElement));
	q->e[q->size]->liczba = liczba;
	q->e[q->size]->priorytet = p;
	int tmp = q->size++;
	shift_up(tmp, q);
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

/*********************************************************************************************************************/
/********************************************** MAIN *****************************************************************/
/*********************************************************************************************************************/

int main(void){
	PriorityQueue* queue = queue_make();
	int n;
	scanf("%d",&n);

	for(int i = 0; i < n; i++){
		char command[20];
		scanf("%s",command);
		printf("---| %s |---\n",command);
		if(!strcmp(command,"insert")){
			int liczba, priorytet;
			scanf("%d",&liczba);
			scanf("%d",&priorytet);
		    	(!queue_insert(liczba, priorytet, queue));
		} 
		else if(!strcmp(command,"empty"))	printf("%d\n",queue_empty(queue));
		else if(!strcmp(command,"print")) 	{queue_print(queue);printf("\n");}

		else if(!strcmp(command,"top")){
			int top=queue_top(queue);
		    	if(top != INT_MIN)	printf("%d\n",top);
		    	else	printf("\n");
		} 
		else if(!strcmp(command,"pop")){
		    	int pop=queue_pop(queue);
		    	if(pop != INT_MIN)	printf("%d\n",pop);
		    	else	printf("\n");
		} 
		else if(!strcmp(command,"priority")) {
			int liczba, priorytet;
		    	scanf("%d",&liczba);
		    	scanf("%d",&priorytet);
		    	queue_priority(liczba, priorytet, queue);
		}
		else printf("Zla komenda\n");
		
	}
	return 0;
}
