#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>


struct marbles{
	int val;
	struct marbles *prev;
	struct marbles *next;
};


struct marbles * insert(struct marbles **m, int val){
	struct marbles *n = malloc(sizeof(struct marbles));
	struct marbles *i = (*m)->next;
	i->next->prev = n;
	n->val=val;
	n->next=i->next;
	i->next = n;
	n->prev=i;
	return n;
}

int delete(struct marbles **m){
	struct marbles *d = (*m)->prev->prev->prev->prev->prev->prev->prev;
	d->prev->next=d->next;
	d->next->prev = d->prev;
	*(m)=d->next;
	return d->val;
}

void printList(struct marbles *m){
	struct marbles *c=m;
	do{
		printf("%d ", c->val);
		c=c->next;
	}while(c!=m);
	printf("\n");
}

int main(int argc, char * args[]){
	
	int players = 439;
	long long playerscore[439];memset(playerscore, 0, sizeof(long long)*439);
	int lastms = 71307;

	struct marbles *m = malloc(sizeof(struct marbles));
	m->val=0;
	struct marbles *first =malloc(sizeof(struct marbles));
	first->val=1;
	m->prev=first;
	m->next=first;
	first->next=m;
	first->prev=m;
	int cp=0;
	int cm=1;
	m=first;
	printList(m);
	for(int rounds =0;;rounds++){
		cp++; cp%=players;
		cm++;
		if(cm %23 ==0){
			int ns =cm+delete(&m);
			playerscore[cp] +=ns;
		}else{
			m=insert(&m, cm);
		}
		if(cm == lastms*100)break;
		//printList(m);
		//char temp[123];
		//fgets(temp, 123, stdin);
	}
	long long max =-1;
	for(int i =0;i<players;i++){
		//printf("s: %d\n", playerscore[i]);
		if(playerscore[i] >max){
			max= playerscore[i];
		}
	}
	printf("max is: %lld\n", max);	
	return 0;
}































