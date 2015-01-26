#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {

	// this line is to check if the shellcode has any CR, LF characters
	// printf("Number of arguments: %d\n", argc);
	char buffer[80];
	int i;
	for (i = 1; i < argc; i++) {
		// printf("%s\nLength: %d\n", argv[i], strlen(argv[i]));
	}
	strcpy(buffer, argv[1]);
	printf("%x\n", &buffer);
	printf("I haven't been hacked yet. Going to exit program normally.\n");
	return 0;
}
