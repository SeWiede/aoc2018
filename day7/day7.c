#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

struct dep{
	char *deps;
	char rep;
	int depc;
	int length;
};

void del(struct dep ***d, int c){
	for(int i=0;;i++);
}

int main(int argc, char * args[]){
	
	char *buf = (char *)malloc(128*sizeof(char));
	memset(buf, 0, sizeof(char) *128);

	struct dep **dep = NULL;	
	char c=26;
	dep= malloc(sizeof(struct dep)*c);

	for(int i =0;i< c;i++){
		dep[i] = malloc(sizeof(struct dep));
		dep[i]->depc = 0;
		dep[i]->deps = malloc(1*sizeof(char));	
		dep[i]->deps[0] ='\0';
		dep[i]->rep = i+'A';
		dep[i]->length = 60+i+1;
	}


	while(fgets(buf, sizeof(char)*128, stdin) != NULL){
		if(strlen(buf) <2) break;
		char w = buf[36];
		char on = buf[5];
		
		dep[w-'A']->depc++;
		dep[w-'A']->deps = realloc(dep[w-'A']->deps, sizeof(char)*((dep[w-'A']->depc)+1));
		char * temp= malloc(2); temp[0] = on; temp[1] = '\0';
		strcat(dep[w-'A']->deps, temp); free(temp);
		//printf("index: %d- %c on %s\n",w-'A', w, dep[w-'A']->deps);
	}
	//printf("dep[0] is: %s\n", dep[0] == NULL ? "NULL?!?!?": "nice");
	char out[50];
	int tick = -1;
	struct dep *(worker[5]);
	int wc = 5;

	for(int i =0;i<wc;i++){
		worker[i] = NULL;	
	}	

	while(1){
		tick+=1;
		int change=0;
		for(int j=0;j<c;j++){
			if(dep[j]->depc ==0){
				int occup=1;
				for(int w =0;w<wc;w++){
					if(worker[w] == NULL){
						occup=0;
						worker[w] = dep[j]; 
						dep[j]->depc--;
						break;
					}
				}
				if(occup) break;

				
			}
		}
		printf("tick %d: ", tick);
		for(int w =0;w<wc;w++){
			if(worker[w] == NULL) continue;
		printf("%c ", worker[w]->rep);
			worker[w]->length--;
			change =1;
			if(worker[w]->length == 0){		
				strcat(out, &(worker[w]->rep));
				worker[w]->depc--;
				for(int i=0;i<c;i++){
					if(worker[w]->rep != dep[i]->rep && dep[i]->depc > 0&& strchr(dep[i]->deps, worker[w]->rep)){
						dep[i]->depc--;	
					}
				}
				worker[w] = NULL;
			}
		}
		printf("\n");
		if(!change) break;
	}
	printf("order is %s\n", out);
	return 0;
}





/*
//printf("dep %c has deps: \"%s\" count %d\n", dep[j]->rep, dep[j]->deps, dep[j]->depc);
			if(dep[j]->depc ==0 && !strchr(out, dep[j]->rep)){
				//printf("%c has no dep!\n", j+'A');
				
				strcat(out, &(dep[j]->rep));
				for(int i=0;i<c;i++){
					if(i != j && dep[i]->depc > 0&& strchr(dep[i]->deps, dep[j]->rep)){
						dep[i]->depc--;	
						change=1;	
					}
				}
			}
			if(change) goto start;



*/

























