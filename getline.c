#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>


#define MAX_LENGTH 1024
#define DELIMS " \t\r\n"

int main(int argc, char *argv[]) {
  char *cmd;
  char line[MAX_LENGTH];

  while (1) {
    printf("$ ");
    if (!fgets(line, MAX_LENGTH, stdin)) break;

    // Parse and execute command
    if ((cmd = strtok(line, " \t\r\n"))) {
      // Clear errors
      errno = 0;

      if (strcmp(cmd, "cd") == 0) {
        char *arg = strtok(0, " \t\r\n");

        if (!arg) fprintf(stderr, "cd missing argument.\n");
        else chdir(arg);

      } 
    else system(line);

    }
  }

  return 0;
}