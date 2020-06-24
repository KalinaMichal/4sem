#include <stdio.h>
#include <time.h>
#include <stdlib.h>

_Bool los(double prob){return rand() <  prob * ((double)RAND_MAX + 1.0);}

int main(int argc, char** argv){

    	if(argc < 4){
		printf("\nZle dane\n  Usage: ./szum <prawdopodobienstwo> <in_file> <out_file>\n\n");
	 	return 0;
	}

	srand(time(NULL));  
        int c;
	double prob = atof(argv[1]);
	FILE*  in_file  = fopen(argv[2], "r");
	FILE*  out_file = fopen(argv[3], "w");

	
	while((c=fgetc(in_file)) != EOF){
		for(int i=0; i<8; i++)	if(los(prob))  c ^= (1 << i);	//zmiana XOR
		putc(c, out_file);
    	}   
    
	fclose(in_file);
	fclose(out_file);
    
    	return 0;
}
