#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>
#include <time.h>
#include <sys/sysinfo.h>

#define MODULO 128
#define N 1

/**********************************************************************************************************************/
/******************************************** Struktura dzrewa *************************************************************/
/**********************************************************************************************************************/
enum treeType  { BST, RBT, HMAP}; 
typedef enum Color { BLACK, RED} Color;

//list
typedef struct ListNode {
	char* value;
	struct ListNode* next;
} ListNode;

typedef struct List{
	int size;
	ListNode* head;
} List;


//bst
typedef struct bstNode{
	char* value;
	struct bstNode* p;
	struct bstNode* right;
	struct bstNode* left;
} bstNode;

typedef struct bstTree{
	int size;
	bstNode* root;
} bstTree;

//rbt
typedef struct rbtNode{
	Color color;
	char* value;
	struct rbtNode* p;
	struct rbtNode* right;
	struct rbtNode* left;
} rbtNode;

typedef struct rbtTree{
	int size;
	rbtNode* root;
	rbtNode* nil;
} rbtTree;

//hash
union hashData{
	List* list;
 	rbtTree* tree;
};

typedef struct hashListElement{
	int size;
	union hashData structure;
} hashListElement;


typedef struct hashMap{
  	int modulo;
	int size;
	int switchSize;
	hashListElement** data;
} hashMap;

//tree
union treeData{
	hashMap* hmap;
	bstTree* bst;
  	rbtTree* rbt;
};

typedef struct Tree{
	enum treeType type;
	union treeData data;
} Tree;



/**********************************************************************************************************************/

bool compare(char* a, char* b);
void rbt_insert_fix(rbtTree* t, rbtNode* z);
void rbt_delete_fix(rbtTree* t, rbtNode* z);
char* checkcopy(char* key);

/**********************************************************************************************************************/
/******************************************** LISTA *************************************************************/
/**********************************************************************************************************************/

void makeList(List* list){
	list -> head = NULL;
	list -> size = 0;
}

void list_add(List* list, char* val) {
	ListNode* tmp = list->head;
	ListNode* prev = NULL;
	while (tmp && compare(val, tmp->value)){
		prev = tmp;
		tmp = tmp-> next;
	}
	ListNode* new = malloc(sizeof(ListNode));
	new -> value = checkcopy(val);
	new -> next = tmp;
	if(prev)	prev->next = new;
	else	list->head=new;
	(list->size)++;
}
 
char* list_pop_midval(List *list){
	int i = 0;
	int mid = (list->size/2);

    
	ListNode* tmp = list->head;
	ListNode* prev = NULL;
	while (tmp && i < mid){
		prev = tmp;
		tmp = tmp-> next;
		i++;
	}
    
	if (!tmp)	return NULL;
	char* value = checkcopy(tmp->value);
    
	if(prev)	prev->next = tmp->next;
	else	list->head=tmp->next;

	free(tmp); 
	(list->size)--;
	return value;
}

/*
void list_rysuj(List* lista){
	ListNode* tmp = lista->head;
	printf("lista = -> ");
	while (tmp){
		printf("(%s, %p) -> ",tmp->value, tmp->next);
		tmp = tmp -> next;
	}
	printf(" NULL\n");
}*/


int list_delete(List* list, char* val){
	ListNode* tmp = list->head;
	ListNode* prev = NULL;
	while (tmp && compare(tmp->value,val)){
		prev = tmp;
		tmp = tmp-> next;
	}
    
	if (!tmp || strcmp(tmp->value, val)>0)	return 0;
    
	if(prev)	prev->next = tmp->next;
	else		list->head=tmp->next;
	free(tmp); 
	(list->size)--;
	return 1;
}


 
void list_find(List* list, char* val){
	ListNode* tmp = list->head;
	while (tmp && compare(tmp->value,val)){
		tmp = tmp-> next;
	}
	if (!tmp || strcmp(tmp->value, val)>0)	printf("0\n");
	else	printf("1\n");
}	

/**********************************************************************************************************************/
/********************************************** BST *******************************************************************/
/**********************************************************************************************************************/


bstNode* searchBST(bstTree* t, char* value) {
	bstNode* tmp = t->root;
	while(tmp && strcmp(value,tmp->value)) {
		if(compare(value, tmp->value))	tmp = tmp -> left;
		else				tmp = tmp -> right;
	}
	return tmp;
} 
 
void bst_find(bstTree* t, char* value){
	if(searchBST(t,value))	printf("1\n");
	else			printf("0\n");
}


bstNode* minimumBST(bstNode* n) {
	if(!n)	return NULL;
	else {
		while(n->left)	n = n->left;
        	return n;
    	}
}

void bst_min(bstTree* t){
	bstNode* tmp = minimumBST(t->root);
	if(tmp)	printf("%s\n",tmp->value);
	else	printf("\n");
}

bstNode* maximumBST(bstNode* node) {
	if(!node)	return NULL;
	else {	
		while(node->right)	node = node->right;
        	return node;
	}
}

void bst_max(bstTree* t){//gotowe
	bstNode* tmp = maximumBST(t->root);
	if(tmp)		printf("%s\n",tmp->value);
    	else		printf("\n");
}

bstNode* bst_succesor(bstNode* x) {//gotowe
	if(x->right)	return minimumBST(x->right);
	bstNode* y=x->p;
	while(y && x == y->right) {
        	x = y;
        	y = y->p;
    	}
	return y;
}

void bst_succesor_get(bstTree* t, char* value){
	bstNode* tmp_ser = searchBST(t, value);
	if(tmp_ser){
		bstNode* tmp_suc = bst_succesor(tmp_ser);
		if(tmp_suc){
			printf("%s\n",tmp_suc->value);
		return;
		}
	}
	printf("\n");
}

void bst_trans(bstTree* t, bstNode* x, bstNode* y) {
	if(!(x->p))			t->root = y;
	else 	if(x == x->p->left)	x->p->left = y;
		else 			x->p->right = y;
	if(y)				y->p = x->p;
}
 
void bst_insert(bstTree* t, char* key){ 
    bstNode* new = malloc(sizeof(bstNode));
    new->value = checkcopy(key);
    new->left = new->right = new->p = NULL;
    
    if(!(t->root)){
        t->root = new;
    } 
    else {
        bstNode* tmp = t->root;
        bstNode* prev;
        while(tmp){
            prev = tmp;
            if(compare(new->value,tmp->value))
                tmp = tmp -> left;
            else
                tmp = tmp -> right;
        }
        new->p = prev;
        if(compare(new->value, prev->value))
            prev -> left = new;
        else 
            prev -> right = new;
    }
    (t->size)++;
}
    
void bst_delete(bstTree* t, char* key){ 
	bstNode* found = searchBST(t, key);
	if(found) {
		if(!(found->left))	bst_trans(t, found, found->right);
		else if(!(found->right))bst_trans(t, found, found->left);
       		 else {
            		bstNode* minRight = minimumBST(found->right);
            		if(minRight->p != found) {
                	bst_trans(t, minRight, minRight->right);
                	minRight->right = found->right;
                	minRight->right->p = found;
            		}
            		bst_trans(t, found, minRight);
            		minRight->left = found->left;
            		minRight->left->p = minRight;
        	}
        	free(found);
        	(t->size)--;
    	}
}    

void bst_load(bstTree* t, char* file_name){
	FILE *file;
	if((file = fopen(file_name, "r")) == NULL)	printf("file ERROR: ( %s ) \n",file_name);
    	else {
        	char element[151];
        	while(fscanf(file,"%150s",element) != EOF)	bst_insert(t, element);
	}
}

void bst_inorder_by_node(bstNode* node){
	if(node){
		bst_inorder_by_node(node->left);
		printf("%s ",node->value);
		bst_inorder_by_node(node->right);
	}
}
 
void bst_inorder(bstTree* t){
	bst_inorder_by_node(t->root);
	printf("\n");
}

void bst_delete_tree_bynode(bstNode* node){
	if(node){
		bst_delete_tree_bynode(node->left);
		bst_delete_tree_bynode(node->right);
		free(node);
	}
}

//destroy
void bst_destroy(bstTree* t){
	bstNode* node = t->root;
	bst_delete_tree_bynode(node);
}   


/**********************************************************************************************************************/
/********************************************** RBT *******************************************************************/
/**********************************************************************************************************************/

rbtNode* searchRBT(rbtTree* t, char* value) {
	rbtNode* tmp = t->root;
	while(tmp != t->nil && strcmp(value,tmp->value)) {
		if(compare(value, tmp->value))	tmp = tmp -> left;
	else	tmp = tmp -> right;
	}
	return tmp;
} 
 
void rbt_find(rbtTree* t, char* value){
	if(searchRBT(t,value) != t->nil)	printf("1\n");
	else	printf("0\n");
}


rbtNode* minimumRBT(rbtNode* nil, rbtNode* node) {
	if(node == nil)	return nil;
	else {	
		while(node->left != nil)	node = node->left;
		return node;
	}
}

void rbt_min(rbtTree* t){
	rbtNode* tmp = minimumRBT(t->nil, t->root);
	if(tmp != t-> nil)	printf("%s\n",tmp->value);
	else	printf("\n");
}

rbtNode* maximumRBT(rbtNode* nil, rbtNode* node) {
	if(node == nil)	return nil;
	else {
		while(node->right != nil)	node = node->right;
        return node;
	}
}

void rbt_max(rbtTree* t){
	rbtNode* tmp = maximumRBT(t->nil, t->root);
	if(tmp != t->nil)	printf("%s\n",tmp->value);
	else	printf("\n");
}

rbtNode* rbt_successor(rbtNode* nil, rbtNode* x) {
	if(x->right != nil)	return minimumRBT(nil, x->right);
	rbtNode* y=x->p;
	while(y != nil && x == y->right) {
		x = y;
		y = y->p;
	}
	return y;
}

void rbt_successor_get(rbtTree* t, char* value){
	rbtNode* tmp1 = searchRBT(t, value);
	if(tmp1 != t-> nil){
		rbtNode* tmp2 = rbt_successor(t->nil, tmp1);
		if(tmp2 != t->nil){
			printf("%s\n",tmp2->value);
			return;
		}
	}
	printf("\n");
}



void rbt_inorder_by_node(rbtNode* nil, rbtNode* n){
	if(n != nil){
		rbt_inorder_by_node(nil, n->left);
		printf("%s ",n->value);
		rbt_inorder_by_node(nil, n->right);
	}
}
 
void rbt_inorder(rbtTree* t){
	rbt_inorder_by_node(t->nil, t->root);
	printf("\n");
}

void reverseInorderToArrayByNodeRBT(rbtNode* nil,rbtNode* n, char** array){
	if(n != nil){
		int i = 0;
		reverseInorderToArrayByNodeRBT(nil, n->right,array);
		while(array[i++]);
		array[i-1] = checkcopy(n->value);
		reverseInorderToArrayByNodeRBT(nil, n->left,array);
	}
}

void reverseInorderToArrayRBT(rbtTree* t, char** array){
	reverseInorderToArrayByNodeRBT(t->nil,t->root, array);
}

void deleteTreeByNodeRBT(rbtNode* nil, rbtNode* n){
	if(n != nil)
	{
		deleteTreeByNodeRBT(nil, n->left);
		deleteTreeByNodeRBT(nil, n->right);
		free(n);
	}
}

void rbt_destroy(rbtTree *t){
	rbtNode* n = t->root;
	deleteTreeByNodeRBT(t->nil, n);
	free(t->nil);
}  

void rbt_left(rbtTree* t, rbtNode* x) {
	rbtNode* y = x->right;
	x->right = y->left;
    	if(y->left != t->nil)	y->left->p = x;
	y->p = x->p;
	if(x->p == t->nil)	t->root = y;
	else if(x == x->p->left)x->p->left = y;
	else 			x->p->right = y;
	y->left = x;
	x->p = y;
}

void rbt_right(rbtTree* t, rbtNode* x) {
	rbtNode* y = x->left;
	x->left = y->right;
	if(y->right != t->nil)	y->right->p = x;
	y->p = x->p;
	if(x->p == t->nil)	t->root = y;
	else if(x == x->p->left)x->p->left = y;
	else 			x->p->right = y;
	y->right = x;
	x->p = y;
}


void rbt_insert(rbtTree* t, char* key){ 
	rbtNode* new = malloc(sizeof(rbtNode));    
	new->value = checkcopy(key);    
	new->color = RED;
	new->left = new->right = new->p = t->nil;
	if(t->root == t->nil)	t->root = new;
	else {
		rbtNode* tmp = t->root;
		rbtNode* prev = t->nil;
        	while(tmp != t->nil){
            		prev = tmp;
            		if(compare(new->value,tmp->value))	tmp = tmp -> left;
			else					tmp = tmp -> right;
        	}
        	new->p = prev;
        	if(compare(new->value, prev->value))		prev -> left = new;
        	else 						prev -> right = new;
   	 }
    	rbt_insert_fix(t, new);
    	(t->size)++;
}

void rbt_insert_fix(rbtTree* t, rbtNode* z) {
	while(z != t->root && z->p->color == RED) {
		if(z->p == z->p->p->left) {
			rbtNode* y = z->p->p->right;
			if(y->color == RED) {
				z->p->color = BLACK;
				y->color = BLACK;
				z->p->p->color = RED;
				z = z->p->p;
			} else {
				if(z == z->p->right) {
				z = z->p;
				rbt_left(t, z);
			}
			z->p->color = BLACK;
			z->p->p->color = RED;
            		rbt_right(t, z->p->p);
		}
		} else {
			rbtNode* y = z->p->p->left;
			if(y->color == RED) {
				z->p->color = BLACK;
				y->color = BLACK;
                		z->p->p->color = RED;
                		z = z->p->p;
            	} else {
                	if(z == z->p->left) {
                    		z = z->p;
                    		rbt_right(t, z);
                	}
                	z->p->color = BLACK;
                	z->p->p->color = RED;
                	rbt_left(t, z->p->p);
            		}
        	}
    	}
    	t->root->color = BLACK;
}

void rbt_trans(rbtTree* t, rbtNode* a, rbtNode* b){
	if(a->p == t->nil)	t->root = b;
	else if(a == a->p->left)a->p->left = b;
	else 	a->p->right = b;
	b->p = a->p;
}

//delete
int rbt_delete(rbtTree* t, char* key){ 
	rbtNode* found = searchRBT(t, key);
	if(found != t->nil) {
        	rbtNode* changed;
        	Color foundColor = found->color;
        	if(found->left == t->nil){
        	    changed = found->right;
        	    rbt_trans(t, found, found->right);
        	}
        	else if(found->right == t->nil){
        	    changed = found->left;
        	    rbt_trans(t, found, found->left);
        	}
        	else {
        	    rbtNode* minRight = minimumRBT(t->nil, found->right);
        	    foundColor = minRight->color;
        	    changed = minRight->right;
        	    if(minRight->p != found) {
        	        rbt_trans(t, minRight, minRight->right);
        	        minRight->right = found->right;
        	        minRight->right->p = minRight;
        	    }
        	    else	changed->p = minRight;
        	    rbt_trans(t, found, minRight);
        	    minRight->left = found->left;
            	    minRight->left->p = minRight;
            	    minRight->color = found->color;
        	}
        	if(foundColor == BLACK)
            	rbt_delete_fix(t, changed);
        	free(found);
        	(t->size)--;
        	return 1;
    	}
    	return 0;
}    

//delete ale inny
void rbt_delete_fix(rbtTree* t, rbtNode* x) {
    while(x != t->root && x->color == BLACK) {
        if(x == x->p->left) {
            rbtNode* w = x->p->right;
            if(w->color == RED) {
                w->color = BLACK;
                x->p->color = RED;
                rbt_left(t, x->p);
                w = x->p->right;
            }
            if(w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->p;
            } else {
                if(w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rbt_right(t, w);
                    w = x->p->right;
                }
                w->color = x->p->color;
                x->p->color = BLACK;
                w->right->color = BLACK;
                rbt_left(t, x->p);
                x = t->root;
            }
        } else {
            rbtNode* w = x->p->left;
            if(w->color == RED) {
                w->color = BLACK;
                x->p->color = RED;
                rbt_right(t, x->p);
                w = x->p->left;
            }
            if(w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->p;
            } else {
                if(w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    rbt_left(t, w);
                    w = x->p->left;
                }
                w->color = x->p->color;
                x->p->color = BLACK;
                w->left->color = BLACK;
                rbt_right(t, x->p);
                x = t->root;
            }
        }
    }
    x->color = BLACK;
}

//load
void rbt_load(rbtTree* t, char* filename){
	FILE *file;
	if((file = fopen(filename, "r")) == NULL)	printf("file ERROR: ( %s ) \n",filename);
  	 	 else {
        		char element[151];
        		while(fscanf(file,"%150s",element) != EOF){
            		rbt_insert(t, element);
		}
	}
}

/**********************************************************************************************************************/
/********************************************** HMAP *******************************************************************/
/**********************************************************************************************************************/

int hash(char* key, int modulo){
	size_t len =strlen(key);
	uint32_t hash, i;

	for(hash = i = 0; i < len; ++i)
	{
		hash += key[i];
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}
	hash += (hash << 3);	//bit shiffing/problem
	hash ^= (hash >> 11);
	hash += (hash << 15);
	return hash%modulo;
}

//insert
void hmap_insert(hashMap* m, char* key2){
	char* key = checkcopy(key2);
	int index_hash = hash(key, m->modulo);
	if (m->data[index_hash]->size < m->switchSize){
		list_add(m->data[index_hash]->structure.list, key);
		(m->data[index_hash]->size)++;
		(m->size)++;
		if(m->data[index_hash]->size == m->switchSize)
		{
		//***********
		//LIST to RBT
		//***********
			hashListElement* e=m->data[index_hash];
			rbtTree* tree_tmp = malloc(sizeof(rbtTree));
			//***
			//create RBT
			//***
			tree_tmp->size=0;   
			tree_tmp->nil = malloc(sizeof(rbtNode));
			tree_tmp->nil->color = BLACK;
			tree_tmp->nil->p = tree_tmp->nil->right = tree_tmp->nil->left =  NULL;
			tree_tmp->nil->value = NULL;
			tree_tmp->root = tree_tmp->nil;   
			//***
			char* key_tmp = list_pop_midval(e->structure.list);
			while(key_tmp){
				rbt_insert(tree_tmp, key_tmp);        
				free(key_tmp);
      				key_tmp = list_pop_midval(e->structure.list);
    			}
			free(e->structure.list);
			e->structure.tree = tree_tmp;
		}
	}
	else{
		rbt_insert(m->data[index_hash]->structure.tree, key);
		(m->data[index_hash]->size)++;
		(m->size)++;
	}
	free(key);
}

//delete
void hmap_delete(hashMap* m, char* key2){
	char* key = checkcopy(key2);
	int index_hash = hash(key, m->modulo);
	if (m->data[index_hash]->size < m->switchSize){
		if(list_delete(m->data[index_hash]->structure.list, key)){
			(m->data[index_hash]->size)--;
			(m->size)--;
		}
	}
	else{
		if(rbt_delete(m->data[index_hash]->structure.tree, key)){
			(m->data[index_hash]->size)--;
			(m->size)--;
			if(m->data[index_hash]->size < m->switchSize)
			{
			//***********
			//RBT TO LIST
			//***********
				hashListElement* e=m->data[index_hash];
				List* list = malloc(sizeof(List));
				makeList(list);
				char** key_tmp = malloc(sizeof(char*)*(e->size));
				for(int i=0; i< e->size; i++)	key_tmp[i]=NULL;
				reverseInorderToArrayRBT(e->structure.tree, key_tmp);
				for(int i=0; i<e->size; i++)	list_add(list, key_tmp[i]);   
				rbt_destroy(e->structure.tree);
				free(e->structure.tree);
				e->structure.list = list;
			}	
		}
	}    
	free(key);
}

//find
void hmap_find(hashMap* m, char* key2){
	char* key = checkcopy(key2);
	int index_hash = hash(key, m->modulo);
	if (m->data[index_hash]->size < m->switchSize)	list_find(m->data[index_hash]->structure.list, key);
	else	rbt_find(m->data[index_hash]->structure.tree, key);
    
}

//destroy
void hmap_destroy(hashMap* m){
	for(int i=0; i< m->modulo; i++){
		if (m->data[i]->size < m->switchSize)	(m->data[i]->structure.list);
		else	rbt_destroy(m->data[i]->structure.tree);
		free(m->data[i]);
	}
	free(m->data);    
}

//load
void hmap_load(hashMap* m, char* filename){
	FILE *file;
	if((file = fopen(filename, "r")) == NULL){
		printf("file ERROR: ( %s ) \n",filename);
	}
	else {
		char element[151];
		while(fscanf(file,"%150s",element) != EOF)	hmap_insert(m, element);
    	}
}

/**********************************************************************************************************************/
/*********************************************** HELPING ONES *********************************************************/
/**********************************************************************************************************************/

bool compare(char* x, char* y){
	return (strcmp(x,y) <=0);
}

char* checkcopy(char* key){
	char* tmp = malloc(sizeof(char)*(strlen(key)+1));
	strcpy(tmp,key);
	char* p=tmp;
	while(strlen(p) > 0 && !( (p[0] >= 'A' && p[0] <= 'Z') || (p[0] >= 'a' && p[0] <= 'z'))) p++;
	int s = strlen(p)-1;
	while(strlen(p) > 0 && !( (p[s] >= 'A' && p[s] <= 'Z') || (p[s] >= 'a' && p[s] <= 'z'))){
		p[s] = '\0';
		s--;
	}
    
	char* result = malloc(sizeof(char)*(strlen(p)+1));
    	strcpy(result,p);
    	free(tmp);
    	return result;
}

//*****************
//zniszczenie drzew
//*****************

void destroyTree(Tree* tree){
	switch(tree->type){
		case BST:
			bst_destroy(tree->data.bst);
			free(tree->data.bst);
		break;
		case RBT:
			rbt_destroy(tree->data.rbt);
			free(tree->data.rbt);
		break;
		case HMAP:
			hmap_destroy(tree->data.hmap);
			free(tree->data.hmap);
		break;
	}
}

/*********************************************************************************************************************/
/********************************************** MAIN *****************************************************************/
/*********************************************************************************************************************/

int main (int argc, char* argv[]){
	int n;

//sprawdzanie i pobieranie danych
    	if(argc != 3) {
		printf("Zle dane\n");
		return 0;
	}
	printf("Podaj ilosc komend: ");
	scanf("%d",&n);
	Tree* tree = malloc(sizeof(Tree));

//*****************************
//tworzenie drzewa wdlg. wyboru
//*****************************
	if(!strcmp((argv[1]),"--type")){
	//BST		
	        if(!strcmp(argv[2],"bst")){
			tree->type = BST;
			tree->data.bst = malloc(sizeof(bstTree));
			tree->data.bst->root = NULL;
			tree->data.bst->size=0;
        	}
	//RBT
        	else if(!strcmp(argv[2],"rbt")){
			tree->type = RBT;
			tree->data.rbt = malloc(sizeof(rbtTree));
			tree->data.rbt->size=0;   
			tree->data.rbt->nil = malloc(sizeof(rbtNode));
			tree->data.rbt->nil->color = BLACK;
			tree->data.rbt->nil->p = tree->data.rbt->nil->right = tree->data.rbt->nil->left =  NULL;
			tree->data.rbt->nil->value = NULL;
	    		tree->data.rbt->root = tree->data.rbt->nil;
        	}
	//HMAP
        	else if(!strcmp(argv[2],"hmap")){
			int modulo=MODULO;
			tree->type = HMAP;
    			tree->data.hmap = malloc(sizeof(hashMap));
    			tree->data.hmap->modulo=modulo;   
    			tree->data.hmap->switchSize = N;
    			tree->data.hmap->data = malloc(sizeof(hashListElement*)*modulo);
    			for(int i =0; i<modulo; i++){
    				tree->data.hmap->data[i] = malloc(sizeof(hashListElement));
        			tree->data.hmap->data[i]->size = 0;
        			tree->data.hmap->data[i]->structure.list = malloc(sizeof(List));
        			makeList(tree->data.hmap->data[i]->structure.list); 
    			}
        	}
    	}

//****************************************
// Robie rzeczy podane przez uzytkownika *
//****************************************

	int maxsize=0, successors=0, inserts=0, finds=0, deletes=0, mins=0, loads=0, maxes=0, inorders=0;
	char instruction[11],element[151];
    	clock_t start, end;
    	start = clock();
	for(int i=0; i<n; i++){
		scanf("%10s",instruction);			//skanuje 1 czesc 
		//te z 1 czescia
        		if(!strcmp(instruction,"max")){
				switch(tree->type){
                			case BST:
                    				bst_max(tree->data.bst);
                    			break;
                			case RBT:
                    				rbt_max(tree->data.rbt);
                    			break;
                			case HMAP:
                    			printf("\n");
                    			break;
            			}
            			maxes++;
        		}
			else if(!strcmp(instruction,"min")){
           			switch(tree->type){
                			case BST:
                    				bst_min(tree->data.bst);
                    			break;
                			case RBT:
                    				rbt_min(tree->data.rbt);
                    			break;
                			case HMAP:
                    				printf("\n");
                    			break;
            			}	
            			mins++;
			}
			else if(!strcmp(instruction,"inorder")){
				switch(tree->type){
                		case BST:
					bst_inorder(tree->data.bst);
				break;
				case RBT:
					rbt_inorder(tree->data.rbt);
				break;
				case HMAP:
					printf("\n");
				break;
				}
				inorders++;
			}
		//te z 2 czescia
			else if(!strcmp(instruction,"insert")){
				scanf("%150s",element);		//skanuje 2 czesc 
				switch(tree->type){
					case BST:
						bst_insert(tree->data.bst,element);
						if(tree->data.bst->size > maxsize)	maxsize = tree->data.bst->size;
					break;
					case RBT:
						rbt_insert(tree->data.rbt,element);
						if(tree->data.rbt->size > maxsize)	maxsize = tree->data.rbt->size;
    	                		break;
					case HMAP:
						hmap_insert(tree->data.hmap,element);
						if(tree->data.hmap->size > maxsize)	maxsize = tree->data.hmap->size;
    	                		break;
    	        		}
				inserts++;
			}
			else if(!strcmp(instruction,"delete")){
    	        		scanf("%150s",element);
				switch(tree->type){
					case BST:
						bst_delete(tree->data.bst,element);
						if(tree->data.bst->size > maxsize)	maxsize = tree->data.bst->size;
					break;
               				case RBT:
                    				rbt_delete(tree->data.rbt,element);
                    				if(tree->data.rbt->size > maxsize)	maxsize = tree->data.rbt->size;
                    			break;
                			case HMAP:
						hmap_delete(tree->data.hmap,element);
                    				if(tree->data.hmap->size > maxsize)	maxsize = tree->data.hmap->size;
                   			break;
            			}
            			deletes++;
        		}
			else if(!strcmp(instruction,"find")){
				scanf("%150s",element);
				switch(tree->type){
					case BST:
						bst_find(tree->data.bst,element);
					break;
					case RBT:
						rbt_find(tree->data.rbt,element);
					break;
					case HMAP:
						hmap_find(tree->data.hmap,element);
					break;
            			}
            			finds++;
			}
			else if(!strcmp(instruction,"load")){
				scanf("%150s",element);
				switch(tree->type){
    	            			case BST:
    	                			bst_load(tree->data.bst,element);
    	                			if(tree->data.bst->size > maxsize)	maxsize = tree->data.bst->size;
					break;
					case RBT:
						rbt_load(tree->data.rbt,element);
						if(tree->data.rbt->size > maxsize)	maxsize = tree->data.rbt->size;
    	                		break;
					case HMAP:
						hmap_load(tree->data.hmap,element);
						if(tree->data.hmap->size > maxsize)	maxsize = tree->data.hmap->size;
    	                		break;
    	        		}
    	        		loads++;
			}
        		else if(!strcmp(instruction,"successor")){
            			scanf("%150s",element);
            			switch(tree->type){
                		case BST:
					bst_succesor_get(tree->data.bst,element);
				break;
				case RBT:
					rbt_successor_get(tree->data.rbt,element);
				break;
				case HMAP:
					printf("\n");
				break;
				}
				successors++;
			}
		}
		end = clock();
		double times = (double) ((end-start) / 1000.0);
		int size=0;
		switch(tree->type){
			case BST:
				size = tree->data.bst->size;
			break;
			case RBT:
				size = tree->data.rbt->size;
			break;
				case HMAP:
			size = tree->data.hmap->size;
			break;
	
	}

	fprintf(stderr, "time:       %f\n",	times);
	fprintf(stderr, "inserts:    %d\n",	inserts);
	fprintf(stderr, "loads:      %d\n",	loads);
	fprintf(stderr, "deletes:    %d\n",	deletes);
	fprintf(stderr, "finds:      %d\n",	finds);
	fprintf(stderr, "min:        %d\n",	mins);
	fprintf(stderr, "max:        %d\n",	maxes);
	fprintf(stderr, "successors: %d\n",	successors);
	fprintf(stderr, "inorders:   %d\n",	inorders);
	fprintf(stderr, "max size:   %d\n",	maxsize);
	fprintf(stderr, "end size:   %d\n",	size);

//*********************
// Konczenie programu *
//*********************

	destroyTree(tree);
	free(tree);

	return 0;
}
