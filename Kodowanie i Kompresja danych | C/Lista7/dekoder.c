#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int pop(int x){
	int rtr;              
	for (rtr=0; x; rtr++)	x &= x - 1;
	return rtr % 2;
}

int main(int argc, char** argv){
	if(argc <3){
		printf("\nzle dane\n   Usage: ./dekoder <in_file> <out_file>\n\n");
		return 0;	
	}

        unsigned char x=0;
	int matrix[3] = {170, 102, 30};                    //macierz parzystosci
	int c[2], errors=0;
	FILE* in_file  = fopen(argv[1], "r");
	FILE* out_file = fopen(argv[2], "w");

	while((c[0]=fgetc(in_file)) != EOF){
		c[1]=fgetc(in_file);
        
		for(int i=0; i<2; i++){ 
			x=0;
            		for(int j=0; j<3; j++)	x += (pop(c[i] & matrix[j]) << (2-j));   //syndrom
            
            		if(pop(c[i])){                   	//if bit parzystosci not OK:
                		if(x)	c[i] ^= (1 << (8-x));	//zmiana zlego bitu
				else  	c[i] ^= 1; 		//zmiana na przeciwny
			}
			else if(x)	errors++;		
            	} 

		x = ((c[0] << 2) & (1 << 7)) + ((c[0] << 3) & 112) + ((c[1] >> 2) & (1 << 3)) + ((c[1] >> 1) & 7);         
        	putc(x, out_file);
	}

	printf("Kody z dwoma bledami: %d\n", errors);
    
	fclose(in_file);
	fclose(out_file);
 
    	return 0;
}
