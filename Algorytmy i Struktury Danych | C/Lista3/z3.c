#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <sys/sysinfo.h>

/////////////////////////////////////////////////////////////////////////
/////////////////////////////SORTUJE TABICE//////////////////////////////
/////////////////////////////////////////////////////////////////////////

/******************************************** QUICK *******************/
int* mergeSort(int* array, int start, int end, int* compares, int* moves, bool comp);
int* merge(int* A, int* B, int lengthA, int lengthB, int* compares, int* moves, bool comp);
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


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////PROGRAM WLASCIWY//////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////


int binarySearch(int start, int end, int v, int* a, int* compares, int* rekur) {  
	if (start > end) return 0;     //jesli koniec i nie znalazl		
	int middle = (start + end)/ 2;     //dziel na pol
	(*compares)++;
	if (a[middle] == v)    return 1;        //sprawdzam czy to, to	
	(*compares)++;
	(*rekur)++;
	if(v < a[middle]) return binarySearch(start, middle-1, v, a, compares, rekur);//rekurencja
	else return binarySearch(middle+1, end, v, a, compares,rekur);
}

int main(){
	//if (argc < 2){
	//	printf("Usage: ./main <table size(n)> <lost element(v)>\n");
	//	return 0;
	//}
	int n, v, compares=0, result,par1,par2,rekur,b;
	float times;
	printf("n= ");
    	scanf("%d",&n);
	printf("Wpisz posortowane elementy tablicy\n");
    	int* a = malloc(sizeof(int)*n);
 
	for (int i=0; i<n; i++)
		scanf("%d", &a[i]);
    	clock_t start, end;
	printf("podaj v: ");
    	scanf("%d",&v);
	start = clock();
   	result = binarySearch(0, n-1, v, a, &compares, &rekur);
   	end = clock();
   	times = (float) (end-start);
	
	printf("-----------\nn = %d\nv = %d\nResult: %d\nTime: %f \ncompares: %d\n",n,v, result, times/1000.0, compares);
    return 0;
}
//**************************************
//***DLA ZUZYCH LOSOWYCH DANYCH NIZEJ***
//**************************************

	/*struct sysinfo sys_infotmp;
 	if(sysinfo(&sys_infotmp) != 0) perror("sysinfo");


	FILE *file = fopen("results.txt","w");
	int n, v, compares, result,par1,par2,rekur;
	int* a;
	float times;
	for(int n=1000;n<=100000;n+=1000)
	{
		times=rekur=compares=result=0;
		a = malloc(sizeof(int)*n);		
		int* a = malloc(sizeof(int)*n);
		srand(time(NULL));
		for(int i=0; i<n; i++) {a[i] = rand()/10000; /*printf("%d aa ",a[i]);*///}/*
		/*int v=rand()/10000;
		a = mergeSort(a, 0, n-1, &par1, &par2, 0);

		clock_t start, end;
			
   		start = clock();
   		 result = binarySearch(0, n-1, v, a, &compares, &rekur);
   		 end = clock();
   		 times = (float) (end-start);

    		fprintf(file,"%d;",n);
		fprintf(file,"%d;",compares);
		fprintf(file,"%d;\n",rekur);

    		printf("-----------\nn = %d\nv = %d\nResult: %d\nTime: %f \ncompares: %d\n",n,v, result, times/1000.0, compares);

	}
	printf("\nWszystkie dane zostaly zapisane w pliku Results\n");

	fclose (file);*/



