#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>


//pom
void printToConsole(int array[], int p, int k);
void swap(int* a, int* b, int* moves);
bool compare(int a, int b, int* compares);
void copyArray(int src[], int dest[], int n);
int partition(int arr[], int l, int h, int* compares, int* moves);
//rand
int RandomizedPartition(int a[], int l, int h, int* compares, int* moves);
int RandomizedSelect(int a[], int p, int q, int i, int* compares, int* moves);
//select
bool greater(int a, int b, int* compares);
int partitionver2(int arr[], int l, int h, int* compares, int* moves);
int pivot(int a[], int l, int h, int* compares, int* moves);
int preselectedPivotPartition(int a[], int l, int h, int pivotvalue, int* compares, int* moves);
int Select(int a[], int l, int h, int k, int* compares, int* moves);
//other
void generateRandomArray(int array[], int n);
void permute(int array[], int n);
void print(int array[], int n, int num);


/****************************************/
/**********SELECT ***********************/
/****************************************/
bool greater(int a, int b, int* compares) {
	fprintf(stderr, "compared %d and %d\n", a, b);
	return a > b;
}

int partitionver2(int array[], int l, int h, int* compares, int* moves) {         
	if(h == l)	return l;
	bool iskey;
	int key;
    
	for(int i = l+1; i <= h; i++) {                     //insertSort
        iskey = false;
        
        int j = i-1;
        while(j >= l && greater(array[j], key, compares)) {
            if(!iskey){	
                key = array[i];
                iskey = true;
            }
            array[j+1] = array[j];
            j--;
        }
        if(iskey) {
            array[j+1] = key;
        }
    }
    return l + (h-l)/2;             //srodkowy element
}

//wyznaczanie pivota
int pivot(int a[], int l, int h, int* compares, int* moves) {
	if(h - l < 5) {
		partitionver2(a, l, h, compares, moves);
		return a[l - (l-h)/2];
	}

	int n = h-l+1;
    	int n1 = (int) (n/5);
    	int medianArray[n1]; 
    
	for(int i = 0; i < n1; i++) {       //dzieli tablice
		int left = 5*i;
		int subRight = left + 4;
		if(subRight > h)	 subRight = h;
        int median5 = partitionver2(a, l+left, l+subRight, compares, moves); 
        medianArray[i] = a[median5];    
    }
    return pivot(medianArray, 0, n1-1, compares, moves);     
}

int preselectedPivotPartition(int a[], int l, int h, int pivotvalue, int* compares, int* moves) {    
	int j;
    for(j = l; j <= h; j++) { 
        if(a[j] == pivotvalue)
            break;
    }
    swap(&a[j], &a[l], moves);
    return partition(a, l, h, compares, moves);
}

int Select(int a[], int l, int h, int k, int* compares, int* moves) {       //SELECT
    fprintf(stderr, "Select\n");
    printToConsole(a, l, h);
    fprintf(stderr, "k = %d\n", k);
    
    if(l == h)	 return a[l];
    
    int pivotvalue = pivot(a, l, h, compares, moves);  
    int pivotIndex = preselectedPivotPartition(a, l, h, pivotvalue, compares, moves);       //robimy wedle niego partitiona

    int i = pivotIndex-l+1;
    if(k == i)	 return pivotvalue;
    else if(k < i)	return Select(a, l, pivotIndex-1, k, compares, moves);      
    else	return Select(a, pivotIndex+1, h, k-i, compares, moves);
}

/****************************************/
/**********RANDOMIZED SELECT*************/
/****************************************/

int partition(int array[], int l, int h, int* compares, int* moves) {       //zwykly partition, ktory dzieli tablice wzgledem lewego indeksu
	printf("pivot = %d\n", array[l]);
	int i = l;
	for(int j = l + 1; j <= h; j++) {
        	if(compare(array[j], array[l], compares)) {
			i++;
			swap(&array[i], &array[j], moves);
        	}
    	}
    	swap(&array[i], &array[l], moves);
    	return i;
}

int RandomizedPartition(int a[], int l, int h, int* compares, int* moves) {         //losuje i zmieni z lewym i podziel
	int j = rand() % (h - l + 1);
	swap(&a[l + j], &a[l], moves);
	return partition(a, l, h, compares, moves);
}

//random select
int RandomizedSelect(int a[], int p, int q, int i, int* compares, int* moves) {
	fprintf(stderr, "RandomizedSelect\n");
	printToConsole(a, p, q);
	fprintf(stderr, "k = %d\n", i);
    
	if(p == q)	return a[p];
    
	int ra = RandomizedPartition(a, p, q, compares, moves);                
	int k = ra - p + 1;
    
	if(i == k)	 return a[ra];
	else if(i < k)	return RandomizedSelect(a, p, ra-1, i, compares, moves);             
	else	return RandomizedSelect(a, ra+1, q, i-k, compares, moves);
}

/****************************************/
/************OTHER***********************/
/****************************************/
void generateRandomArray(int array[], int n) {
	srand(time(NULL));
	for(int i = 0; i < n; i++){
       		array[i] = rand()/100000;
	}
}

void permute(int array[], int n) {
    for(int i = 0; i < n; i++) {
        array[i] = i + 1;
    }
    for(int i = n - 1; i > 0; i--) {
        int j = rand() % (i+1);
        int tmp = array[j];
        array[j] = array[i];
        array[i] = tmp;
    }
}

void printToConsole(int array[], int p, int k) {//wypisywanie
    for(int i = p; i <= k; i++) {
        fprintf(stderr, "%d ", array[i]);
    }
    fprintf(stderr, "\n");
}

void swap(int* a, int* b, int* moves) {
    fprintf(stderr, "swapp %d & %d\n", *a, *b);
    (*moves)++;
    int t = *a;
	*a = *b;
	*b = t;
}

bool compare(int a, int b, int* compares) {
	(*compares)++;
	fprintf(stderr, "compare %d & %d\n", a, b);
	return a <= b;
}

void copyArray(int src[], int dest[], int n) {
	for(int i = 0; i < n; i++) dest[i] = src[i];
}
/****************************************/
/*****************MAIN ******************/
/****************************************/
int main(int argc, char *argv[]) {
	int type, n, k,  randSelMoves, selCompares, selmoves, ransselcompares;
	type=2;
	randSelMoves = 0;
	selCompares = 0;
	selmoves = 0;
	ransselcompares = 0;
	if (argc > 1){
 		if (!strcmp(argv[1], "-r"))	type = 1;
        	else	 type = 2;
	}
 
	scanf("%d", &n);
	int* array = malloc(sizeof(int)*n);
	int* s = malloc(sizeof(int)*n);
	scanf("%d", &k);

	if(type == 1)	generateRandomArray(array, n);
	else	permute(array, n);

	copyArray(array, s, n);
	int selvalue = Select(s, 0, n-1, k, &selCompares, &selmoves);       
	fprintf(stderr, "---------\nSelect finished\n---------\nCompares = %d\nmoves = %d\n", selCompares, selmoves);

	int* rs = malloc(sizeof(int)*n);
	copyArray(array, rs, n);
	int randsvalue = RandomizedSelect(rs, 0, n-1, k, &ransselcompares, &randSelMoves);
	fprintf(stderr, "---------\nRandomizedSelect finished\n---------\nCompares = %d\nmoves = %d\n", ransselcompares, randSelMoves);

	if(selvalue != randsvalue) {
		fprintf(stderr, "\nResults are different\n");
		exit(-1);
   	 }
	else	 fprintf(stderr, "\nResults are the same\n");

	fprintf(stderr, "---\nSELECT \n---\nMOVES: %d COMPARES: %d\n", selmoves, selCompares);
	fprintf(stderr, "---\nRANDOM SELECT\n---\n MOVES: %d COMPARES:%d\n\n", randSelMoves, ransselcompares);

	for(int i = 0; i < n; i++) {
		if(rs[i] == randsvalue)	printf("[%d] ", rs[i]);
        	else	 printf("%d ", rs[i]);
    	}
	printf("\n");
/*int n=10;
int* array = malloc(sizeof(int)*n);
permute(array, n);
//generateRandomArray(array, n);
for (int i=0;i<=n;i++){
//array[i]== (rand() % (n+1));
printf("%d\n",array[i]);
}	printf("%daaa",i);*/
	
    return 0;
}

