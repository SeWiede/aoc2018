#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

int occ(char c, char* cp){
	int occ=0;
	for(int i=0; i< strlen(cp); i++){
		if(cp[i] == c){
			occ++;
			memmove(&cp[i], &cp[i + 1], strlen(cp) - i);
			i--;
		}	
	}
	return occ;
}

void checkMatch(char *s1, char*s2){
	int pos=0;
	for(int i=0,  diff=0; i< strlen(s1); i++){
		if(s1[i] != s2[i]){
			diff++;
			pos=i;
			if(diff >=2)
				return;
		}
	}
	memmove(&s1[pos], &s1[pos + 1], strlen(s1) - pos);
	printf("%s\n", s1);
	exit(0);
}


int main(int argc, char * args[]){
	
	char buf[100];
	int twos =0;
	int threes =0;
	
	char **words =malloc(1);
	int count=0;

	while(fgets(buf, sizeof buf, stdin) != NULL){
		if(strlen(buf) <=1) continue;
		buf[strlen(buf)-1] = '\0';

		count++;
		words = (char**)realloc(words,count*sizeof(char*));
		words[count-1] = (char*)malloc(strlen(buf)*sizeof(char));
		strcpy(words[count-1], buf);

		int t3=0;
		int t2=0;
		for(int i=0; i< strlen(buf)-1; i++){
			int tempocc= occ(buf[i], buf+i+1);
			if(tempocc ==1 &&!t2) t2++;
			if(tempocc ==2 &&!t3) t3++;
		}
		twos+=t2;
		threes+=t3;
	}

	printf("checksum: %d\n", twos*threes);

	for(int i=0; i< count; i++){
		for(int j =i+1; j< count; j++){
			checkMatch(words[i], words[j]);
		}
	}
	return 0;
}































