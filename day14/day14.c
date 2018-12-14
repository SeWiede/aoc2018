#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

struct rec{
	uint8_t score;
	struct rec *next;
};

struct rec * insert(struct rec *l, int score){
	struct rec *new = malloc(sizeof(struct rec));
	if(new == NULL) {printf("memory error\n");exit(1);}
	new ->score = score;
	if(l == NULL){
		l  = new;
		new->next=new;
		return new;
	}
	new->next = l->next;
	l->next = new;	
	l=new;
	return l;
}

struct rec *getNext(struct rec *l, int steps){
	struct rec *cur =l;	
	for(int i=0;i<steps;i++)
		cur=cur->next;
	return cur;
}

void printList(struct rec *l){
	struct rec *cur = l->next;
	do{
		printf("%d->", cur->score);
		cur = cur->next;
	}while(cur != l->next);
	printf("\n");
}

int main(int argc, char * argv[]){
		
	char c;

	struct rec *last = NULL;
	int l=0;
	while((c = fgetc(stdin)) != EOF){
		if(c == '\n') break;
		last = insert(last, c-'0');
		l++;	
	}
	//printList(last);
	
	struct rec *ai = last->next;
	struct rec *n1 = last->next;
	struct rec *n2 = n1->next;
	
	int from =	argc > 1 ? strtol(argv[1], NULL, 10) : l;

	for(uint64_t i=0; i< 100000000;i++){
		int ns = n1->score + n2->score;
		if(ns /10 >=1){
			last = insert(last, ns/10);
		}
		last = insert(last, ns%10);
		n1 = getNext(n1, n1->score+1);
		n2 = getNext(n2, n2->score+1);
	}
	//printList(last);
	
	

	for(int i=0; i< from;i++){
		ai=ai->next;
	}
	for(int i=0; i< 10;i++){
		printf("%d", ai->score);
		ai=ai->next;
	}printf("\n");

	ai=last->next;
//	printList(last);

	int i;
	for(i=0;;i++){
		int cf = 1;
		struct rec *c = ai;
		for(int i=0;i<strlen(argv[1]);i++){
			char cc = argv[1][i];
			if(strtol(&(cc), NULL, 10) != c->score ){
				cf =0;
				break;
			}
			c=c->next;
		}
		ai=ai->next;
		if(ai==last) {printf("shit :/ %d\n", i); exit(1);}
		if(!cf) continue;

		break;
	}
	printf("%d\n", i);

	return 0;
}































