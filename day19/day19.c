#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>


#define REGCOUNT 6
#define OPCODES 16

uint16_t opcode[OPCODES];

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


int reg[REGCOUNT] = {0};

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

void divr(int *A, int *B, int *C){
	if(reg[*B] != 0)
		reg[*C] = reg[*A] / reg[*B];
	else{
		reg[*C] = 0;
	}
}

void nop(int *A, int *B, int *C){

}



void (*ops[OPCODES+2]) (int *A, int *B, int *C) = {addr, addi, mulr, muli, banr, bani, borr, bori, setr, seti, gtir, gtri, gtrr, eqir, eqri, eqrr, divr,nop};

int getOpcode(const char* op){
	if(strcmp(op, "addr") == 0){
		return 0;
	}
	if(strcmp(op, "addi") == 0){
		return 1;
	}
	if(strcmp(op, "mulr") == 0){
		return 2;
	}
	if(strcmp(op, "muli") == 0){
		return 3;
	}
	if(strcmp(op, "banr") == 0){
		return 4;
	}
	if(strcmp(op, "bani") == 0){
		return 5;
	}
	if(strcmp(op, "borr") == 0){
		return 6;
	}
	if(strcmp(op, "bori") == 0){
		return 7;
	}
	if(strcmp(op, "setr") == 0){
		return 8;
	}
	if(strcmp(op, "seti") == 0){
		return 9;
	}
	if(strcmp(op, "gtir") == 0){
		return 10;
	}
	if(strcmp(op, "gtri") == 0){
		return 11;
	}
	if(strcmp(op, "gtrr") == 0){
		return 12;
	}
	if(strcmp(op, "eqir") == 0){
		return 13;
	}
	if(strcmp(op, "eqri") == 0){
		return 14;
	}
	if(strcmp(op, "eqrr") == 0){
		return 15;
	}
	if(strcmp(op, "divr") == 0){
		return 16;
	}
	if(strcmp(op, "nopp") == 0){
		return 17;
	}
	return -1;
}


int main(int argc, char * argv[]){
		
	char *buf = malloc(sizeof(char)*256);

	int ipreg=-1;
	int **instr =NULL;
	int instrc=0;
	
	char **istr =NULL;

	while(fgets(buf, 256, stdin) !=NULL){
		if(strlen(buf) < 2) continue;
		
		char *temp = buf;

		if(buf[0] == '#'){
			ipreg = strtol(buf+4, NULL, 10);
		}else if(buf[0] != ' '){
			instrc++;
			
			istr = realloc(istr, sizeof(char*)*instrc);
			istr[instrc-1] = malloc(sizeof(char)*strlen(buf)+1);			
			strncpy(istr[instrc-1], buf, strlen(buf));
			istr[instrc-1][strlen(buf)-1] = '\0';
			int intr[4] = {0};
			char op[4];
			strncpy(op, buf, 4);
			buf+=5;
			intr[0] = getOpcode(op);
			intr[1] = strtol(buf, &buf, 10);
			buf++;
			intr[2] = strtol(buf, &buf, 10);
			buf++;
			intr[3] = strtol(buf, NULL, 10);

			instr = realloc(instr, sizeof(int*)*instrc);
			instr[instrc-1] = malloc(sizeof(int)*4);
			memcpy(instr[instrc-1], intr, sizeof(intr));		
		}
		buf=temp;
	}
	memset(reg, 0, sizeof(reg));

	reg[0] =1;	

	while(reg[ipreg] < instrc){
		int intr[4];
		memcpy(intr, instr[reg[ipreg]], sizeof(intr));
		ops[intr[0]](&intr[1], &intr[2], &intr[3]);
		reg[ipreg]++;
	}

	printf("A= %d\n", reg[0]);

	free(buf);
	return 0;
}































