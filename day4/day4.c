#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

enum  guard_action{
	BEGIN,
	ASLEEP,
	WAKES
};

struct guardinfo{
	uint16_t year;
	uint16_t month;
	uint16_t day;

	uint16_t hour;
	uint16_t minute;

	int gnr;
	enum guard_action action;
};

typedef struct guardinfo guardinfo;


struct gL{
	int sleeps[60];
	int gnr;
	struct gL *next;
};


int cmpG(const void *p1, const void *p2){
	if(((guardinfo*)p1)->month > ((guardinfo*)p2)->month){
		return 1;
	}else if(((guardinfo*)p1)->month < ((guardinfo*)p2)->month){
		return 0;	
	}
	if(((guardinfo*)p1)->day > ((guardinfo*)p2)->day){
		return 1;
	}else if(((guardinfo*)p1)->day < ((guardinfo*)p2)->day){
		return 0;
	}
	if(((guardinfo*)p1)->hour > ((guardinfo*)p2)->hour){
		return 1;
	}else if(((guardinfo*)p1)->hour < ((guardinfo*)p2)->hour){
		return 0;
	}
	if(((guardinfo*)p1)->minute > ((guardinfo*)p2)->minute){
		return 1;
	}else if(((guardinfo*)p1)->minute < ((guardinfo*)p2)->minute){
		return 0;
	}
	return 0;
}

void insert(struct gL *l, int gnr, int amin, int wmin){
	struct gL *current = l;
	
	while(current -> next != NULL && current ->gnr != gnr){
		current = current->next;
	}

	if(current->gnr != gnr){
		struct gL *new = malloc(sizeof(struct gL));
		memset(new->sleeps, 0, sizeof(new->sleeps));
		new->gnr = gnr;
		current->next = new;
		new->next=NULL;
		current = current->next;
	}
	for(int i =amin;i<wmin;i++){
		current->sleeps[i]++;
	}
}

int main(int argc, char * args[]){
	
	char *buf = (char *)malloc(120*sizeof(char));

	guardinfo *gi = NULL;
	int count=0;

	uint16_t currentGuard =0;

	

	while(fgets(buf, sizeof(char)*120, stdin) != NULL){
		if(strlen(buf) <= 1) continue;
		char* start=buf;
				
		count++;
		gi = realloc(gi, count*sizeof(struct guardinfo));

		if(gi==NULL){printf("reee\n");exit(1);}

		buf++;
		gi[count-1].year = strtol(buf,&buf,10);
		buf++;
		gi[count-1].month = strtol(buf,&buf,10);
		buf++;
		gi[count-1].day = strtol(buf,&buf,10);		
		buf++;
		gi[count-1].hour = strtol(buf,&buf,10);
		buf++;
		gi[count-1].minute = strtol(buf,&buf,10);
		

		buf+=2;

		char temp[6]; strncpy(temp, buf, 6);
		temp[5] = '\0';
		buf+=6;	
		if(strcmp(temp, "Guard") == 0){
			buf++;
			currentGuard = strtol(buf, &buf, 10);	
			gi[count-1].action = BEGIN;
		}else if(strcmp(temp, "falls") == 0){
			gi[count-1].action = ASLEEP;
		}else if(strcmp(temp, "wakes") == 0){
			gi[count-1].action = WAKES;
		}else{
			printf("uncaught string: %s\n", temp);
		}

		gi[count-1].gnr = currentGuard;
		
				
		buf=start;
	}
	//printf("---------\n");
	qsort(gi, count,sizeof(guardinfo), cmpG);

	int cg=-1;
	int cs=-1;

	struct gL *l = malloc(sizeof(struct gL)); 
	l->gnr =-1;
	l->next = NULL;
	memset(l->sleeps, 0, sizeof(l->sleeps));
	for(int i =0; i< count; i++){
		if(gi[i].action == BEGIN){
			if(cs != -1){
				insert(l, cg, cs, 60);
				cs=-1;
			}
			cg = gi[i].gnr;
		}
		if(gi[i].action == ASLEEP){
			if(cs != -1){
				insert(l, cg, cs, 60);
			}
			cs = gi[i].minute;
		}
		if(gi[i].action == WAKES){
			insert(l, cg, cs, gi[i].minute);
			cs =-1;
		}
		
	}

	cg =-1;
	cs=-1;	
	
	struct gL *chosen;

	struct gL *c = l;

	int maxmin=-1;

	while(c  != NULL){
		for(int i=0;i<60;i++){
			if(cs < c->sleeps[i]){
				maxmin = i;
				cs = c->sleeps[i];
				cg= c->gnr;
				chosen =c;	
			}
		}
		c=c->next;
	}
	
	printf("guard %d * min %d = %d\n", cg, maxmin, chosen->gnr* maxmin);

	return 0;
}































