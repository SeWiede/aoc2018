#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

#define FS 50

char field[FS][FS];

int cF(int x, int y, char c){
	if(x<0 || y <0 || x>=FS || y>=FS)
		return 0;
	return field[y][x] == c;
}

long long getVal(){
	long t,ya;
	t=ya=0;
	for(int y=0;y<FS;y++){
		for(int x=0;x<FS;x++){	
			t += (field[y][x] == '|' ? 1 :0);	
			ya += (field[y][x] == '#' ? 1 :0);
		}
	}
	return t*ya;
}

int checkReOccurence(long* vals, long valc){
	int range=-1;
	for(long i=0;i<valc;i++){
		for(long j=i+1;j<valc;j++){
			if(vals[i] == vals[j]){
				range=1;
				long startVal = vals[i];
				long k;
				for(k=j+1;k < valc && vals[k] != startVal;k++){
					range++;
				}
				if(k == valc) range=-1; else return range;
			}
		}
	}
	return range;
}

int main(int argc, char * argv[]){
		
	char *buf = malloc(sizeof(char)*256);


	memset(field, 0,sizeof(field));

	int cnt=0;

	while(fgets(buf, 256, stdin) !=NULL){
		if(strlen(buf) < 2) break;	
		char *temp = buf;

		strcpy(field[cnt], buf);
		cnt++;
		buf=temp;
	}

	long *vals=0;
	long valc=0;
	long val=getVal();
	long i=0;

	long range=-1;	

	for(;i < 1000000000;i++){
		char temp[FS][FS];
		memcpy(temp,field,sizeof(field));
		for(int y=0;y<FS;y++){
			for(int x=0;x<FS;x++){
				int c=field[y][x];
				int nL = cF(x+1,y,'#') + cF(x-1, y, '#') +  cF(x, y-1, '#') + cF(x, y+1, '#')
						+ cF(x+1,y+1,'#') + cF(x-1, y-1, '#') +  cF(x+1, y-1, '#') + cF(x-1, y+1, '#');
				int nT = cF(x+1,y,'|') + cF(x-1, y, '|') +  cF(x, y-1, '|') + cF(x, y+1, '|')
						+ cF(x+1,y+1,'|') + cF(x-1, y-1, '|') +  cF(x+1, y-1, '|') + cF(x-1, y+1, '|');
				if(c == '#'){
					temp[y][x] = (nL >=1 && nT >=1) ? '#':'.';
				}
				if(c == '.'){	
					temp[y][x] = (nT >=3) ? '|':'.';
				}
				if(c == '|'){
					temp[y][x] = (nL >=3) ? '#':'|';
					
				}
			}
		}
		memcpy(field,temp,sizeof(field));
		val=getVal();

		valc++;
		vals = realloc(vals, sizeof(long)*valc);
		vals[valc-1] = val;
		if((range = checkReOccurence(vals, valc)) > 0) break;
		//printf("%d: %d %d\n", i+1, diff, val);
	}

	for(int j=400;j< valc;j++){
		//printf("j=%d :%d\n", j,vals[j]);
	}

	printf("range: %lu i=%lu\n", range, i);

	printf("%lu\n", vals[(i-range)+(1000000000-(i+1-range)) %range]);
	free(buf);
	return 0;
}































