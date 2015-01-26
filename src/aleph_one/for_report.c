void function(char *str) {
	char buffer[16];
	strcpy(buffer, str);
}

void main() {
	char large_string[15];
	int i;
	scanf("%s", large_string);
	for(i = 0; i < 15; i++)
		large_string[i] = 'A';
	function(large_string);
}
