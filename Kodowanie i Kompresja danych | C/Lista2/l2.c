#include <stdio.h>
#include <string.h>
#include <math.h>
//#include <unistd.h>
#include <stdlib.h>
//#include <limits.h>

#define MAX     0x7FFFFFFFFFFFFFFFLLU   // 2^63 - 1
#define HALF    0x4000000000000000LLU   // MAX*0.5
#define HALFL   0x2000000000000000LLU   // MAX*0.25
#define HALFR   0x6000000000000000LLU   // MAX*0.75 


//konce
unsigned long long int  l=0,            //lewy
                        r=MAX;          //prawy
                        
unsigned long int   countAll = 257,     //ilosc wszystkich znako
                    count[257],         //ilosc znakow
                    codedBytes=0;      

//*********************************************************
//********************FUNKCJE POMOCNICZE*******************
//*********************************************************

//wyslij
int sendBit(FILE* out, unsigned char bit){
	static unsigned char byte=0;
    	static char bitsAmm=0;
    	byte = byte << 1 | bit;
    	bitsAmm += 1;

    	if(bitsAmm == 8){
        	fputc(byte,out);
       	 	codedBytes++;
        	bitsAmm=0;
        	return 0;
    	}
    	return 1;
}

//czytaj
unsigned char readBit(FILE* in) {
	static unsigned char byte=0;
	static char bitsAmm=0;
	int i;
   	if(bitsAmm==0){
        	if ((i = getc(in)) != EOF) {
            		byte = (unsigned char) i;
            		codedBytes++;
        	}
        	else byte=0;
        	bitsAmm=8;
    	}
	unsigned char bit = byte >> 7;
	byte <<= 1;
	bitsAmm -= 1;
	return bit;
}

//*********************************************************
//********************FUNKCJE WLASCIWE*********************
//*********************************************************

//***************************************
//*******	KOMPRESJA	*********
//***************************************

void compress(FILE* in, FILE* out){
	int letter=0;
	unsigned int counter=0;
	while (letter!=256) {                           //petla az nie mam EOF
		letter = getc(in);
		if(letter == EOF)letter=256;
        
        	unsigned long int countL=0;
		int j;
        	for(j=0; j<letter; j++) countL+=count[j];
        
       		unsigned long long int jump = (r - l + 1) / countAll;
        	r=l+jump*(countL+count[j])-1;
        	l=l+jump*countL;
        
		//***************
		//**Skalowanie***
		//***************

        	while(r<HALF || l>=HALF){
 	// przedzial [0, 0.5)
            		if (r<HALF){                           
                		l*=2;
                		r=r*2+1;
                		sendBit(out,0);
                		for(;counter>0;counter--) sendBit(out,1);
            		}
	// przedzial [0.5, 1)
            		else if (l>=HALF){              
              			l=2*(l-HALF);
               			r=2*(r-HALF)+1;
                		sendBit(out,1);
                		for(;counter>0;counter--) sendBit(out,0);
            		}
        	}
	// przedzial [0.25, 0.75)
        	while (l>=HALFL && r<HALFR){
           		counter++;
			l=2*(l-HALFL);
            		r=2*(r-HALFL)+1;
       		 }
	// iterowanie licznikow
		if(letter < 256){   
           		countAll++;                                    
            		count[letter]++;

        	}
	}
	//konczenie
    	if(l<HALFL){	//przedzial [0, 0,75)
        	sendBit(out,0);
        	for(unsigned int i=0; i<counter+1;i++) sendBit(out,1);
    	}
    	else sendBit(out,1);	//przedzial [0.25, 1)
	while(sendBit(out,0));  //dodaje brakujace zera  
}

//***************************************
//*******	DEKOMPRESJA	*********
//***************************************

void decompress(FILE* in, FILE* out){
	unsigned long long int p=0;//znacznik (pointer)
	int letter;
	for(int i=0; i<63; i++) p = (p<<1) + readBit(in);	 //wczytuje po bitcie do p-(znacznika)

	do{
        	unsigned long long int jump = (r-l+1)/countAll; //"skok" miedzy znakami
        	unsigned long long int value = (p-l)/jump;      //wartosc znacznika w proporcji do krokow
        	unsigned long int countL=0;
        	for(letter=0; countL + count[letter] <= value; letter++)countL += count[letter]; //Szukam znaku co ma przedzial w ktorym znacznik jest
		if(letter < 256) fputc(letter,out);
        	r=l+jump*(countL+count[letter])-1;
       		l=l+jump*countL;
		
		//****************
		//**Odskalowanie** (analogicznie jak w kompresji tylko ze inaczej)
		//****************
 	// przedzial [0, 0.5)
        	while(r<HALF || l>=HALF){              
        		if (r<HALF){
                		l*=2;
                		r=r*2+1;
                		p=p*2+readBit(in);          //pointer*2 + nowy bit
            		}
	// przedzial [0.5, 1)
            	else if (l>=HALF){
                	l=2*(l-HALF);
                	r=2*(r-HALF)+1;
                	p=2*(p-HALF)+readBit(in);           //pointer*2-1 + nowy bit
            	}
        }
	// przedzial [0.25, 0.75)
        while (l>=HALFL && r<HALFR){
		l=2*(l-HALFL);
		r=2*(r-HALFL)+1;
		p=2*(p-HALFL)+readBit(in);                    //pointer*2-0.5 + nowy bi
        }
        
	if(letter < 256){
		countAll++;
		count[letter]++;
        }	
	} while(letter != 256);
}

//***************************************
//*******     PROGRAM GŁÓWNY    *********
//***************************************
int main(int argc, char** argv){
	if (argc < 4){
	        printf("*** Usage ***\ncompression:   ./a.out -c <in_file> <out_file>\ndecompression: ./a.out -d <in_file> <out_file>\n");
	        return 0;
	}

	FILE* in = fopen(argv[2], "rb");	//czytaj
	FILE* out = fopen(argv[3], "wb");	//pisz

	if(!in || !out){
	        printf("File error\n");
	        return 0;
	}

	for(int i=0; i<257; i++) count[i]=1;
    
   	if(!strcmp(argv[1], "-d")) decompress(in, out);
    	else if(!strcmp(argv[1], "-c")) compress(in, out);
	else{
		printf("*** Usage ***\ncompression:   ./a.out -c <in_file> <out_file>\ndecompression: ./a.out -d <in_file> <out_file>\n");
	        return 0;
	}

	fclose(in);
	fclose(out);

//***************************************
//*******	STATYSTYKI	*********
//***************************************

	//printf("%ld",countAll);
	double entropy = 0;
	countAll-=257;//l.znk.+eof
	for (int i=0; i<256; i++){
		count[i]--;
        	if(count[i]>0){
            		double probability  =  (double) count[i] / (double) countAll;
            		entropy += probability*log2(probability);    //wyliczenie entropii
        	}
	}

	printf("Entropy = %f\n", fabs(entropy));
	printf("Average code lengh: %f\n", ((double)codedBytes*8.0)/(double)countAll);
	printf("Compression level: %f%%\n",(double)codedBytes/(double)countAll*100.0);      //=(waga przed)/(waga po)
/**/
    return 0;
}
