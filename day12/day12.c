#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

#define FS 300

int calcVal(char* state){
	int sum=0;
	for(int i=0;i<strlen(state);i++){
		if(state[i] == '#'){
			sum+= i-6;
		}
	}
	return sum;
}

int main(int argc, char * args[]){
		
	char * buf = malloc(sizeof(char)*256);

	fgets(buf, 256*sizeof(char),stdin);
	memmove(buf, buf+9, strlen(buf));
	buf[0]='.'; buf[1]='.';buf[2]='.'; buf[3]='.';buf[5]='.'; buf[4]='.';
	buf[strlen(buf)-1] ='\0';
	//memset(buf+strlen(buf)-1, '.', 20);
		
	
	char *state = malloc(strlen(buf)*sizeof(char)+1);
	strcpy(state, buf);

	char **rules = NULL;
	int rc =0;
	while(fgets(buf, 256*sizeof(char),stdin) != NULL){
		if(strlen(buf) <2)continue;
		rc++;
		rules = realloc(rules, sizeof(char*)*rc);
		rules[rc-1]=malloc(strlen(buf)+1);
		strcpy(rules[rc-1], buf);
		//printf("%s", rules[rc-1]);
	}
	printf("     init: %s\n", state);	
	int lastval=0;
	for(int l =0;l<1000;l++){
		state= realloc(state, strlen(state)+3);
		strcat(state, "....");

		char *copy = malloc(strlen(state)*sizeof(char)+1);
		strcpy(copy, state);
		for(int i =2;i<strlen(copy);i++){
			for(int r=0;r<rc;r++){
				if(copy[i]==rules[r][2] 
					&& copy[i-1]==rules[r][1] 
					&&copy[i-2]==rules[r][0] 
					&&copy[i+1]==rules[r][3] 
					&&copy[i+2]==rules[r][4]){
						
					//memset(state+i-2, '.',5);
					state[i] = rules[r][9];
				} else{
					//state[i] = '.';
				} 
			}
		}
		free(copy);
		int current=calcVal(state);
		printf("state: %d diff: %d\n", l+1/*,state, current,*/, current-lastval);
		lastval=current;
	}
	//constant change after state 163
	int sum=calcVal(state);

		
	printf("sum is: %d\n",sum);

	return 0;
}































