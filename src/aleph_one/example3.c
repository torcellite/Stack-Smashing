#include <stdio.h>
void function(int a, int b, int c) {
	char buffer1[5];
	int *ret;
	ret = buffer1 + 24;
	(*ret) += 7;
}

void main() {
	int x;
	x = 0;
	function(1, 2, 3);
	x = 1;
	printf("%d\n", x);
}
