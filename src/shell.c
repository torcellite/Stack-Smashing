#include <stdio.h>
#include <stdlib.h>

int main() {
	char *args[2];

	args[0] = "/bin/sh";
	args[1] = NULL;

	execve(args[0], args, NULL);
	
	//exit(0);
	
	return 0;	
}
