#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

struct tree{
	int *m;
	int ms;
	struct tree **c;
	int cc;
		
	int val;
};

typedef struct tree Tree;

uint32_t metasum=0;

void insertTree(Tree **t, char** buf){
	//printf("buf:%s\n", *buf);
	if((*buf)[0] == ' '){(*buf)++;}
	(*t)->cc =strtol(*buf, buf, 10);
	//printf("buf:%s-\n", *buf);
	if((*buf)[0] == ' ') (*buf)++;
	(*t)->ms = strtol(*buf, buf,10);
	//printf("buf:%s\n", *buf);
	(*t)->m = malloc(sizeof(int)*(*t)->ms);
	memset((*t)->m, 0, sizeof(int)*(*t)->ms);
	(*t)->c = malloc(sizeof(Tree *)*(*t)->cc);
	//printf("childs: %d\n", t->cc);
	//if(t->ms <= 0) printf("should not happen!\n");
	for(int i =0;i<(*t)->cc;i++){
		(*t)->c[i]=malloc(sizeof(Tree));memset((*t)->c[i], 0, sizeof(struct tree));
		insertTree(&((*t)->c[i]), buf);
	}
	//printf("meta begin length: %d \n", t->ms);
	//(*buf)++;
	for(int i=0;i<(*t)->ms;i++){
		if((*buf)[0] == ' ') {(*buf)++;}
		int meta = strtol(*buf, buf, 10);
		(*t)->m[i] = meta;
		metasum+=(*t)->m[i];
	
		if((*t)->cc == 0){
			(*t)->val+=meta;
		}else if(meta <=(*t)->cc){
			(*t)->val +=(*t)->c[meta-1]->val;
			//printf("val increase: %d\n", (*t)->val);
		}
		//printf("meta is: %d\n", t->m[i]);
	}
}



int main(int argc, char * args[]){
	
	char *buf = (char *)malloc(2048*8*4*sizeof(char));
	memset(buf, 0, sizeof(char) *2048*8*4);

	Tree **t = NULL;
	int c=0;
	metasum=0;
	while(fgets(buf, sizeof(char)*8*4*2048, stdin) != NULL){
		if(strlen(buf) <= 1) continue;
		char* start=buf;
		if(c>0)printf("buf:%s\n", buf);
		c++;
		t = realloc(t, sizeof(Tree*)*c);
		t[c-1] = malloc(sizeof(Tree));
		memset(t[c-1], 0, sizeof(Tree));
		insertTree(&t[c-1], &buf);			
		
		buf=start;
	}

	printf("metasum: %d\n", metasum);

	printf("val of root: %d\n", t[0]->val);	

	return 0;
}































