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
#define SLO_SIZE 1<<20
unsigned long int 	countAll=0,  	     //ilosc wszystkich znako
                    	count[257],         //ilosc znakow
			codedBytes=0;


//gotowe
/********************************************************************************/
/*****************	SLOWNIKOWE RZECZY TO TUTAJ + Programy pomocnicze*********/
/********************************************************************************/

//slowo
typedef struct Word_{           //slowo
    int prefix;                 //slowa w slowniku sa w tablicy. Prefix to indeks prefiksu danego slowa
    unsigned char c;            //literka, ktora razem z prefiksem tworzy slowo.
} Word;

//slownik
typedef struct Slownik_{     //slownik
    Word** words;               //tablica slow
    int size;                   //aktualny rozmiar
} Slownik;

//initialisation
void slo_init(Slownik** d){
    *d = malloc(sizeof(Slownik));
    (*d)->words = malloc(sizeof(Word*) * SLO_SIZE);
    (*d)->size=0;
}

//reset
void slo_reset(Slownik* d){
    for(int i=256; i<d->size; i++)
        free(d->words[i]);
    d->size=256;
}

//dodawanie
void slo_add(Slownik* d, int prefix, unsigned char c){
	if(d->size == SLO_SIZE){
		slo_reset(d);
	return;//nic ne zwroca
    	}
    
	Word* word = malloc(sizeof(Word));
	word->prefix=prefix;
    	word->c=c;
    	d->words[d->size]=word;
    	(d->size)++;
}

//szukanie
int slo_find(Slownik* d, int prefix, unsigned char c){
	for(int i=0; i<d->size; i++)
        	if(d->words[i]->prefix == prefix && d->words[i]->c == c)
        	    return i;
    	return -1;
}

//get first
unsigned char sloget_first(Slownik* d, int n){    
	while(d->words[n]->prefix >= 0) n=d->words[n]->prefix;
	return d->words[n]->c;
}

//drukowanie
void slo_print(Slownik*d, int n, FILE* out){ 
	countAll++;                 
    	if(d->words[n]->prefix >= 0)	slo_print(d, d->words[n]->prefix, out);
    	fputc(d->words[n]->c, out);
}

//free
void slo_free(Slownik** d){
	for(int i=0; i < (*d)->size; i++) free((*d)->words[i]);
	free((*d)->words);
	free(*d);
	*d=NULL;
}


/****************************************************************/
/***********************	WYMIANA		*****************/
/****************************************************************/
						//te same co w pop. liscie(prawie)
//wyslij
int sendBit(FILE* out, unsigned char bit){       //gotowe       
	static unsigned char byte=0;
	static char bitsAmm=0;
	byte = byte << 1 | bit;
	bitsAmm += 1;

	if(bitsAmm == 8){
		fputc(byte,out);
		bitsAmm=0;
		codedBytes++;
        	return 0;
	}
	return bitsAmm;
}

//czytaj
int readBit(FILE* in) {			//gotowe
    	static unsigned char byte=0;
    	static char bitsAmm=0;
   	int i;
    	if(bitsAmm==0){
        	if ((i = fgetc(in)) != EOF){
			byte = (unsigned char) i;
			codedBytes++;
        	}
        	else  return -1;
        	bitsAmm=8;
    	}
    	int bit = byte >> 7;                       
    	byte <<= 1;                                             
	bitsAmm -= 1;
	return bit;
}

//gotowe
/***********************************************************/
/******************  BRAK KODOWANIA plain tekst ************/
/***********************************************************/

void comp_plaintxt(int ch, FILE* out){
	fprintf(out, "%d ", ch);
}

int dek_plaintxt(FILE* in){
	int ch;
	if(fscanf(in,"%d", &ch) == 1)	return ch;
	return -1;
}

/***********************************************************/
/****************	DELTA	****************************/
/***********************************************************/

void comp_delta(int ch, FILE* out){
	int n,j,tmp_n;
	n=j=0;
	ch+=1;
    	int tab[32],tab2[32];
    
	while(ch>0){
        	tab[n] = ch%2;
		n++;
        	ch = ch/2;
    	}
	tmp_n = n;
	while(tmp_n>0){
	        tab2[j] = tmp_n%2;
		j++;
        	tmp_n = tmp_n/2;
    	}
	for(int i=0; i<(j-1); i++)	sendBit(out,0);
	for(int i=j-1; i>=0; i--)	sendBit(out, (unsigned char) tab2[i]);
	for(int i=n-2; i>=0; i--)	sendBit(out, (unsigned char) tab[i]);
}

int dek_delta(FILE* in){
	int j,bt,n,pot,nzwr;
	j=1;
	while((bt=readBit(in)) == 0)	j++;
    	if(bt == -1) return -1;
	pot = 1;
	
	for(int i=1; i<j; i++)	pot*=2;
	
	pot=pot/2;
	n = pot*2;

	for(int i=1; i<j; i++){
        	if((bt=readBit(in)) == -1)	return -1;
        	else{
            		n+=bt*pot;
            		pot=pot/2;
       		}
    	}
	pot = 1;

	for(int i=1; i<n; i++)	pot*=2;

	pot=pot/2;  	
	nzwr = pot*2;

	for(int i=1; i<n; i++){
        	if((bt=readBit(in)) == -1)	return -1;
        	else{
            		nzwr+=bt*pot;
            		pot=pot/2;
        	}
    	}

	return nzwr-1;
}

//gotowe
/***********************************************************/
/****************************	OMEGA	*******************/
/***********************************************************/

void comp_omega(int ch, FILE* out){
	int n=0;
	ch=ch+2;  //reprezentacja jedynki to 0 wiec zwieksze o 2
	int buff[256];

	buff[n++]=0;
    
    	int bin[32];
    	int k=0;
    	while(ch>1){
    	    int tmp=ch;
    	    k=0;
    	    while(tmp>0){
    	        bin[k++] = tmp%2;
    	       // printf("tmp to %d ",tmp);
    	        tmp /= 2;
    	    }
    	    //printf("asdkkk%d \n",k);
    	    for(int i=0; i<k; i++) buff[n++]=bin[i];
    	    ch = k-1;
    	    //printf("asdch c:h%d ",ch);
    	}
    	for(int i=n-1; i>=0; i--){
    	    sendBit(out, (unsigned char) buff[i]);
    	    //printf("buff= %d \n",buff[i]);
    	}
}

int dek_omega(FILE* in){
	int n=1;
	int b;
   	int beginsWithZero=1;
    
    	while((b=readBit(in)) == 1){
        	beginsWithZero=0;
        	int pow = 1;
        	for(int i=1; i<n; i++) pow*=2;
        	int tmp = pow*2;
        	for(int i=0; i<n; i++){
            		if((b=readBit(in)) == -1) return -1;
           		else{
                		tmp+=b*pow;
                		pow/=2;
            		}
        	}
        	n=tmp;
    	}
    	if (b==-1 || beginsWithZero)	return -1;
	return n-2;
}

//gotowe
/***********************************************************/
/****************	Fibonacci	************/
/***********************************************************/

//liczby fiboo
int fib(int n){
	if(n==0) return 1;
	if(n==1) return 2;
	if(n==2) return 3;
	else	return fib(n-1)+fib(n-2);
}//32 czyli 31

void comp_fibo(int ch, FILE* out){     
   	int n=0;  
	ch+=1;
    	int buffer[32];
	int iter=0;
	while (fib(iter+1) <= ch)    iter++; //najwieksza mniejsza od mojej
    	buffer[n++]=1;
	ch-=fib(iter--);
    
	while(ch>0){//ide w dol
		if(fib(iter) <= ch){
            		buffer[n++]=1;
            		ch-=fib(iter);
        	} else	buffer[n++]=0;
		iter--;
    	}
	while(iter>=0){
		buffer[n++]=0;
		iter--;
	}
	for(int i=n-1; i>=0; i--) sendBit(out, (unsigned char) buffer[i]);
	sendBit(out,1);
}

int dek_fibo(FILE* in){  
	int n=0;
	int b;
	int poprz = readBit(in);
	if(poprz == -1)	return -1;
	int zwr=fib(n++)*poprz;
	while((b=readBit(in)) != -1){
        	if(b==1 && poprz==1)	return zwr-1;
        		zwr+=fib(n++)*b;
       			poprz = b;
   		}
	return -1;
}


//gotowe
/***********************************************************/
/****************	GAMMA		************/
/***********************************************************/

void comp_gamma(int ch, FILE* out){                        
	ch+=1;
	int tab[32];
    	int num=0;
    	while(ch>0){
		tab[num++] = ch%2;                                 //zamiana liczby na binarny zapis dlugosci n
	    	ch=ch/2;
    	}
    	for(int i=0; i<num-1; i++)	sendBit(out,0);
    	for(int i=num-1; i>=0; i--)	sendBit(out, (unsigned char) tab[i]);//buff
}

int dek_gamma(FILE* in){
	int b; int num=1; int pot = 1; int zwr;
	while((b=readBit(in)) == 0) num++;
     	if(b == -1)  return -1; //koniec str
    	for(int i=1; i<num; i++) pot*=2;
	zwr = pot;
	pot/=2;
    	for(int i=1; i<num; i++){                            
        	if((b=readBit(in)) == -1)	return -1;
       		else{
            		zwr+=b*pot;
            		pot/=2;
        	}
    	}
	zwr-=1;
	return zwr;
}

/***********************************************************/
/*****************	KOMPILACJE	********************/
/***********************************************************/

	//************************
	//*******kompresja********
	//************************

void compress(FILE* in, FILE* out, void (*write)(int, FILE*)){//gotowe
	int index;
    	int ch;
	Slownik* slo;
	slo_init(&slo);
	for(int i=0; i<256; i++) slo_add(slo, -1, (unsigned char) i);//zainicjalizowanie slownika 8-bitowymi kodami (char)
	int start = fgetc(in);               
  	if (start == EOF)return;
	countAll++;
	while((ch=fgetc(in)) != EOF){
		countAll++;
        	if((index=slo_find(slo, start, (unsigned char) ch)) != -1) start=index;
        	else{                                                                   
			write(start, out);                      
           		slo_add(slo, start, (unsigned char) ch);           
            		start = ch; //reset startu.
        	}
   	}   
	write(start, out);//kod
	while(sendBit(out,0) > 1);//dopelnianie do 8
	slo_free(&slo);
}

	//**************************
	//*******DEkompresja********
	//**************************

void decompress(FILE* in, FILE* out, int (*read)(FILE*)){//gotowe
	int ch;
	Slownik* slo;
    	slo_init(&slo);
    	for(int i=0; i<256; i++) slo_add(slo, -1, (unsigned char) i);
	int poprz = read(in);
	if(poprz < 0)	return;   
    	slo_print(slo, poprz, out);

    	while((ch=read(in)) != -1){
        	if(ch < slo->size){
            	slo_add(slo, poprz, sloget_first(slo, ch));
		slo_print(slo, ch, out);
        	}else{//nie ma w slowaniku
            		slo_add(slo, poprz, sloget_first(slo, poprz));
            		slo_print(slo, slo->size - 1, out);
        	}
	poprz=ch;
    	}
	slo_free(&slo);   
}


/***********************************************************/
/***********************	main	********************/
/***********************************************************/

int main(int argc, char** argv){

	int option = 0; int c = 2; int filecounter = 0;
	FILE* in; FILE* out;

        if(argc<4){
		fprintf(stderr,"Bad input!\n Usage case: ./main -c/-d -gamma/-delta/-omega/-fibonacci/-plain <input file> <output file>)\n");
		return 0;
	}
	argc--; argv++;
	while(argc>0){
        	if(!strcmp(argv[0],"-c"))option=0;
        	else if(!strcmp(argv[0],"-d"))option=1;
        	else if(!strcmp(argv[0], "-plain"))c=0;
        	else if(!strcmp(argv[0], "-delta"))c=1;                                  
        	else if(!strcmp(argv[0], "-omega"))c=2;
        	else if(!strcmp(argv[0], "-fibonacci"))c=3;
        	else if(!strcmp(argv[0], "-gamma"))c=4;
        	else if(filecounter == 0){
			in = fopen(argv[0], "rb");
			filecounter++;
        	}
        	else if(filecounter==1){
			out = fopen(argv[0], "wb");
			filecounter++;
        	}
        	argc--; argv++;
    	}
    
	if(!in || !out){
		printf("Cos poszlo nie tak. Zle dane\n");
		return 0;
	}

	//*********************
	//***Wlaczam program***
	//*********************
	void (*komp[])(int, FILE*)  = {comp_plaintxt, comp_delta, comp_omega, comp_fibo,comp_gamma }; //komper
    	int (*dekomp[])(FILE*) = {dek_plaintxt, dek_delta, dek_omega, dek_fibo, dek_gamma};       //dekąper
	if(!option) compress(in, out, komp[c]);
	else decompress(in, out, dekomp[c]);
	/*
	    if(!option)
		if(c=0) compress(in, out, comp_plaintxt);
		if(c=1) compress(in, out, comp_delta);
		if(c=2) compress(in, out, comp_omega);
		if(c=3) compress(in, out, comp_fibo);	
		if(c=4) compress(in, out, comp_gamma);
    	else
		if(c=0) decompress(in, out, dek_plaintxt);
		if(c=1) decompress(in, out, dek_delta);
		if(c=2) decompress(in, out, dek_omega);
		if(c=3) decompress(in, out, dek_fibo);
		if(c=4) decompress(in, out, dek_gamma);
	*/

	//*********************
	//*****Statystyki******
	//*********************
//drukuje
	printf("Stopien kompresji: %f%%\n", (double)codedBytes/(double)countAll*100.0);      //stopien kompresji: waga przed / waga po 
	printf("Dlugosc sreania kodu: %f\n", (double)codedBytes*8.0/(double)countAll);    //l. zakodowanych bitow / l. znakow
//zamykam pliki
        fclose(in);
        fclose(out);
    
return 0;
}
