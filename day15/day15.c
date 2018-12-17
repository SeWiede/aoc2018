#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

#include "AStar.h"

enum kind{
	GOBLIN='G', ELF='E', DEAD ='.'
};

struct mob{
	int hp;
	int x;
	int y;
	enum kind k;
};
	
struct coord{
	int x,y;
};


char **field =NULL;
int maxy=0;
int maxx=0;

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

struct mob* getMob(struct mob *mobs, int mobc, int x, int y){
	for(int i=0;i<mobc;i++){
		if(mobs[i].x == x && mobs[i].y == y && mobs[i].hp >0){
			return &(mobs[i]);
		}
	}
	return NULL;
}

struct mob *getMobNeighborWithLeastHP(struct mob *mobs, int mobc, struct mob m){
	struct mob *ret = NULL;
	struct mob *temp = NULL;

	int x= m.x;
	int y=m.y;	
	
	ret = getMob(mobs, mobc, x, y-1);
	if(ret != NULL && (ret->k==m.k || ret->hp<=0)) ret =NULL;

	temp = getMob(mobs, mobc, x-1, y);
	if(temp != NULL && (temp->k==m.k || temp->hp <= 0)) temp =NULL;

	if((ret != NULL && temp != NULL && ret->hp > temp->hp)
		|| (ret == NULL))
		ret = temp;

	temp = getMob(mobs, mobc, x+1, y);
	if(temp != NULL && (temp->k==m.k || temp->hp <=0)) temp =NULL;
	
	if((ret != NULL && temp != NULL && ret->hp > temp->hp)
		|| (ret == NULL))
		ret = temp;

	temp = getMob(mobs, mobc, x, y+1);
	if(temp != NULL && (temp->k==m.k || temp->hp <=0))  temp =NULL;
	
	if((ret != NULL && temp != NULL && ret->hp > temp->hp)
		|| (ret == NULL))
		ret = temp;

	if(ret != NULL && ret->k==m.k) ret=NULL;
	/*if(ret != NULL)
		printf("enemy of: %c(%d,%d) is %c(%d,%d)\n",m.k, x,y, ret->k ,ret->x, ret->y);
	else
		printf("enemy of: %c(%d,%d) is nooone\n",m.k, x,y );*/
	return ret;
}

void PathNodeNeighbors(ASNeighborList neighbors, void *node, void *context)
{
    struct coord *pathNode = (struct coord *)node;
	enum kind mob = (*(enum kind*)(context));
	//printf("looking at: (%d,%d) : %c context: %c\n", pathNode->x, pathNode->y, field[pathNode->y][pathNode->x], mob);
	
	//char buf[12];
	//fgets(buf, 5, stdin);

	if(field[pathNode->y][pathNode->x] != '.') return;

	if(pathNode->x ==0 || pathNode->y==0 || pathNode->y==maxx-1 || pathNode->y==maxy-1) return;

    if (field[pathNode->y-1][pathNode->x] == '.' 
		 || (mob=='G' && field[pathNode->y-1][pathNode->x] == 'E')
		 || (mob=='E' && field[pathNode->y-1][pathNode->x] == 'G')
		) {
        ASNeighborListAdd(neighbors, &(struct coord){pathNode->x, pathNode->y-1}, 1);
    }
    if (field[pathNode->y][pathNode->x-1] == '.'
		 || (mob=='G' && field[pathNode->y][pathNode->x-1] == 'E')
		 || (mob=='E' && field[pathNode->y][pathNode->x-1] == 'G')
		) {
        ASNeighborListAdd(neighbors, &(struct coord){pathNode->x-1, pathNode->y}, 1);
    }
    if (field[pathNode->y][pathNode->x+1] == '.'
		 || (mob=='G' && field[pathNode->y][pathNode->x+1] == 'E')
		 || (mob=='E' && field[pathNode->y][pathNode->x+1] == 'G')
		) {
        ASNeighborListAdd(neighbors, &(struct coord){pathNode->x+1, pathNode->y}, 1);
    }
    if (field[pathNode->y+1][pathNode->x] == '.'
		 || (mob=='G' && field[pathNode->y+1][pathNode->x] == 'E')
		 || (mob=='E' && field[pathNode->y+1][pathNode->x] == 'G')
		) {
        ASNeighborListAdd(neighbors, &(struct coord){pathNode->x, pathNode->y+1}, 1);
    }
}


int mobcmp(const void *p1, const void *p2){
	struct mob *m1 = (struct mob *)p1;
	struct mob *m2 = (struct mob *)p2;
		
	if(m1->y < m2->y) return -1;
	if(m1->y > m2->y) return 1;
	if(m1->x < m2->x) return -1;
	if(m1->x > m2->x) return 1;
	return 0;
}

int main(int argc, char * argv[]){
		
	char *buf = malloc(sizeof(char)*256);

	struct mob *mobs =NULL;
	

	int mobc=0;
	int elfc=0;
	int gobc=0;
	while(fgets(buf, sizeof(char) *256, stdin) != NULL){
		if(strlen(buf) <2) break;
		buf[strlen(buf)-1] = '\0';
		maxx=strlen(buf);
		maxy++;
		field = realloc(field, sizeof(char*)*maxy);
		field[maxy-1] = malloc(sizeof(char)*maxx);
		strcpy(field[maxy-1], buf);
		for(int i =0; i<maxx;i++){
			if(buf[i] == 'G'){
				mobc++;
				mobs = realloc(mobs, sizeof(struct mob)*mobc);
				mobs[mobc-1].x=i;
				mobs[mobc-1].y=maxy-1;
				mobs[mobc-1].hp=200;
				mobs[mobc-1].k=GOBLIN;
				gobc++;
			}else if(buf[i] == 'E'){
				mobc++;
				mobs = realloc(mobs, sizeof(struct mob)*mobc);
				mobs[mobc-1].x=i;
				mobs[mobc-1].y=maxy-1;
				mobs[mobc-1].hp=200;
				mobs[mobc-1].k=ELF;
				elfc++;
			}
		}
	}
	
	for(int y=0;y<maxy;y++){
		printf("%s\n", field[y]);
	}printf("--------------------------\n");

	static const ASPathNodeSource PathNodeSource =
	{
		sizeof(struct coord),
		&PathNodeNeighbors,
		&h,
		NULL,
		&nc
	};
	
	/*struct coord from = {mobs[12].x, mobs[12].y};
	struct coord to = {mobs[8].x, mobs[8].y};
	
	printf("from (%d,%d) to (%d, %d)\n", from.x, from.y, to.x,to.y);
	printf("--------------------------\n");

	char cmob = mobs[12].k;
	*/
	
	//ASPath path = ASPathCreate(&PathNodeSource, &cmob, &from, &to);



	/*if (ASPathGetCount(path) == 0)  printf("reeeeeeeeeee\n"); else{
		 for (int i=0; i<ASPathGetCount(path); i++) {
		    struct coord *pathNode = ASPathGetNode(path, i);
		    
			if(field[pathNode->y][pathNode->x] == '.')
				field[pathNode->y][pathNode->x] = 'x';
		}

		for(int y=0;y<maxy;y++){
			printf("%s\n", field[y]);
		}
	}*/
	int rounds=0;

	ASPath tempp[4];
	int resHP=0;int e=0;
	while(1){
		rounds++;
		for(int i=0;i<mobc;i++){
			if(mobs[i].hp <=0) continue;
			ASPath *paths =NULL;
			int pathc=0;
			for(int j=0;j<mobc;j++){
				if(mobs[j].hp <=0) continue;
				if(mobs[i].k != mobs[j].k){
										
					tempp[0] = ASPathCreate(&PathNodeSource, &(mobs[i].k), &(struct coord){mobs[i].x, mobs[i].y-1}, &(struct coord){mobs[j].x, mobs[j].y});
					tempp[1] = ASPathCreate(&PathNodeSource, &(mobs[i].k), &(struct coord){mobs[i].x-1, mobs[i].y}, &(struct coord){mobs[j].x, mobs[j].y});			
					tempp[2] = ASPathCreate(&PathNodeSource, &(mobs[i].k), &(struct coord){mobs[i].x+1, mobs[i].y}, &(struct coord){mobs[j].x, mobs[j].y});
					tempp[3] = ASPathCreate(&PathNodeSource, &(mobs[i].k), &(struct coord){mobs[i].x, mobs[i].y+1}, &(struct coord){mobs[j].x, mobs[j].y});
					
	
					int lengths[4];
				
					int gt0 =0;
					for(int l=0;l<4;l++){
						if(tempp[l] != NULL){
							lengths[l] = ASPathGetCount(tempp[l]);
							if(lengths[l] > 0) gt0=1;
						}else 		
							lengths[l] = 0;
					}

					if(gt0)
						pathc++;
					else
						continue;

					paths=realloc(paths, sizeof(ASPath)*pathc);	
			
					for(int l=0;l<4;l++){
						if(lengths[l] <= 0) lengths[l] = 999;
					}


					int p =0;
					for(int l=1;l<4;l++){
						if(lengths[p] > lengths[l]) p=l;
					}

					paths[pathc-1]=tempp[p];

				}
			}

			if(pathc ==0){
				//printf("%c (%d,%d) has no enemy\n", mobs[i].k == ELF ? 'E':'G', mobs[i].x, mobs[i].y);
			}else{
				ASPath shortest = paths[0];
				for(int p=1;p<pathc;p++){
					if(ASPathGetCount(paths[p]) < ASPathGetCount(shortest)){
						shortest = paths[p];
					}
				}
				if(ASPathGetCount(shortest) > 0){
					struct coord *cur = ASPathGetNode(shortest, 0);
					//printf("%c (%d,%d) goes to (%d,%d)\n", mobs[i].k == ELF ? 'E':'G', mobs[i].x, mobs[i].y, cur->x, cur->y);
					if(field[cur->y][cur->x] == '.'){
						field[mobs[i].y][mobs[i].x] = '.';
						mobs[i].x = cur->x;
						mobs[i].y = cur->y;
						field[cur->y][cur->x] = mobs[i].k;
					}else{
						//printf("mob %d:%c: (%d, %d) wanted to go to (%d, %d):%c\n", i,mobs[i].k, mobs[i].x, mobs[i].y, cur->x, cur->y, field[cur->y][cur->x]);
					}
				}		
				//attack
			}
			struct mob *n = getMobNeighborWithLeastHP(mobs, mobc, mobs[i]);
			if(n != NULL){
				if(mobs[i].k == ELF)
					n->hp-=25;
				else 
					n->hp-=3;
				if(n->hp <=0){
					field[n->y][n->x]='.';
				}
			}
		}
	
		qsort(mobs, mobc, sizeof(struct mob), mobcmp);

		/*printf("round %d\n", rounds);
		for(int y=0;y<maxy;y++){
			printf("%s\n", field[y]);
		}printf("--------------------------\n");

		for(int i=0;i<mobc;i++){
			if(mobs[i].k == ELF){ 
				printf("elf %d: on (%d,%d) hp: %d\n", i, mobs[i].x, mobs[i].y, mobs[i].hp);
			} 
			if(mobs[i].k == GOBLIN){ 
				printf("goblin %d: on (%d,%d) hp: %d\n", i, mobs[i].x, mobs[i].y, mobs[i].hp);
			} 
		}



		char asd[12]; fgets(asd, 5, stdin);*/
		

		

		e=0;
		int g=0;
		resHP=0;
		for(int i=0;i<mobc;i++){
			if(mobs[i].k == ELF && mobs[i].hp >0){ 
				e++;
				resHP+=mobs[i].hp;
			} 
			if(mobs[i].k == GOBLIN && mobs[i].hp >0){ 
				g++;
				resHP+=mobs[i].hp;
			} 
		}
		if(e == 0 || g==0) break;

	}
	if(e == elfc) printf("yay :D\n");
	/*for(int i=0;i<mobc;i++){
		if(mobs[i].k){ 
			printf("%d: %d\n", i, mobs[i].hp);
		} 
	}*/

	for(int y=0;y<maxy;y++){
		printf("%s\n", field[y]);
	}printf("--------------------------\n");

	printf("end after %d rounds with %d HP left. score:%d\n", rounds,resHP, resHP*rounds);

	free(buf);
	return 0;
}































