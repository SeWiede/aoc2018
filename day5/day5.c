#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>


int react(char *buf, char a){
	int s= strlen(buf);
	char *temp = malloc(s*sizeof(char));
	strcpy(temp, buf);
	
	//printf("%s\n", temp);
	//printf("deleting all %c/%c\n", a,a+32);
	for(int i =0;i<s;i++){
		if(temp[i] == a || temp[i] == a+32){
			memmove(&temp[i], &temp[i + 1], s- i);
			s--;
			i--;
		}
	}	
	//printf("%s\n", temp);

	while(1){
		int change =0;
		for(int i =0;i<s;i++){
			if(i+1 < s && (temp[i] == temp[i+1]+32 || temp[i]+32 == temp[i+1])){
				memmove(&temp[i], &temp[i + 2], s- i-1);
				s-=2;
				i--;
				change=1;
			}
		}
		if(!change) break;
	}
	free(temp);
	return s;
}

int main(int argc, char * args[]){
	
	char *buf = NULL;
	int s = 0;
	char c;
	while((c = fgetc(stdin)) != EOF){
		if(c == '\n') break;
		s++;
		buf = realloc(buf, sizeof(char)*s+1);
		strncat(buf, &c, 1);
	}
	
	int min=999999999;
	for(int i =0;i<26;i++){
		int current = react(buf, 'A'+i);
		if(current < min) min = current;
	}

	printf("%d\n",  min);
	free(buf);	
	return 0;
}
























