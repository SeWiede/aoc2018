#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

struct list{
	int part;
	struct list *next;
};

typedef struct list list;

void insert(list *l, int n){
	list *current = l;
	list *insert;

	while(current->next != NULL && current->next->part < n){
		current = current->next;
	}

	if(current->next == NULL){
		current->next = (list*)malloc(sizeof(struct list));
		current->next->part = n;
		current->next->next=NULL;
	}else{
		list *t = (list*)malloc(sizeof(struct list));
		t->part = n;	
		t->next = current->next;
		current->next = t;
	}	
}

void printList(list *l){
	while(l != NULL){
		printf("%d -> ", l->part);l=l->next;
	}
	printf("\n");
}

int checkTwice(list *l, int sum){
	while(l != NULL){
		if(l->part == sum)
			return 1;
		if(l->part > sum){
			return 0;
		}
		l= l->next;		
	}
	return 0;
}

int main(int argc, char * args[]){
	char buf[100];
	int current;
	int *nums = (int*)malloc(sizeof(int*));
	int c =1;
	fgets(buf, sizeof buf, stdin);
	list l; l.part = strtol(buf, NULL, 10); l.next=NULL;
	int sum =l.part;
	nums[0] = sum;
	while(fgets(buf, sizeof buf, stdin) != NULL){
		//printf("%d\n", c);
		current = strtol(buf, NULL, 10);
		sum+=current;
		insert(&l, sum);
		c++;
		nums = realloc(nums, c*sizeof(int*));
		nums[c-1] = current;
	}

	printf("sum is: %d\n", sum);
	for(int j =0; j<c; j++){
		for(int i = 0; i <c; i++){
			sum+=nums[i]; 
			if(checkTwice(&l, sum)){
				printf("first double freq: %d\n", sum);
				return 0;
			}
		}
	}
	free(nums);
	return 0;
}































