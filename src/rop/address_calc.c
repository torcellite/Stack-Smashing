#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char **argv) {

  long base;
  long limit;
  int commands_length;

  // argv[1]: base
  // argv[2]: limit
  // argv[2]: commands_length
  // argv[3]: system nickname

  if (argc < 4) {
      printf("Base address, limit address, number of commands and system nickname need to be entered.\n");
      exit(1);
  }

  sscanf(argv[1], "%x", &base);
  sscanf(argv[2], "%x", &limit);
  commands_length = atoi(argv[3]);

  int i;

  FILE *fp = fopen("rop_address.txt", "a+");

  long offset[commands_length];
  long address[commands_length];

  char string[11]; // example 0xdeadbeef - 10 characters + 1 null

  for (i = 0; i < commands_length; i++) {
      printf("Enter offset for command[%d]: ", i+1);
      scanf("%s", string);
      sscanf(string, "%x", &offset[i]);
      address[i] = base + offset[i];
      printf("Base\t\tOffset\t\tAddress\n");      
      printf("%010p\t%010p\t%010p\n", base, offset[i], address[i]);
      if (address[i] > limit)
        printf("%010p exceeds limit %010p by %010p\n", address[i], limit, address[i] - limit);
  }

  fprintf(fp, "\n\n----START----\n\n");
  fprintf(fp, "System: %s\n\n", argv[3]);
  time_t raw_time = time(NULL);
  fprintf(fp, "Time: %s\n\n", ctime(&raw_time));
  fprintf(fp, "Base\t\tOffset\t\tAddress\n");
  for (i = 0; i < commands_length; i++) {
      fprintf(fp, "%010p\t%010p\t%010p\n", base, offset[i], address[i]);
  }
  fprintf(fp, "\n\n-----END-----\n\n");

  fclose(fp);

  fp = fopen("current_address_space.txt", "w+");
  for (i = 0; i < commands_length; i++)
    fprintf(fp, "%010p\n", address[i]);
  fclose(fp);

  printf("Log saved - Check rop_address.txt\n");

  return 0;
}