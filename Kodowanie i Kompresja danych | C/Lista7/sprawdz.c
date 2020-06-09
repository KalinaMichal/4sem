#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char** argv){
	int c1, c2, same=0;
	if(argc <3){
		printf("\nzle dane\n   Usage: ./sprawdz <first in_file> <second in_file>\n\n");
		return 0;	
	}
    
	FILE* in_file1 = fopen(argv[1], "r");
	FILE* in_file2 = fopen(argv[2], "r");
      
	while((c1=fgetc(in_file1)) != EOF){
        	if((c2=fgetc(in_file2)) == EOF)	break;

     		if((c1 & 0xF) != (c2 & 0xF)) same++;       //4 last  b.
		if((c1 >> 4) != (c2 >> 4)) same++;         //4 first b.

	}
      
	printf("4-bitowe nie identyczne bloki %d\n",same);
  
	fclose(in_file1);
	fclose(in_file2);

	return 0;
}
