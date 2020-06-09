#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int pop(int x){
	int rtr;              
	for (rtr=0; x; rtr++)	x &= x - 1;
	return rtr % 2;
}

int main(int argc, char** argv){
	if(argc <3){
		printf("\nzle dane\n   Usage: ./koder <in_file> <out_file>\n\n");
		return 0;	
	}
        char x, y; 
	int matrix[8] = {13, 11, 8, 7, 4, 2, 1, 14}; //gen. kod Hamminga (8,4)
	int c;
	FILE*  in_file  = fopen(argv[1], "r");
	FILE* out_file = fopen(argv[2], "w");

   	while((c=fgetc(in_file)) != EOF){	//as long as mam co kodowac
		x=y=0;
		for(int i=0; i<8; i++){     
        	    	x += (pop((c >> 4)  & matrix[i]) << (7-i));	
			y += (pop((c & 0xF) & matrix[i]) << (7-i));
        	}      
        	putc(x,out_file);
        	putc(y,out_file);
    	}  
    
	fclose(in_file);
	fclose(out_file);

	return 0;
}
