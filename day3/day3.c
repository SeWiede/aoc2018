#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

#define FIELDSIZE 1000

int field[FIELDSIZE][FIELDSIZE];

void fill(int x, int y, int fc){
	if(x < 0 || y <0 || y >= FIELDSIZE || x >= FIELDSIZE)
		return;

	printf("x: %d, y:%d\n", x,y);

	if(field[x][y] == fc){
		field[x][y] = -1;
		fill(x-1, y, fc);
		fill(x, y+1, fc);
		fill(x, y-1, fc);
		fill(x+1, y, fc);

	}
		
}

void printField(){
	for(int i=0; i<FIELDSIZE;i++){
		for(int j=0;j<FIELDSIZE;j++){
			printf("%3d ", field[i][j]);
		}
		printf("\n");
	}
}

int main(int argc, char * args[]){
	
	char *buf = (char *)malloc(100*sizeof(char));

	for(int i=0; i<FIELDSIZE;i++){
		for(int j=0;j<FIELDSIZE;j++){
			field[i][j]=0;
		}
	}

	uint8_t claimp[2000]; memset(&claimp[0], 0, 2000);
	uint16_t claims=0;
	uint16_t sum=0;;

	uint16_t count=0;

	while(fgets(buf, sizeof(char)*100, stdin) != NULL){
		char delim = '#';
		strsep(&buf, &delim);
		int claim =strtol(buf,&buf,10);

		delim = '@';
		strsep(&buf, &delim);

		buf++;

		int y =strtol(buf,&buf,10);
		delim = ',';
		strsep(&buf, &delim);
		int x =strtol(buf,&buf,10);

		delim = ':';
		strsep(&buf, &delim);
		buf++;

		int h =strtol(buf,&buf,10);
		delim = 'x';
		strsep(&buf, &delim);
		int w =strtol(buf,&buf,10);
	
		claims++;

		claimp[claims-1] = 1;
		sum+=claim;

		for(int i=0; i<w;i++){
			for(int j=0;j<h;j++){
				int *current = &(field[x+i][y+j]);
				if(*current == 0){
					*current = claim;	
				}else{
					if(claimp[claim-1]){
						sum-=claim;
						claimp[claim-1] =0;
					}
					if(*current ==-1) continue;
					if(claimp[*current-1]){
						sum-=*current;						
						claimp[*current-1] = 0;
					}
					if(*current != -1){
						count++;
						*current = -1;
					}
				}
			}
		}
	}
	printf("intersections %d\n", count);

	printf("solo guy: %d\n", sum);
	return 0;
}































