#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

#define FS 300


int main(int argc, char * args[]){

int max = -999;
int maxx =-1;
int maxy=-1;
int maxs=-1;
	for(int y=1; y < FS;y++){
		for(int x=1;x < FS;x++){
			for(int s=10;s<FS;s++){
				if(x+s-1 >= FS || y+s-1 >= FS) continue;	
				int *rackID = malloc(s*sizeof(int));
				for(int i=0;i<s;i++){
					rackID[i] = x+i+10;
				}
				// = {x+10, x+1+10, x+2+10};
				int *powLev = malloc(s*s*sizeof(int));
				int c =-1;
				for(int i=0;i<s*s;i++){
					if(i%s==0) c++;
					powLev[i] =rackID[i%s]*(y+(c%s));
				}
				//= {rackID[0]*y, rackID[1]*y, rackID[2]*y, rackID[0]*(y+1), rackID[1]*(y+1), rackID[2]*(y+1), rackID[0]*(y+2), rackID[1]*(y+2), rackID[2]*(y+2)};
				int tempsum=0;
				c=-1;
				for(int i =0;i<s*s;i++){
					powLev[i]=(powLev[i]+8444)*rackID[i%s];
					powLev[i]/=100;
					powLev[i]%=10;
					powLev[i]-=5;
					tempsum+=powLev[i];
				}
				free(rackID);
				free(powLev);
				if(tempsum > max) {max = tempsum;maxx=x;maxy=y;maxs=s;
				
	printf("max is: %d x:%d y:%d s:%d\n", max, maxx, maxy, maxs);	
			}
			}
		}
	}


	return 0;
}































