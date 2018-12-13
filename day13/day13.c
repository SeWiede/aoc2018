#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

#define FS 300

enum direction{
	LEFT, RIGHT, DOWN, UP
};

struct cart{
	int x, y;
	enum direction d;
	int is;
};


int cmp(const void *p1, const void *p2){
	if(((struct cart *)p1)-> x< ((struct cart *)p2)-> x){
		return 0;
	}else if(((struct cart *)p1)-> x ==  ((struct cart *)p2)-> x){
		return !(((struct cart *)p1)-> y< ((struct cart *)p2)-> y);
	}
	return 1;
}



int checkCrash(char **t, int x, int y){
	return t[y][x] == '<' || t[y][x] == 'v' || t[y][x] == '^' || t[y][x] == '>';
}

void printtracks(char **t,int xb, int yb){
	printf("   ");
	for(int x =0;x<xb;x++){
		printf("%d", x/100);
	}
	printf("\n   ");
	for(int x =0;x<xb;x++){
		printf("%d", (x/10)%10);
	}
	printf("\n   ");
	for(int x =0;x<xb;x++){
		printf("%d", x%10);
	}
	printf("\n");
	for(int y =0;y<yb;y++){ 
		printf("%d%d%d",y/100, (y/10)%10, y%10);
		printf("%s", t[y]);			
	}
}

void printtrackscarts(char **t,struct cart *c,int cc, int xb, int yb){

	char **temp = malloc(sizeof(char*)*yb);
	for(int i=0;i<yb;i++){
		temp[i]=malloc(sizeof(char)*xb);
		strcpy(temp[i], t[i]);
	}
	for(int i =0;i< cc;i++){
		temp[c[i].y][c[i].x] = c[i].d == RIGHT ? '>' : (c[i].d == UP ? '^' : (c[i].d == DOWN ? 'v':'<'));
	}
	printf("   ");
	for(int x =0;x<xb;x++){
		printf("%d", x/100);
	}
	printf("\n   ");
	for(int x =0;x<xb;x++){
		printf("%d", (x/10)%10);
	}
	printf("\n   ");
	for(int x =0;x<xb;x++){
		printf("%d", x%10);
	}
	printf("\n");
	for(int y =0;y<yb;y++){ 
		printf("%d%d%d",y/100, (y/10)%10, y%10);
		printf("%s", temp[y]);			
	}
	for(int i=0;i<yb;i++){
		free(temp[i]);
	}
	free(temp);
}

int checkcrash(struct cart **c, int *cc, int *i){
//start:

		for(int j=0;j<*cc;j++){
			if(*i==j) continue;
			if((*c)[(*i)].x == (*c)[j].x &&(*c)[*i].y == (*c)[j].y ){
				//printf("deleting %d and %d\n", i, j);
				//for(int k=0;k<*cc;k++){
				//	printf("cart: %d on: %d %d\n", k+1, (*c)[k].x, (*c)[k].y);
				//}//printf("------------\n");
				printf("crash on: (%d, %d)\n",(*c)[(*i)].x, (*c)[(*i)].y);
				(*cc)-=2;
				int d1,d2;
				if(*i > j){
					d1 =j;
					d2=*i;
				}else{
					d1=*i;
					d2=j;
				}
				
				struct cart *nc = malloc(sizeof(struct cart)*(*cc));
				memcpy(nc, *c, sizeof(struct cart)*d1);
				memcpy(nc+d1, (*c)+d1+1, sizeof(struct cart)*(d2-d1-1));
				memcpy(nc+d2-1, (*c)+d2+1, sizeof(struct cart)*((*cc+2)-d2-1));
				free(*c);
				(*c) = nc;
				
				if(*i > j){
					(*i)-=2;
				}else{
					(*i)--;
				}

				//goto start;
				for(int k=0;k<*cc;k++){
					//printf("cart: %d on: %d %d dir: %d\n", k+1, (*c)[k].x, (*c)[k].y, (*c)[k].d);
				}//printf("------------\n");

			}
		
	}
	return (*cc);
}

int main(int argc, char * args[]){
		
	char * buf = malloc(sizeof(char)*256);

	char **tracks = NULL;
	int yb=0;
	int xb=0;

	while(fgets(buf, 256*sizeof(char),stdin) != NULL){
		if(strlen(buf) <2) break;
		xb= strlen(buf);
		yb++;
		tracks = realloc(tracks, sizeof(char*)*yb);
		tracks[yb-1]=malloc(sizeof(char)*xb+1);
		strncpy(tracks[yb-1], buf, xb+1);
		//printf("%s\n", buf);
		
	}


	

	char **old = malloc(sizeof(char*)*yb);
	for(int y =0;y<yb;y++){
		old[y] = malloc(sizeof(char)*xb);
		strcpy(old[y], tracks[y]);
	}


	struct cart *carts=NULL;
	int cc=0;

	for(int y =0;y<yb;y++){
			for(int x=0;x<xb;x++){
				switch(tracks[y][x]){
					case '>':{
						cc++;
						carts = realloc(carts, sizeof(struct cart)*cc);
						carts[cc-1].x=x;
						carts[cc-1].y=y;
						carts[cc-1].d=RIGHT;
						carts[cc-1].is=0;
						tracks[y][x]='-';
						break;
					}
					case '<':{
						cc++;
						carts = realloc(carts, sizeof(struct cart)*cc);
						carts[cc-1].x=x;
						carts[cc-1].y=y;
						carts[cc-1].d=LEFT;
						carts[cc-1].is=0;
						tracks[y][x]='-';
						break;
					}
					case '^':{
						cc++;
						carts = realloc(carts, sizeof(struct cart)*cc);
						carts[cc-1].x=x;
						carts[cc-1].y=y;
						carts[cc-1].d=UP;
						carts[cc-1].is=0;
						tracks[y][x]='|';
						break;
					}
					case 'v':{	
						cc++;
						carts = realloc(carts, sizeof(struct cart)*cc);
						carts[cc-1].x=x;
						carts[cc-1].y=y;
						carts[cc-1].d=DOWN;
						carts[cc-1].is=0;
						tracks[y][x]='|';
						break;
					}
					default:{
						break;
					}
				}
			}				
		}

	//printtracks(tracks, xb,yb);

	

	//printtracks(old, xb,yb);

	int xc=-1;
	int yc=-1;
	
	int cycle=-1;

	for(int i=1;;i++){
		//printf("cycle: %d\n",i);
		//printtrackscarts(tracks, carts, cc, xb,yb);
		int temp =cc;
		int last=0;
		for(int c =0;c<temp;c++){
			int ch=0;
			switch(carts[c].d){
				case LEFT:{
					carts[c].x--;

					char t = tracks[carts[c].y][carts[c].x];
					int is = carts[c].is;
					if(t == '+'){
						if(is%3==0){
							carts[c].d = DOWN;
							carts[c].is++;
						}else if(is%3==1){
							//carts[c].d = DOWN;
							carts[c].is++;
						}else{
							carts[c].d = UP;
							carts[c].is++;
						}
					}else if(t == '\\'){
						carts[c].d = UP;
					}else if(t == '/'){
						carts[c].d = DOWN;
					}
				}	
ch=checkcrash(&carts, &cc, &c);
	if(ch==1){
				xc=carts[0].x;
				yc=carts[0].y;
				cycle=i;
				last=1;
			}
				break;
				case RIGHT:{
					carts[c].x++;

					char t = tracks[carts[c].y][carts[c].x];
					int is = carts[c].is;
					if(t == '+'){
						if(is%3==0){
							carts[c].d = UP;
							carts[c].is++;
						}else if(is%3==1){
							//carts[c].d = DOWN;
							carts[c].is++;
						}else{
							carts[c].d = DOWN;
							carts[c].is++;
						}
					}else if(t == '\\'){
						carts[c].d = DOWN;
					}else if(t == '/'){
						carts[c].d = UP;
					}
				}	
ch=checkcrash(&carts, &cc, &c);
	if(ch==1){xc=carts[0].x;
				yc=carts[0].y;
				cycle=i;
				last=1;
			}
				break;
				case UP:{
					carts[c].y--;

					char t = tracks[carts[c].y][carts[c].x];
					int is = carts[c].is;
					if(t == '+'){
						if(is%3==0){
							carts[c].d = LEFT;
							carts[c].is++;
						}else if(is%3==1){
							//carts[c].d = DOWN;
							carts[c].is++;
						}else{
							carts[c].d = RIGHT;
							carts[c].is++;
						}
					}else if(t == '\\'){
						carts[c].d = LEFT;
					}else if(t == '/'){
						carts[c].d = RIGHT;
					}
				}	
ch=checkcrash(&carts, &cc, &c);
	if(ch==1){xc=carts[0].x;
				yc=carts[0].y;
				cycle=i;
				last=1;
			}
				break;
				case DOWN:{
					carts[c].y++;

					char t = tracks[carts[c].y][carts[c].x];
					int is = carts[c].is;
					if(t == '+'){
						if(is%3==0){
							carts[c].d = RIGHT;
							carts[c].is++;
						}else if(is%3==1){
							//carts[c].d = DOWN;
							carts[c].is++;
						}else{
							carts[c].d = LEFT;
							carts[c].is++;
						}
					}else if(t == '\\'){
						carts[c].d = RIGHT;
					}else if(t == '/'){
						carts[c].d = LEFT;
					}
					
				}	
ch=checkcrash(&carts, &cc, &c);
	if(ch==1){xc=carts[0].x;
				yc=carts[0].y;
				cycle=i;
				last=1;
			}
				break;
				default: break;
			}
			if(ch != temp) temp = ch;
		}if(last) goto done;
		qsort(carts, cc, sizeof(struct cart), cmp);
		//printtracks(tracks, xb,yb);
	}
	done:
	//printf("crash at: (%d, %d) after %d cycles\n", xc,yc,cycle);
	printf("pos of last cart: %d %d\n", xc,yc);
	return 0;
}































