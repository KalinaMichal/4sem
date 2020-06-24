//gcc -Wall -std=c99 zad1.c -lm

#include <stdio.h>
#include <math.h>
#include <string.h>

int main(int argc, char** argv){
    if (argc < 2)
        return 1;
    
	FILE* f = fopen(argv[1], "r");	// r - otwiera plik do czytania
    
	long int countAll = 0;
	int count1[256];
	int count2[256][256];

//**Zerowanie tablicy**//
    for(int i=0; i<256; i++){
        count1[i]=0;
        for(int j=0; j<256; j++)
            count2[i][j]=0;
    }
    
    int c=0;
    int prev = 0;

//**licze se tutaj wszystkie znaki(tzn. ich wystapien liczbe)**//
    while ((c = fgetc(f)) != EOF) {	//czyta znak ze strumienia np: 'A'=65 w txt
        countAll++;
        count1[c]++;
        count2[prev][c]++;		//licze wystapienia po danym znaku
        prev=c;
    }
    
    fclose(f);

//**Liczenie entr itd.****************//
    double entropia = 0, entropia2 = 0;	//1-jest pojedynczych, 2-jest podwojnych
    for (int i=0; i<256; i++){
        if(count1[i]>0){//jesli nie pusty
            double prawdopodobienstwo  =  (double) count1[i] / (double) countAll;	//P(x)
            entropia += prawdopodobienstwo*log2(prawdopodobienstwo);
            
            double h = 0;
            for(int j=0; j<256; j++){
                if(count2[i][j] > 0){
                    double probabilityTmp =  (double) count2[i][j] / (double) count1[i];	//P(y|x)
                    h += probabilityTmp*log2(probabilityTmp);//H(Y|x)=P(y|x)*I(y|x)
                }
            }

            entropia2 += prawdopodobienstwo * (-1.0) * h;
        }
    }
    if(entropia != 0)
        entropia *= -1;

    printf("entopia = %f\nentropia warunkowa = %f\n", entropia,entropia2);

/**WYSWIETLANIE NA PLIKU**/
    if (argc > 2 && !strcmp(argv[2], "-save")){
        f = fopen("wyniki", "w");
        fprintf(f,"Dane dla %s: \n\nLiczba znakow: %lu\n\nIlosc pojedynczych znakow:\n",argv[1],countAll);
        for (int i=0; i< 256; i++)
            fprintf(f, "%d (%c) = %d razy = %0.5f = %0.3f%%\n", i, i, count1[i], (double) count1[i] / (double) countAll, 100.0* (double) count1[i] / (double) countAll);

        fprintf(f,"\nWystapienia par znakow:\n");
        for(int i=0; i<256;i++)
            for(int j=0; j<256;j++)
                fprintf(f,"%d (%c) przed znakiem %d (%c) = %d razy = %0.5f = %0.3f%%\n",i, i, j, j, count2[i][j], (double) count2[i][j] / (double) countAll, 100.0* (double) count2[i][j] / (double) countAll);
        fclose(f);
	printf("Zapisano do pliku wyniki\n");
    }
/**WYSWIETLANIE NA EKANIE**/
    if (argc > 2 && !strcmp(argv[2], "-show")){
        printf("Dane dla %s: \n\nIlosc znakow: %lu\n\nLiczba pojedynczych znakow:\n",argv[1],countAll);
        for (int i=0; i< 256; i++)
		{
			if(count1[i]!=0)
            fprintf(f, "%d (%c) = %d razy = %0.5f = %0.3f%%\n", i, i, count1[i], (double) count1[i] / (double) countAll, 100.0* (double) count1[i] / (double) countAll);
		}

        printf("\nWystapienia par znakow:\n");
        for(int i=0; i<256;i++)
            for(int j=0; j<256;j++)
		{
		if(count2[i][j]!=0)
                printf("%d (%c) przed znakiem %d (%c) = %d razy = %0.5f = %0.3f%%\n",i, i, j, j, count2[i][j], (double) count2[i][j] / (double) countAll, 100.0* (double) count2[i][j] / (double) countAll);
		}
    }
    return 0;
}
