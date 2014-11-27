#include <stdio.h>
void Example(int num) {
	char buf1[5];
	char buf2[10];
	int *ret;
	long ebp;
	asm("mov %%rbp, %0\n" : "=r"(ebp));
	ret = ebp + 8; //stack frame pointer seems to be 8 bytes
	(*ret) += 0xA;
	//printf("%x %x\n", ret,*ret);
}

int main(int argc, char **argv) {
	printf("1st print statement\n");
	Example(1);
	printf("Going to skip the 2nd Print <-- this one\n");
	printf("3rd Print\n");
	return 0;
}
