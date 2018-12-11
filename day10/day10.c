#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

#define FIELDSIZE 1000

struct posVel{
	int x;
	int y;
	int xv;
	int yv;
};

int main(int argc, char * args[]){
	
	char *buf = (char *)malloc(100*sizeof(char));

	FILE * in = fopen("day10.in", "r");

	struct posVel *posVel = NULL;
	int count =0;
	while(fgets(buf, sizeof(char)*100, in) != NULL){
		char * temp = buf;
		if(strlen(buf) < 2) continue;
		count++;
		for(int i=0;i<strlen(buf);i++){
			if(buf[i] == ' '){
				memmove(&buf[i], &buf[i + 1], strlen(buf) - i);
			}
		}
		posVel = realloc(posVel, count*sizeof(struct posVel));
		
		posVel[count-1].x = strtol(buf+10, &buf, 10);
		posVel[count-1].y = strtol(buf+1, &buf, 10);
		posVel[count-1].xv = strtol(buf+11, &buf, 10);
		posVel[count-1].yv = strtol(buf+1, NULL, 10);
		buf = temp;
		printf("x: %d y: %d xv: %d yv: %d\n", posVel[count-1].x, posVel[count-1].y, posVel[count-1].xv, posVel[count-1].yv);
	}
	int steps=1;
	int positiveX=0;
	int maxx=-1;
	int maxy=-999;	
	
	int wasPositive=0;
	for(;;steps++){
		if(positiveX==1){
			wasPositive=1;
		}
		positiveX=1;
		for(int i =0; i<count; i++){
			posVel[i].x+=posVel[i].xv;
			posVel[i].y+=posVel[i].yv;
			if(posVel[i].x < 0) positiveX=0;
			if(wasPositive){
				if(posVel[i].x > maxx) maxx = posVel[i].x;
				if(posVel[i].y > maxy) maxy = posVel[i].y;
			}
		}	
		if(positiveX && wasPositive){
		printf("maxy: %d maxx: %d\n", maxy, maxx);
			char f[maxy+1][maxx+1];
		printf("press anything to continue...\n");
			for(int y=0;y<maxy;y++){
				for(int x=0;x<maxx;x++){
					f[y][x] = '.';
				}
			}
			for(int i =0; i<count; i++){
				f[posVel[i].y][posVel[i].x] = '#';	
				//printf("x: %d y: %d xv: %d yv: %d\n", posVel[i].x, posVel[i].y, posVel[i].xv, posVel[i].yv);
			}	
			for(int y=0;y<maxy;y++){
				for(int x=0;x<maxx;x++){
					int is=0;
					for(int u=0;u<maxx;u++){if(f[y][u] == '#') {is=1;break;}}
					if(!is) {goto skip;}
					printf("%c",f[y][x]);
				}
				printf("\n");
				skip:
				;
			}
			printf("secs: %d\n", steps);
			fgets(buf, 100, stdin);
		}
	}		

	return 0;
}































