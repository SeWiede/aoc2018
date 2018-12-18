#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>


struct instr{
	int bef[4];
	int instr[4];
	int aft[4];
};

uint16_t opcode[16];

int count1(uint16_t x){
	int ret=0;
	while(x != 0){
		ret+= x&1;;
		x>>=1;
	}
	return ret;
}

int pos1(uint16_t x){
	int ret=0;
	while((x & 1)==0){
		ret++;
		x>>=1;
	}
	return ret;
}


int reg[4] = {0};

void addr(int *A, int *B, int *C){
	reg[*C] = reg[*A] + reg[*B];
}

void addi(int *A, int *B, int *C){
	reg[*C] = reg[*A] + *B;
}

void mulr(int *A, int *B, int *C){
	reg[*C] = reg[*A] * reg[*B];
}

void muli(int *A, int *B, int *C){
	reg[*C] = reg[*A] * *B;
}

void banr(int *A, int *B, int *C){
	reg[*C] = reg[*A] & reg[*B];
}

void bani(int *A, int *B, int *C){
	reg[*C] = reg[*A] & *B;
}

void borr(int *A, int *B, int *C){
	reg[*C] = reg[*A] | reg[*B];
}

void bori(int *A, int *B, int *C){
	reg[*C] = reg[*A] | *B;
}

void setr(int *A, int *B, int *C){
	reg[*C] = reg[*A];
}

void seti(int *A, int *B, int *C){
	reg[*C] = *A;
}

void gtir(int *A, int *B, int *C){
	reg[*C] = *A > reg[*B] ? 1: 0;
}

void gtri(int *A, int *B, int *C){
	reg[*C] = reg[*A] > *B ? 1: 0;
}

void gtrr(int *A, int *B, int *C){
	reg[*C] = reg[*A] > reg[*B] ? 1: 0;
}

void eqir(int *A, int *B, int *C){
	reg[*C] = *A == reg[*B] ? 1: 0;
}

void eqri(int *A, int *B, int *C){
	reg[*C] = reg[*A] == *B ? 1: 0;
}

void eqrr(int *A, int *B, int *C){
	reg[*C] = reg[*A] == reg[*B] ? 1: 0;
}


void (*ops[16]) (int *A, int *B, int *C) = {addr, addi, mulr, muli, banr, bani, borr, bori, setr, seti, gtir, gtri, gtrr, eqir, eqri, eqrr};

int checkInstr(struct instr i){
	int ret=0;

	if(i.bef[i.instr[1]] + i.bef[i.instr[2]] == i.aft[i.instr[3]]) ret++;
	else opcode[i.instr[0]] &= 0xfffe;

	if(i.bef[i.instr[1]] + i.instr[2] == i.aft[i.instr[3]]) ret++;
	else opcode[i.instr[0]] &= 0xfffd;

	if(i.bef[i.instr[1]] * i.bef[i.instr[2]] == i.aft[i.instr[3]]) ret++;
	else opcode[i.instr[0]] &= 0xfffb;

	if(i.bef[i.instr[1]] * i.instr[2] == i.aft[i.instr[3]]) ret++;
	else opcode[i.instr[0]] &= 0xfff7;

	if((i.bef[i.instr[1]] & i.bef[i.instr[2]]) == i.aft[i.instr[3]]) ret++;
	else opcode[i.instr[0]] &= 0xffef;

	if((i.bef[i.instr[1]] & i.instr[2]) == i.aft[i.instr[3]]) ret++;
	else opcode[i.instr[0]] &= 0xffdf;

	if((i.bef[i.instr[1]] | i.bef[i.instr[2]]) == i.aft[i.instr[3]]) ret++;
	else opcode[i.instr[0]] &= 0xffbf;

	if((i.bef[i.instr[1]] | i.instr[2]) == i.aft[i.instr[3]]) ret++;
	else opcode[i.instr[0]] &= 0xff7f;

	if(i.bef[i.instr[1]] == i.aft[i.instr[3]]) ret++;
	else opcode[i.instr[0]] &= 0xfeff;

	if(i.instr[1] == i.aft[i.instr[3]]) ret++;
	else opcode[i.instr[0]] &= 0xfdff;

	if(i.instr[1] > i.bef[i.instr[2]] && (i.aft[i.instr[3]] == 1)) ret++;
	else if(i.instr[1] <= i.bef[i.instr[2]] && (i.aft[i.instr[3]] == 0)) ret++;
	else opcode[i.instr[0]] &= 0xfbff;

	if(i.bef[i.instr[1]] > i.instr[2] && (i.aft[i.instr[3]] == 1)) ret++;
	else if(i.bef[i.instr[1]] <= i.instr[2] && (i.aft[i.instr[3]] == 0)) ret++;
	else opcode[i.instr[0]] &= 0xf7ff;

	if(i.bef[i.instr[1]] > i.bef[i.instr[2]] && (i.aft[i.instr[3]] == 1)) ret++;
	else if(i.bef[i.instr[1]] <= i.bef[i.instr[2]] && (i.aft[i.instr[3]] == 0)) ret++;
	else opcode[i.instr[0]] &= 0xefff;

	if(i.instr[1] == i.bef[i.instr[2]] && (i.aft[i.instr[3]] == 1)) ret++;
	else if(i.instr[1] != i.bef[i.instr[2]] && (i.aft[i.instr[3]] == 0)) ret++;
	else opcode[i.instr[0]] &= 0xdfff;

	if(i.bef[i.instr[1]] == i.instr[2] && (i.aft[i.instr[3]] == 1)) ret++;
	else if(i.bef[i.instr[1]] != i.instr[2] && (i.aft[i.instr[3]] == 0)) ret++;
	else opcode[i.instr[0]] &= 0xbfff;

	if(i.bef[i.instr[1]] == i.bef[i.instr[2]] && (i.aft[i.instr[3]] == 1)) ret++;
	else if(i.bef[i.instr[1]] != i.bef[i.instr[2]] && (i.aft[i.instr[3]] == 0)) ret++;
	else opcode[i.instr[0]] &= 0x7fff;

	return ret;
}

int main(int argc, char * argv[]){
		
	char *buf = malloc(sizeof(char)*256);

	struct instr *ins =NULL;
	
	int instrc=0;

	int bl3=0;

	memset(&opcode, 0xff,sizeof(opcode));

	while(1){
		char *temp=buf;
		fgets(buf, 256, stdin);		
		if(strlen(buf) < 2) break;

		instrc++;
		ins = realloc(ins, sizeof(struct instr)*instrc);
		memset(&ins[instrc-1], 0, sizeof(struct instr));
		buf+=9;
		for(int i=0;i<4;i++){
			ins[instrc-1].bef[i] = strtol(buf,&buf, 10);
			buf+=2;
		}
		buf =temp;
		fgets(buf, 256, stdin);	
		for(int i=0;i<4;i++){
			ins[instrc-1].instr[i] = strtol(buf,&buf, 10);
			buf+=1;
		}
		buf =temp;
		fgets(buf, 256, stdin);	
		buf+=9;
		for(int i=0;i<4;i++){
			ins[instrc-1].aft[i] = strtol(buf,&buf, 10);
			buf+=2;
		}
		buf =temp;

		if(checkInstr(ins[instrc-1]) >=3) bl3++;

		fgets(buf, 256, stdin);
	}

	int max=500;

	ele:
	;
	int c=0;
	for(int i=0;i<16 && max >0;i++){
		if(count1(opcode[i]) == 1){
			for(int j=0;j<16;j++){
				if(i != j && (opcode[j] & opcode[i]) != 0){
					opcode[j] -= opcode[i];
					c=1;
				}
			}
		}
	max--;
	}
	if(c) goto ele;


	int mapper[16];

	for(int i=0;i<16;i++){
		mapper[i] = pos1(opcode[i]);
	}


	

	fgets(buf, 256, stdin);
	while(fgets(buf, 256, stdin) !=NULL){
		if(strlen(buf) < 2) break;
		
		char *temp = buf;
		int intr[4] = {0};
		intr[0] = strtol(buf, &buf, 10);
		buf++;
		intr[1] = strtol(buf, &buf, 10);
		buf++;
		intr[2] = strtol(buf, &buf, 10);
		buf++;
		intr[3] = strtol(buf, NULL, 10);

		ops[mapper[intr[0]]](&intr[1], &intr[2], &intr[3]);	

		buf=temp;
	}



	printf("ops that behave like 3: %d\n",  bl3);
	
	printf("A= %d\n", reg[0]);

	free(buf);
	return 0;
}































