#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
#include <string.h>
#include <math.h>
//#include <limits.h>
//#include <time.h>

unsigned char naglowek[18]; //naglowek 18 bajtow
double	mse_best_all;			//blad sredniokwadratowy calego obrazu      
double	snr_best_all;			//stosunek sygnal/szumu 

/****************************************************************************************************************************************/
/***************************************************	        Pomocnicze	*********************************************************/
/****************************************************************************************************************************************/

/**********************************************************/
/*  wyznacza tablice przypisujaca reprezentanta kolorowi  */
/**********************************************************/

void podzial(unsigned char* quant, int bits){
	int pow = 1 << bits;
	int* podz=malloc(sizeof(int)*(pow+2));
	podz[0]=0;//1 kominikat:0
    
	for(int i=0; i<pow; i++){//gen. przedzialow
		podz[i+1]=(i+1)*256/(pow);                                  //wartosc na koncu
		for(int j=podz[i]; j<podz[i+1]; j++)	quant[j]=(unsigned char) ((podz[i]+podz[i+1])/2);
	}     
	free(podz);
}

/*************/
/*  min/max  */
/*************/

double min_3(double a,double b, double c)
{
	double rtr;
	if(a<b)rtr=a;
	else rtr=b;
	if(rtr<c) return rtr;
	else	  return c;

}

double max_3(double a,double b, double c)
{
	double rtr;
	if(a>b)rtr=a;
	else rtr=b;
	if(rtr>c) return rtr;
	else	  return c;
}

/****************************************************************************************************************************************/
/******************************************************	            MAIN	*********************************************************/
/****************************************************************************************************************************************/

int main(int argc, char** argv){
	if (argc!=4) {
		printf("Usage:\n\t$ ./main <picture_file> <result_file> <ammount of bits per pixel>"); 
		return 0;
	}
	FILE* plik = fopen(argv[1], "r");
	if(!plik) {
		printf("File {%s} error\n", argv[1]); 
		return 0;
	}

	fread(&naglowek,1,18,plik);                      //wczytywanie 1*18 bajtow pod adres
	int suma=atoi(argv[3]);
	int mse_best_bits[3]={-1,-1,-1};	//najlepszy podzial dla min z najwiekszego bledu sredniokwadratowego               
	int snr_best_bits[3]={-1,-1,-1};  //najlepszy podzial dla max z najmniejszego stosunku sygnalu do szumu
	int wysokosc = naglowek[14]+256*naglowek[15];    //na tych bajtach jest wysokos zdjecia, licze ja tu
	int szerokosc = naglowek[12]+256*naglowek[13];   //a na tych szerokosc
	unsigned char kwanty[3][256];  	//taka jakby funkcja, co wdlg. podanej l. bitow na kolor podporzadkuje pod bajta (0-255) 
	double	best_snr_kolor[3]={0.0, 0.0, 0.0};                   	//SNR dla max SNR
	double	best_mse_kolor[3]={INFINITY, INFINITY, INFINITY};       //MSE dla min MSE
	unsigned char*** plan = malloc(sizeof(unsigned char**)*wysokosc);    //wysokosc, szerokosc i kolor
	unsigned char*** plan2 = malloc(sizeof(unsigned char**)*wysokosc); 
	for(int w=0; w<wysokosc; w++){                                            
		plan[w]=malloc(sizeof(unsigned char*)*szerokosc);  
		plan2[w]=malloc(sizeof(unsigned char*)*szerokosc); 
		for(int s=0; s<szerokosc; s++){
			plan[w][s]=malloc(sizeof(unsigned char)*3);  
			plan2[w][s]=malloc(sizeof(unsigned char)*3);  
		}
	}

	for(int w=0; w<wysokosc; w++)	for(int s=0; s<szerokosc; s++)	fread(&plan[w][s][0],3,1,plik);       //wczytywanie obrazu do pamieci

	for(int r=0; r<=suma; r++){
		for(int g=0; g<=suma-r; g++){
		    	double mse_temp_kolor[3], snr_temp_kolor[3];  //MSE i SNR
			long int mse_licz[3]={0, 0, 0}, snr_licz[3]={0, 0, 0}, mse_all_count=0, snr_all_count=0;    //liczniki dla MSE i SNR
			int b=suma-r-g;			//skracanie kolorow (czerwonego, zielonego, zoltego) do (r,g,b) bitow
            		podzial(kwanty[0], r);
			podzial(kwanty[1], g);
			podzial(kwanty[2], b);    
             
			for(int w=0; w<wysokosc; w++){
				for(int s=0; s<szerokosc; s++){
					for(int i=0; i<3; i++){
						plan2[w][s][i]=kwanty[i][plan[w][s][i]];
		                		int y=plan2[w][s][i];
						int x=plan[w][s][i];
						mse_all_count+=(x-y)*(x-y);          
						snr_all_count+=x*x;
						mse_licz[i]+=(x-y)*(x-y); 
						snr_licz[i]+=x*x; 
					}           
				}
			}   
		   
			for(int i=0; i<3; i++){
				mse_temp_kolor[i]=(double)mse_licz[i]/(double) (wysokosc*szerokosc);
				snr_temp_kolor[i]=(double)snr_licz[i]/(double) mse_licz[i];
			}


			if (max_3(mse_temp_kolor[0],mse_temp_kolor[1],mse_temp_kolor[2]) < max_3(best_mse_kolor[0],best_mse_kolor[1],best_mse_kolor[2])){//sprawdzam czy max blad sredniokwadratowy z kolorow co mam jest mniejszy niz maksymalny z dotychczasoch
				mse_best_bits[0]=r; mse_best_bits[1]=g; mse_best_bits[2]=b;           //zmieniam bity na kolory
		        
		        	for(int i=0; i<3; i++){//podmiana mse i snr
					best_mse_kolor[i]=mse_temp_kolor[i];
		       		}
		        	//tu dla calego pliku
				mse_best_all=(double) mse_all_count / (double) (3.0*wysokosc*szerokosc);
			}
		    

			if (min_3(snr_temp_kolor[0],snr_temp_kolor[1],snr_temp_kolor[2]) > min_3(best_snr_kolor[0],best_snr_kolor[1],best_snr_kolor[2])){//sprawdzam czy min stos. sygnal/szum jest wiekszy od minimalnego dotychczasowego
				snr_best_bits[0]=r; snr_best_bits[1]=g; snr_best_bits[2]=b;
		        
				for(int i=0; i<3; i++)	best_snr_kolor[i]=snr_temp_kolor[i];
		        	snr_best_all=(double) snr_all_count / (double) mse_all_count;  
			}
		}
	}
    
	unsigned char stopka[26];     
	fread(&stopka,1,26,plik);
	fclose(plik);

/****************************************************************************/
/*********************   Drukowanie wynikow	*****************************/
/****************************************************************************/

/*******/
/* MSE */
/*******/

	//MSE
   	for(int i=0; i<3; i++)	podzial(kwanty[i],mse_best_bits[i]);
	
	//gen pic
	for(int w=0; w<wysokosc; w++)	for(int s=0; s<szerokosc; s++)	for(int i=0; i<3; i++)	plan2[w][s][i]=kwanty[i][plan[w][s][i]];   
   	
    	char mse_file[strlen(argv[2])+6];
	sprintf(mse_file, "%s_mse", argv[2]);       

	//zapis
	plik = fopen(mse_file, "w");
	if(!plik) {
		printf("File {%s} error\n", argv[2]); 
		return 0;
	}
   
	fwrite(&naglowek,1,18,plik);     
	for(int w=0; w<wysokosc; w++)	for(int s=0; s<szerokosc; s++)	fwrite(&plan2[w][s][0],3,1,plik);     
        
	fwrite(&stopka,1,26,plik);
    
	fclose(plik);

/*******/
/* SNR */
/*******/

	//SNR
	for(int i=0; i<3; i++)	podzial(kwanty[i],snr_best_bits[i]);

	//generowanie obrazu
	for(int h=0; h<wysokosc; h++)	for(int w=0; w<szerokosc; w++)	for(int i=0; i<3; i++)	plan2[h][w][i]=kwanty[i][plan[h][w][i]];
                      
	char snr_file[strlen(argv[2])+6];
	sprintf(snr_file, "%s_snr", argv[2]);

	//zapis
	plik = fopen(snr_file, "w");
	if(!plik) {
		printf("File {%s} error\n", argv[2]); 
		return 0;
	}

	fwrite(&naglowek,1,18,plik);     
	for(int w=0; w<wysokosc; w++)	for(int s=0; s<szerokosc; s++)	fwrite(&plan2[w][s][0],3,1,plik);       

	fwrite(&stopka,1,26,plik);
	fclose(plik);


/****************/
/* WYSWIETLANIE */
/****************/

   	printf("\n-----------------------------------------------\n-----------------------------------------------\n-----------------------------------------------\n\n");

	printf("MSE        | %f\nMSE(red)   | %f\nMSE(green) | %f\nMSE(blue)  | %f\n-------------------\n",mse_best_all, best_mse_kolor[0],best_mse_kolor[1],best_mse_kolor[2]);
	
printf("SNR        | %f | %f dB\nSNR(red)   | %f | %f dB\nSNR(green) | %f | %f dB\nSNR(blue)  | %f | %f dB\n\n",snr_best_all, 10.0*log10(snr_best_all), best_snr_kolor[0],10.0*log10(best_snr_kolor[0]),best_snr_kolor[1],10.0*log10(best_snr_kolor[1]),best_snr_kolor[2],10.0*log10(best_snr_kolor[2]));   
    
      	printf("-----------------------------------------------\n-----------------------------------------------\n-----------------------------------------------\n\n");
    
/**********************/
/* Zwalnianie pamieci */
/**********************/

	for(int w=0; w<wysokosc; w++){                                            
        	for(int s=0; s<szerokosc; s++){
			free(plan[w][s]);
			free(plan2[w][s]);
		}
		free(plan[w]);
		free(plan2[w]);
	}
	free(plan);
	free(plan2);
    
	return 0;
}
