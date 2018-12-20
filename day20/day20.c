#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include "AStar.h"


#define ROOMSIZE 500

char room[ROOMSIZE+1][ROOMSIZE+1];

struct coord{
	int x,y;
};


int findSubRegex(char *regex){
	int pos=0;
	int o=1;
	while(o){
		if(regex[pos] == '(') o++;
		if(regex[pos] == ')') o--;
		pos++;
	}
	return pos-1;
}

int constructRoom(char *regex, int x, int y, int pos){
	int startx=x;
	int starty=y;
	while(regex[pos] != '$'){
		room[y-1][x-1] = '#';
		room[y+1][x+1] = '#';
		room[y+1][x-1] = '#';
		room[y-1][x-1] = '#';	

		if(regex[pos] == 'N'){
			room[y-1][x] = '-';
			room[y-2][x] = '.';
			y-=2;
		}else if(regex[pos] == 'E'){
			room[y][x+1] = '|';
			room[y][x+2] = '.';
			x+=2;
		}else if(regex[pos] == 'S'){
			room[y+1][x] = '-';
			room[y+2][x] = '.';
			y+=2;
		}else if(regex[pos] == 'W'){
			room[y][x-1] = '|';
			room[y][x-2] = '.';
			x-=2;
		}else if(regex[pos] == '|'){
			x=startx;
			y=starty;
		}
		if(regex[pos] == '('){
			int cb = findSubRegex(regex+pos+1);
			char *sub = malloc(sizeof(char)*cb+1);
			strncpy(sub, regex+pos+1, cb);
			sub[cb] = '$';
			pos +=constructRoom(sub, x,y,0)+1;
			free(sub);
		}else{
			pos++;
		}
	}
	return pos;
}

float h(void *from, void *to, void *context){
	return abs(((struct coord *)from)->x - ((struct coord *)to)->x) + abs(((struct coord *)from)->y - ((struct coord *)to)->y);
}

int nc(void *node1, void *node2, void *context){
	struct coord *c1 = (struct coord *)node1;
	struct coord *c2 = (struct coord *)node2;
		
	if(c1->y < c2->y) return -1;
	if(c1->y > c2->y) return 1;
	if(c1->x < c2->x) return -1;
	if(c1->x > c2->x) return 1;
	return 0;
}

void PathNodeNeighbors(ASNeighborList neighbors, void *node, void *context)
{
    struct coord *pathNode = (struct coord *)node;

    if (room[pathNode->y-1][pathNode->x] == '-') {
        ASNeighborListAdd(neighbors, &(struct coord){pathNode->x, pathNode->y-2}, 1);
    }
    if (room[pathNode->y][pathNode->x-1] == '|') {
        ASNeighborListAdd(neighbors, &(struct coord){pathNode->x-2, pathNode->y}, 1);
    }
    if (room[pathNode->y][pathNode->x+1] == '|') {
        ASNeighborListAdd(neighbors, &(struct coord){pathNode->x+2, pathNode->y}, 1);
    }
    if (room[pathNode->y+1][pathNode->x] == '-') {
        ASNeighborListAdd(neighbors, &(struct coord){pathNode->x, pathNode->y+2}, 1);
    }
}



int main(int argc, char * argv[]){
		
	char *buf = malloc(sizeof(char)*256*128);

	memset(room, '?', sizeof(room));

	for(int y=0;y<ROOMSIZE;y++){
		room[y][ROOMSIZE]='\0';
	}

	fgets(buf, 256*128, stdin);
	
	room[ROOMSIZE/2][ROOMSIZE/2] = 'X';	

	constructRoom(buf+1, ROOMSIZE/2, ROOMSIZE/2, 0);

	for(int y=0;y<ROOMSIZE;y++){
		for(int x=0;x<ROOMSIZE;x++){
			if(room[y][x] == '?' || room[y][x] == '\0'){
				room[y][x] = '#';
			}
		}
	}

	static const ASPathNodeSource PathNodeSource =
		{
			sizeof(struct coord),
			&PathNodeNeighbors,
			&h,
			NULL,
			&nc
		};


	int max=0;
	int mt1000d=0;
	for(int y=0;y<ROOMSIZE;y++){
		for(int x=0;x<ROOMSIZE;x++){
			if(room[y][x] == '.'){
				ASPath path =ASPathCreate(&PathNodeSource, NULL, &(struct coord){ROOMSIZE/2,ROOMSIZE/2}, &(struct coord){x,y});
				int pL= ASPathGetCount(path);
				if(pL-1 > max)
					max=pL-1;
				if(pL-1 >= 1000)
					mt1000d++;
			}
		}
	}

	printf("max doors are: %d\n", max);
	printf("%d rooms require more than 1000 doors\n", mt1000d);

	free(buf);
	return 0;
}































