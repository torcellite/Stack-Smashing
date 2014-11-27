char code[] =   "\xb8\x3c\x00\x00\x00"
		"\x0f\x05"
		"\xb8\xe7\x00\x00\x00"
		"\x0f\x05"; //exit(0) shellcode
/*char code[] = "\x48\xc7\xc7\xe4\x00\x60\x00"
		"\x48\xc7\xc1\xff\xff\xff\xff"
		"\x30\xc0"
		"\xfc"
		"\xf2\xae"
		"\x48\xc7\xc2\xfe\xff\xff\xff"
		"\x48\x29\xca"
		"\x48\xc7\xc6\xe4\x00\x60\x00"
		"\x6a\x01"
		"\x58"
		"\x48\x89\xc7"
		"\x0f\x05"
		"\x48\x31\xff"
		"\x6a\x3c"
		"\x58"
		"\x0f\x05";*/ //print "Hello World!" shellcode

void main() {
	(*(void(*)())code)(); //not sure how this works
	//figure out the return address correctly
	/*int *ret;
	ret = (int *)&ret + 15; //8 for integer pointer and 7 for slack
	(*ret) = (int) code;*/
//	return 0;
}
