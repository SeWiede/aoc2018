#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>


struct coord{
	int x,y;
	int area;
};


int main(int argc, char * args[]){
		
	char * buf = malloc(sizeof(char)*256);

	struct coord *cs =NULL;	
	int csc=0;

	int maxx=0;
	int maxy=0;
	
	while(fgets(buf, 256*sizeof(char),stdin) != NULL){
		if(strlen(buf) <2)continue;
		char *temp=buf;
		csc++;
		cs= realloc(cs, sizeof(struct coord)*csc);
		int x=strtol(buf,&buf,10);
		int y=strtol(buf+1,NULL,10);
		cs[csc-1].x=x;
		cs[csc-1].y=y;
		cs[csc-1].area=0;
		if(x > maxx) maxx=x;
		if(y > maxy) maxy=y;
		buf=temp;
	}
	
	char field[maxy+1][maxx+1];memset(field, '.', sizeof(field));
	
	/*int maxa=0;
	for(int y=0;y<maxy+1;y++){
		for(int x=0;x<maxx+1;x++){
			int smd=9999;
			struct coord *cur;
			for(int i=0;i<csc;i++){
				int d = abs(cs[i].x-x)+abs(cs[i].y-y);
				if(d < smd) {
					smd=d;
					cur=&(cs[i]);
				}
			}
			if((*cur).area != -1){
				if(x==0 || y==0 || x == maxx || y == maxy){
					(*cur).area=-1;				
				}else{
					(*cur).area++;
					if((*cur).area > maxa) maxa= (*cur).area;
				}
			}
		}
	}

	printf("%d\n",maxa);*/
	
	int area=0;
	for(int y=0;y<maxy+1;y++){
		for(int x=0;x<maxx+1;x++){
			int d=0;
			for(int i=0;i<csc;i++){
				d += abs(x-cs[i].x)+abs(y-cs[i].y);
			}
			if(d < 10000){
				area++;
			}
		}
	}
	
	printf("%d\n", area);

	return 0;
}




















































