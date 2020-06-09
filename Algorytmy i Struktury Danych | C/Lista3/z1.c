#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <sys/sysinfo.h>


/*************************/
/*Algorytmy sortujace*****/
/*************************/

int gmax(int array[], int size);
void countSort(int array[], int base, int exp, int size, bool comp, int* moves);
void radixSort(int array[], int size, int* compares, int* moves, bool comp);

void insertionSort(int* array, int n, int* compares, int* moves, bool comp);

int* mergeSort(int* array, int start, int end, int* compares, int* moves, bool comp);
int* merge(int* A, int* B, int lengthA, int lengthB, int* compares, int* moves, bool comp);

void quickSort(int* array, int start, int end, int* compares, int* moves, bool comp);
int partition(int* array, int start, int end, int* compares, int* moves, bool comp);

void dualquickSort(int* array, int start, int end, int* compares, int* moves, bool comp);
int dualpartition(int* array, int start, int end, int* compares, int* moves, bool comp,int* lp);

void swap(int* a, int* b);
/****************************/
/********* INSERTION *******/
/****************************/
void insertionSort(int* array, int n, int* compares, int* moves, bool comp){
	int key=array[0];		//elemrent porownywany
	int i;			//pomocnicza (taki iterator)

	for(int j = 1; j < n; j++){//zaczynam od 1, bo 0 nie ma z czym porownywac
		key = array[j]; (*moves)++;		//mam nowy klucz
		//fprintf(stderr, "Insertion sort: key=%d\n",key);
		i = j-1;
		if(!comp){//rosnaco
			while (i >= 0 && array[i] > key) 
        		{  
				//fprintf(stderr, "Insertion sort: comparing element array[%d] (%d) with key(%d)\n",i,array[i+1],key);
            			array[i + 1] = array[i];  
				//fprintf(stderr, "Insertion sort: array[%d] (%d) = key (%d)\n",i+1,array[i+1],key);
           	 		i--;  
				(*compares)++;
        		}  
        		array[i + 1] = key;  
		}
		if(comp){//malejaco
			while (i >= 0 && array[i] < key) 
        		{  
				//fprintf(stderr, "Insertion sort: comparing element array[%d] (%d) with key(%d)\n",i,array[i+1],key);
            			array[i + 1] = array[i];  
				//fprintf(stderr, "Insertion sort: array[%d] (%d) = key (%d)\n",i+1,array[i+1],key);
           	 		i--;  
				(*compares)++;
        		}  
        		array[i+1] = key;  
		}
	}
}
/****************************/
/********* MERGE *******/
/****************************/

bool mergeSortCompare(int a, int b, bool comp){
	if(comp)		//niemalejaco
		return a>b;
	else			//nierosnaco
		return b>a;
}

int* mergeSort(int* array, int start, int end, int* compares, int* moves, bool comp){
	if (start == end){					//jeden element
		int* r = malloc(sizeof(int));	
		r[0] = array[start];			
		return r;						//zwraca tablice z nim samym
	}
	else{
        int middle = start+(end-start)/2;		//polowa z (start,end)
		
        int* A = mergeSort(array, start, middle, compares, moves, comp);		//merge do polowy
        int* B = mergeSort(array, middle+1, end, compares, moves, comp);		//merge od polowy
 
        return merge(A, B, middle - start +1, end - middle, compares, moves, comp);		//zwroc scalona tablice
    }
}

int* merge(int* A, int* B, int lengthA, int lengthB, int* compares, int* moves, bool comp){
	int* results;
	results = malloc(sizeof(int)*(lengthB + lengthA));
	
	int a=0, b=0,i=0;
	while(a < lengthA && b < lengthB){		//dopoki w obu tablicach sa elementy
		if(mergeSortCompare(A[a], B[b], comp) && ++(*compares)){//porownaje elementy A i B i zwiekszam compares
			results[i++] = A[a++]; (*moves)++;		//wloz element tablicy A zwieksz 'i' & 'a' do results
		}
		else{
			results[i++] = B[b++]; (*moves)++;		//wloz element tablicy B zwieksz 'i' & 'b' do results
		}
	}
	while(a < lengthA){		//wkladam A do wyniku
		results[i++] = A[a++]; 
	}
	while(b < lengthB){			//wkladanie B do wyniku
		results[i++] = B[b++]; 
	}
	free(B);
	free(A);		//zwalnianie tablic, ktore scalalismy
	return results;
}

/****************************/
/********* QUICK *******/
/****************************/

bool quickSortCompare(int a, int b, bool comp){
	//fprintf(stderr, "Quick sort: comparing %d with %d\n",a,b);
	if(!comp)		//niemalejaco
		return a<=b;
	else		//nierosnaco
		return b<=a;
}

void quickSort(int* array, int start, int end, int* compares, int* moves, bool comp){
	if (start < end){
        	int middle = partition(array, start, end, compares, moves, comp);  //dziele na pol
 	//reukrencja 2 polow tabliczy
        	quickSort(array, start, middle, compares, moves, comp);			
		quickSort(array, middle + 1, end, compares, moves, comp);
	}	
}

int partition(int* array, int start, int end, int* compares, int* moves, bool comp){
	int x = array[start];   //wyznacznikiem jest pierwszy element
    	int i = start;
	int j=end;
	while (true) 		//petla nieskonczona wyjscie przez return j
	{
		if(!comp) while (array[j] > x) { j--; (*compares)++; /*fprintf(stderr, "Quick sort: comparing %d with %d\n",array[j],x);*/}//cisne prawa str tablicy
		else while (array[j] < x) { j--; (*compares)++;/*fprintf(stderr, "Quick sort: comparing %d with %d\n",array[j],x); */}
		if(!comp) while (array[i] < x) {i++; (*compares)++;/*fprintf(stderr, "Quick sort: comparing %d with %d\n",array[i],x);*/ }	//cisne lewa str tablicy
		else while (array[i] > x) {i++; (*compares)++;/*fprintf(stderr, "Quick sort: comparing %d with %d\n",array[i],x);*/ }
		if (i < j) 			//'swap' miejsc gdy gdy i < j
		{
			if(array[i]!=array[j])
			{
				swap(&array[i],&array[j]);
				//fprintf(stderr, "Quick sort: swap %d with %d\n",array[i],array[j]); 
				(*moves)++;
			}
			i++;
			j--;
		}
		else return j;			//gdy i>=j zwracam j jako punkt podzialu tablicy	
	}
}
/**********************************/
/********* DUAL PIVOT QUICK *******/
/**********************************/
bool dualquickSortCompare(int a, int b, bool comp){
	//fprintf(stderr, "Dual pivot Quick sort: comparing %d with %d\n",a,b);
	if(comp)		//niemalejaco
		return a<=b;
	else		//nierosnaco
		return b<=a;

}

void dualquickSort(int* array, int start, int end, int* compares, int* moves, bool comp){
	if (start < end){
		int lp, rp;   // lp: left pivot | rp: right pivot. 
        	rp = dualpartition(array, start, end, compares, moves, comp, &lp); 
        	dualquickSort(array, start, lp - 1, compares, moves, comp);
        	dualquickSort(array, lp + 1, rp - 1, compares, moves, comp);
        	dualquickSort(array, rp + 1, end, compares, moves, comp);
	}	
}
int dualpartition(int* arr, int low, int high, int* compares, int* moves, bool comp, int* lp)
{ 
	if (mergeSortCompare(arr[low], arr[high], !comp) && ++(*compares)){//comp
		swap(&arr[low], &arr[high]); 
	}
    // p: left pivot | q: right pivot. 
    int j = low + 1; 
    int g = high - 1, k = low + 1, p = arr[low], q = arr[high]; 
    while (k <= g) { 
//zlatwiam lewego
	(*moves)++;//<
	if (mergeSortCompare(arr[k], p, comp) && ++(*compares)) { //jesli elem. jest mniejszy(w sensie sortowania) od lewego piwota//comp
		swap(&arr[k], &arr[j]);
		(*moves)++;
		j++; 
        } 
//zalatwiam prawego 
        else if (mergeSortCompare(arr[k], q, !comp)  && ++(*compares)) {//jesli elem. jest wiekszy/rowny(w sensie sortowania)  od prawego pivota//comp
            	while (mergeSortCompare(arr[g], q, !comp) && ++(*compares) && k < g && ++(*compares)){
			g--;
		} 
            	swap(&arr[k], &arr[g]); 
		(*moves)++;
            	g--; 
            	if (mergeSortCompare(arr[k], p, comp) && ++(*compares)) { 
                	swap(&arr[k], &arr[j]); 
			(*moves)++;
                	j++; 
            	} 
        	} 
        	k++; 
    	} 
    	j--; 
    	g++; 
  
	// bring pivots to their appropriate positions. 
	swap(&arr[low], &arr[j]); 
	swap(&arr[high], &arr[g]);
	(*moves)++;
	(*moves)++; 
  
    // returning the indices of the pivots. 
    *lp = j; // because we cannot return two elements  
             // from a function. 
  
    return g; 
} 

/****************************/
/********* RADIX *******/
/****************************/

void countSort(int array[], int base, int exp, int size, bool comp, int* moves) {
	*moves = 3 * size + base;
	int* count = calloc(base, sizeof(int));
	int* result = malloc(sizeof(int)*size);
    
	for(int i = 0; i < size; i++)	count[(array[i]/exp)%base]++; 
    
	if (!comp) 	for(int i = 1; i < base; i++)		count[i] += count[i - 1];           
    	else		for (int i = base - 2; i >= 0; i--)	count[i] += count[i + 1];

	for(int i= size-1; i >=0; i--)	result[--count[(array[i]/exp)%base]] = array[i];   
	for(int i=0; i<size; i++)	array[i] = result[i];
	
	free(result);
	free(count);
}

void radixSort(int array[], int size, int* compares, int* moves, bool comp) {
	*compares = size - 1;
	int max = gmax(array, size);
	int base = 256; 
    
	for(unsigned long int exp = 1; max/exp > 0; exp *= base) { 
		countSort(array, base, exp, size, comp, moves);
	}
	printf("\nPosortowane liczby:\n");
    	for(int i = 0; i < size; i++)	printf("=>: %d\n",array[i]);
}


/******************************************** OTHERS *************************************************************/

int gmax(int array[], int size) {     //maksimum z tablicy
	int max = array[0];
	for(int i = 1; i < size; i++) if(array[i] > max)  max = array[i];
	return max;
}

void swap(int* a, int* b){		
    int temp = *a;
    *a = *b;
    *b = temp;
}

int max(int array[], int size) {
    int max = array[0];
    for(int i = 1; i < size; i++)
	{
		printf("porownuje array[%d] = %d z max= %d\n",i, array[i],max);
        	if(array[i] > max) max = array[i];
	}    
	return max;
}
/******************************************** MAIN *************************************************************/
int main(int argc, char *argv[]){
	if (argc < 2){
		printf("Usage: ./main --type <merge/insert/quick/radix> --comp <'>='/'<='> --stat <filename> <k>\n");
		return 0;
	}

/*************************/
/*Deklaracja danych*******/
/*************************/
_Bool random= false;
int type =0;
_Bool down= true;
int compares=0;
int moves=0;
char* plikN="NULL";
int k;
double times, avgmoves, avgtime, avgcompares;
unsigned long long avgmemory=0;

/************************************/
/*Deklaracja war pocz sys info*******/
/************************************/
struct sysinfo sys_infotmp;
if(sysinfo(&sys_infotmp) != 0) perror("sysinfo");
//unsigned long long pamstart=  (sys_infotmp.totalram *(unsigned long long)sys_infotmp.mem_unit/ 1024)-(sys_infotmp.freeram *(unsigned long long)sys_infotmp.mem_unit/ 1024);

//te 2 sa po to aby nie porownywac w while'u ./a.out niepotrzebnie
argc--;
argv++;

/*************************/
/*wprowadzanie parametrow*/
/*************************/
	while(argc > 0){
		if(!strcmp((argv[0]),"--type")){	
			argc--;
			argv++;	
			if(argc > 0){
				if(!strcmp((argv[0]),"merge")){//merge
					type=1;
				}
				else if(!strcmp((argv[0]),"insert")){//insert
					type=2;
				}
				else if(!strcmp((argv[0]),"quick")){//quick
					type=3;
				}
				else if(!strcmp((argv[0]),"dualquick")){//dualquick
					type=4;
				}
                		else if(!strcmp(argv[0],"radix"))
					type=5;

				argc--;
				argv++;
			}
		}

		else if(!strcmp((argv[0]),"--comp")){
			argc--;
			argv++;
			if(!strcmp((argv[0]),">=")){//rosnaco
				down= false;
			}
			else if(!strcmp((argv[0]),"<=")){//malejaco
				down= true;
			}
			argc--;
			argv++;
		}
		else if(!strcmp((argv[0]),"--rand")){
			argc--;
			argv++;
			random=true;
		}
		else {argc--; argv++;	}
	}
		
/*************************/
/*wprowadzanie danych*****/
/*************************/



/**********************/
/*poczatek duzej petli*/
/**********************/
_Bool multfive= true;
int n=10,decmult=10,fivemult=5;
		printf("\nPodaj ilosc liczb (n): ");
	scanf("%d",&n);
		//struct sysinfo sys_info;
 		//if(sysinfo(&sys_info) != 0) perror("sysinfo");
		moves=compares=0;	
		int* numbers = malloc(sizeof(int)*n);
		srand(time(NULL));
		if(!random){
			printf("Podaj liczby:\n");
			for(int i=0; i<n; i++) scanf("%d",&numbers[i]);
		}
		if(random){
			printf("wylosowane liczby:\n") ;
			for(int i=0; i<n; i++) {numbers[i] = rand(); printf("   %d\n",numbers[i]);}
		}
	/*************************/
	/*Wykonywanie obliczen****/
	/*************************/
		//zegar i sysinfo

		clock_t start = clock();  //start
		//............
		switch (type) {
			case 1:
				numbers = mergeSort(numbers, 0, n-1, &compares, &moves, down);
				break;
			case 2:
				insertionSort(numbers, n, &compares, &moves, down);
				break;
			case 3:
				quickSort(numbers, 0, n-1, &compares, &moves, down);
				break;
			case 4:
				dualquickSort(numbers, 0, n-1, &compares, &moves, down);
				break;
               		case 5:
				radixSort(numbers, n, &compares, &moves, down);
				break;  
		}
		//zegar i sysinfo
		//unsigned long long pam=  (sys_info.totalram *(unsigned long long)sys_info.mem_unit/ 1024)-(sys_info.freeram *(unsigned long long)sys_info.mem_unit/ 1024)-pamstart;
		clock_t end = clock();   //meta
		times = (double) ((end-start) / 1000.0);
		//............
		//avgmemory += pam/k;
		avgtime += (double) times;
		avgcompares += (double) (1.0*compares);
		avgmoves += (double) (1.0*moves);
		free(numbers);
	
/********************************/
/*****Wyswietlanie na ekranie****/
/********************************/
	printf("\nn: %d ",n);
	printf("compares: %f ",avgcompares);
	printf("moves: %f ",avgmoves);
	printf("time: %f\n",avgtime);

	avgmoves=avgtime=avgcompares=moves=compares=avgmemory=0;
	if(multfive){
		fivemult*=10;
		n=fivemult;
		multfive=false;	
	}
	else{
		decmult*=10;
		n=decmult;
		multfive=true;				
	}



return 0;
}
