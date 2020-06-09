#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>


/*************************/
/*Algorytmy sortujace*****/
/*************************/



void insertionSort(int* array, int n, int* compares, int* moves, bool comp);

int* mergeSort(int* array, int start, int end, int* compares, int* moves, bool comp);
int* merge(int* A, int* B, int lengthA, int lengthB, int* compares, int* moves, bool comp);

void quickSort(int* array, int start, int end, int* compares, int* moves, bool comp);
int partition(int* array, int start, int end, int* compares, int* moves, bool comp);

void swap(int* a, int* b);

/******************************************** INSERTION *************************************************************/

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

/******************************************** MERGE *************************************************************/
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

/******************************************** QUICK *************************************************************/
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

/******************************************** OTHERS *************************************************************/

void swap(int* a, int* b){		
    int temp = *a;
    *a = *b;
    *b = temp;
}

/******************************************** MAIN *************************************************************/
int main(int argc, char *argv[]){

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
int* numbers;
double times, avgmoves, avgtime, avgcompares;

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
		else if(!strcmp((argv[0]),"--stat")){
			argc--;
			argv++;
			plikN=argv[0];
			argc--;
			argv++;
			k = strtol(argv[0], NULL, 10);
		}
		else {argc--; argv++;	}
	}
		
/*************************/
/*wprowadzanie danych*****/
/*************************/

/*******plik******/
	FILE *file = fopen(plikN,"w");
	if (file == NULL){
		printf("error, can't open %s. Write correct file name\n",plikN);
		return 0;
	}

/**********************/
/*poczatek duzej petli*/
/**********************/
for(int n=100;n<=10000;n+=100)
{
	for(int j=0; j<k; j++){	
		moves=compares=0;
		numbers = malloc(sizeof(int)*n);	
		int* numbers = malloc(sizeof(int)*n);
		srand(time(NULL));
		for(int i=0; i<n; i++) numbers[i] = rand();

	/*************************/
	/*Wykonywanie obliczen****/
	/*************************/
	
		clock_t start = clock();  //start
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
		}
		clock_t end = clock();   //meta
		times = (double) ((end-start) / 1000.0);
			
		avgtime += (double) times/k;
		avgcompares += (double) (1.0*compares/k);
		avgmoves += (double) (1.0*moves/k);
		free(numbers);
	}
	/********************************/
	/*Wyswietlanie wyniku na pliku***/
	/********************************/
		
	fprintf(file,"%d;",n);
	fprintf(file,"%d;",compares);
	fprintf(file,"%d;",moves);
	fprintf(file,"%f;\n",times);
	avgmoves=avgtime=avgcompares=moves=compares=0;
}

fclose (file);
return 0;
}
