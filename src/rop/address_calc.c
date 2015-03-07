#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char **argv) {

  long libc_base;
  long data_base;
  int commands_length;

  char system_nick[128];
  char buffer[128];

  printf("\nEnter system nickname: ");
  fgets(system_nick, sizeof(system_nick), stdin);

  printf("Enter libc base address: ");
  fgets(buffer, sizeof(buffer), stdin);
  sscanf(buffer, "%x", &libc_base);

  printf("Enter .data base address: ");
  fgets(buffer, sizeof(buffer), stdin);
  sscanf(buffer, "%x", &data_base);

  printf("Enter the total number of commands (includes data, padding and addresses): ");
  scanf("%d", &commands_length);
  getchar();

  int i;

  FILE *fp = fopen("rop_address.txt", "a+");

  long offset[commands_length];
  long address[commands_length];

  int type[commands_length];

  char *string;

  i = 0;
  do {
      printf("Enter type (1 - command, 2 - address, 3 - data or padding) and offset of command[%d]: ", i + 1);
      fgets(buffer, sizeof(buffer), stdin);
      type[i] = atoi(strtok(buffer, " "));
      string = strtok(NULL, " ");
      sscanf(string, "%x", &offset[i]);
      printf("Base\t\tOffset\t\tAddress\n");
      switch (type[i]) {
        case 1:
          address[i] = libc_base + offset[i];
          printf("%010p\t%010p\t%010p\n", libc_base, offset[i], address[i]);
          break;
        case 2:
          address[i] = data_base + offset[i];
          printf("%010p\t%010p\t%010p\n", data_base, offset[i], address[i]);
          break;
        default:
          address[i] = offset[i];
          printf("%010p\t%010p\t%010p\n", 0, offset[i], address[i]);
      }
      i++;
  } while (i < commands_length);

  fprintf(fp, "\n\n----START----\n\n");
  fprintf(fp, "System: %s\n\n", argv[3]);
  time_t raw_time = time(NULL);
  fprintf(fp, "Time: %s\n\n", ctime(&raw_time));
  fprintf(fp, "Base\t\tOffset\t\tAddress\n");
  for (i = 0; i < commands_length; i++) {
    switch(type[i]) {
      case 1:
        fprintf(fp, "%010p\t%010p\t%010p\n", libc_base, offset[i], address[i]);
        break;
      case 2:
        fprintf(fp, "%010p\t%010p\t%010p\n", data_base, offset[i], address[i]);
        break;
      default:
        fprintf(fp, "%010p\t%010p\t%010p\n", 0, 0, address[i]);
    }
  }

  fprintf(fp, "\n\n-----END-----\n\n");

  fclose(fp);

  fp = fopen("current_address_space.txt", "w+");
  for (i = 0; i < commands_length; i++)
    fprintf(fp, "%010p\n", address[i]);
  fclose(fp);

  printf("Log saved - Check rop_address.txt\n");
  printf("current_address_space.txt is ready to be used for ROP shellcode creation.\n");

  return 0;
}