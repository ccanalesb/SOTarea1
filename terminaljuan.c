#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define MAXLINE 256

int main(void)
{
  char buf[MAXLINE];
  pid_t pid;
  int status;
  int i, num_tok;
  char **tok;
  int in, out, inout, bg;
  int fd[2], fdin, fdout;

  printf("%% ");  /* print prompt (printf requires %% to print %) */

  while (fgets(buf, MAXLINE, stdin) != NULL) {
    buf[i = (strlen(buf) - 1)] = 0;       /* replace newline with null */

    num_tok = 1;
    while (i)
      if (buf[i--] == ' ')
        num_tok++;

    if (!(tok = calloc(num_tok + 1, sizeof (char *)))) {
      fprintf(stderr, "calloc failed");
      exit(1);
    }

    i = in = out = inout = bg = 0;
    tok[i++] = strtok(buf, " ");
    while ((tok[i] = strtok(NULL, " "))) {
      switch (*tok[i]) {
      case '<':
        in = i;
        tok[i] = NULL;
        break;
      case '>':
        out = i;
        tok[i] = NULL;
        break;
      case '|':
        inout = i;
        tok[i] = NULL;
        break;
      case '&':
        bg = i;
        tok[i] = NULL;
        break;
      }
      i++;
    }

    if (in && inout || out && inout) {
      fprintf(stderr, "Not implemented\n");
      continue;
    }

    if ((pid = fork()) < 0) {             /* for a process                  */
      perror("fork error");   
      exit(1);
    } else if (!pid) {                    /* child                          */
      if (in) {
        if (-1 == (fdin = open(tok[in + 1], O_RDONLY))) { /* open file      */
          perror("open failed");
          exit(1);
        }
        dup2(fdin, STDIN_FILENO);         /* set it as standard input       */
        if (!out) {
          if (execvp(*tok, tok))          /* and exec                       */
            perror("execvp failed");
        }
      }
      if (out) {
        if (-1 == (fdout = open(tok[out + 1], O_WRONLY | O_CREAT, 0600))) {
          perror("open failed");          /* same idea as in                */
          exit(1);
        }	  
        dup2(fdout, STDOUT_FILENO);
        if (execvp(*tok, tok))
          perror("execvp failed");
      }
      if (inout) {
        if (-1 == pipe(fd)) {
          perror("pipe error");
          exit(1);
        }
        if ((pid = fork()) < 0) {         /* starting 2 new processes, so   */
          perror("fork error");           /* we fork again                  */
          exit(1);
        } else if (pid) {                 /* doesn't matter which is which  */
          if (-1 == dup2(fd[1], STDOUT_FILENO)) {
            perror("dup2 error");         /* make stdout the pipe           */
            exit(1);
        }
          close(fd[0]);
          if (execvp(*tok, tok)) {        /* and start first program        */
            perror("execvp failed");
            exit(1);
          }
        } else {
          if (-1 == dup2(fd[0], STDIN_FILENO)) { /* same idea               */
            perror("dup2 error");
            exit(1);
          }
          close(fd[1]);
          if (execvp(tok[inout + 1], &tok[inout + 1])) {
            perror("execvp failed");
            exit(1);
          }
        }
      }
      if (!in && !out && !inout) {
        if (execvp(*tok, tok)) {
          perror("execvp failed");
          exit(1);
        }
      }
    } else if (pid && !bg)
      waitpid(pid, &status, 0);           /* not bg, wait                   */
    /* else it is a background job!  Don't wait! */

    free(tok);                            /* clean up dynamic memory!       */

    printf("%% ");
  }
  exit(0);
}