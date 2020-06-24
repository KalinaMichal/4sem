#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//#include <unistd.h>
//#include <limits.h>
//#include <time.h>

#define MAX(x,y) (((x)>(y))?(x):(y))
#define MIN(x,y) (((x)<(y))?(x):(y))

//slown
const char kolor[3][6]={"RED  ","GREEN","BLUE "};	//spacje - dla ladniejszego wyswietlania
const char metoda[9][11]= {"Clear     ","W         ","N         ","NW        ","W+N-NW    ","W+(N-NW)/2","N+(W-NW)/2","(W+N)/2   ","LOCO-I    "};

//*******************************************************************************************************************************
//***** 						Entriopie       						*********    
//*******************************************************************************************************************************

//entriopia z pop. programow
double entropy(int* count, int all){
	double rtrn = 0;//return
    
	for (int i=0; i<256; i++){
		if(count[i]>0){
            		double probability  =  (double) count[i] / (double) all;
            		rtrn += probability*log2(probability);                                                           
        	}
    	}
	
	if(rtrn == 0.0) return  rtrn;
	else return (-1.0)*rtrn;
}

//wkladam do entropy() jedynie bajty danego koloru
double color_prediction_entropy(int pred, int color, int wysokosc, int szerokosc,unsigned char**** img ){//zmiana
	int all=0;
	int count[256]={0};
    
	for(int w=1; w<=wysokosc; w++){
		for(int s=1; s<=szerokosc; s++){
			all++;
			count[img[w][s][pred][color]]++;
	        }
	}
    
	return entropy(count, all);
}

//wkladam do entropy() wszystkie bajty obrazu po zastosowaniu podanej metody predykcji
double prediction_entropy(int pred, int wysokosc, int szerokosc, unsigned char**** img){//zmiana
	int all=0;
    	int count[256]={0};
    
    	for(int w=1; w<=wysokosc; w++){
        	for(int s=1; s<=szerokosc; s++){
        	    for(int i=0; i<3; i++){
        	        all++;
        	        count[img[w][s][pred][i]]++;
        	    }
        	}
    	}	    
	return entropy(count, all);
}

//*******************************************************************************************************************************
//***** 						 MAIN								*********    
//*******************************************************************************************************************************

int main(int argc, char** argv){

//**********************************************
//**** 	  pobieranie i deklaracja danych    ****        
//**********************************************

	if (argc!=2) {printf("\n\t Usage: ./main <ImageFile>\n\n"); return 0;}

	FILE* file = fopen(argv[1], "r");
	if(!file) {printf("\n\tFile error\n\n"); return 0;}
    
	unsigned char header[18];                       //naglowek 18 bajtow
    	fread(header,1,18,file);                       	
    	int szerokosc = header[12]+256*header[13];      //szerokosc
    	int wysokosc = header[14]+256*header[15];       //wysokosc
    	unsigned char**** tab = malloc(sizeof(unsigned char***)*(wysokosc+1));//high, wide, prediction metod, color

//*********************************
//*****  operacje na pliku    *****        
//*********************************

    	for(int i=0; i<wysokosc+1; i++){  //kol. i wiersz 0 to 'tlo' wiec zwiekszam szer. i wysok. o 1
        	tab[i]=malloc(sizeof(unsigned char**)*(szerokosc+1));
        	for(int j=0; j<szerokosc+1; j++){
			tab[i][j]=malloc(sizeof(unsigned char*)*9);
            		for(int k=0; k<9; k++)	tab[i][j][k] = calloc(3, sizeof(unsigned char));
       		}
    	}       
    
	for(int w=1; w<=wysokosc; w++){
        	for(int s=1; s<=szerokosc; s++){
            		fread(&tab[w][s][0][0],3,1,file);      //wczytywanie piksela z plika

			for(int i=0; i<3; i++){
                		int X = tab[w][s][0][i];          //kolory obrazu 
               			int W = tab[w][s-1][0][i];        //obecny, lewy, gorny i gorno-lewy
                		int N = tab[w-1][s][0][i];      
				int NW= tab[w-1][s-1][0][i];
			//rzutowanie na char'a
			                          
                		tab[w][s][1][i]=(unsigned char) (X-W); 
                		tab[w][s][2][i]=(unsigned char) (X-N);        //roznica miedzy kolorem wyliczonym a oryginalnym
                		tab[w][s][3][i]=(unsigned char) (X-NW);
                		tab[w][s][4][i]=(unsigned char) (X-(W+N-NW));       
                		tab[w][s][5][i]=(unsigned char) (X-(W+(N-NW)/2));                                         
                		tab[w][s][6][i]=(unsigned char) (X-(N+(W-NW)/2));       
				tab[w][s][7][i]=(unsigned char) (X-(W+N)/2);                                          

				if(NW>=MAX(W,N))		tab[w][s][8][i]=(unsigned char)(X-MIN(W,N));
				else{
					if(NW <= MIN(W,N))	tab[w][s][8][i]=(unsigned char)(X-MAX(W,N));
					else			tab[w][s][8][i]=(unsigned char)(X-(N+W-NW));
				}    
            		}           
		}
	}
    
	fclose(file);
    
//*********************************
//**** 	  zamkniecie pliku     ****        
//*********************************

//minimalna entriopia wedlgug wszystkich metod
	double minEntr=9.0;
    	int min=9; 
	printf("\nminimal entropy\n------------------------------------------------------------\n");
    	for(int i=0; i<9; i++){
        	double tmp=prediction_entropy(i,wysokosc,szerokosc,tab);
        	if (tmp < minEntr){
			minEntr=tmp;
			min=i;
        	}
        	printf("Prediction method: %s  |  Entropy: %f\n", metoda[i], tmp);
    	}
	printf("------------------------------------------------------------\n\n");

//minimalna entriopia koloru dla kazdej metody
	double minEntr2[3]={9.0, 9.0, 9.0};     
	int min2[3]={9,9,9};
	printf("\nminimal entropy for every color\n------------------------------------------------------------\n");
	for(int i=0; i<9; i++){
        	for(int j=0; j<3; j++){
            		double tmp=color_prediction_entropy(i,j,wysokosc,szerokosc,tab);
            		if (tmp < minEntr2[j]){
                		minEntr2[j]=tmp;
                		min2[j]=i;
            		}
            		printf("Prediction method: %s  |  Color: %s  |  Entropy: %f\n",metoda[i], kolor[j], tmp);
        	}
		printf("------------------------------------------------------------\n");
    	}

    

//Wyswietlanie optymalnej metody
	printf("\nThe optimal method for the whole image: %s (entropy: %f)\n\n",metoda[min],minEntr);
    
	for(int i=0; i<3; i++)	printf("The optimal method for %s : %s| Entropy: %f\n",kolor[i],metoda[min2[i]],minEntr2[i]);
	
	printf("\n");
	return 0;
}
