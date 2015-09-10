#include <unistd.h>
#include <stdio.h>
struct command
{
  const char **argv;
};

int spawn_proc (int in, int out, struct command *cmd){
  pid_t pid;
  if ((pid = fork ()) == 0){
    if (in != 0){
        dup2 (in, 0);
        close (in);
      }
    if (out != 1){
        dup2 (out, 1);
        close (out);
      }
    return execvp (cmd->argv [0], (char * const *)cmd->argv);
  }
  return pid;
}

int fork_pipes (int n, struct command *cmd)
{
  int i;
  pid_t pid;
  int in, fd [2];
  in = 0;
  for (i = 0; i < n - 1; ++i)
    {
      pipe (fd);
      spawn_proc (in, fd [1], cmd + i);
      close (fd [1]);
      in = fd [0];
    }
  if (in != 0)
    dup2 (in, 0);
  return execvp (cmd [i].argv [0], (char * const *)cmd [i].argv);
}
int main ()
{

  char *ls[] = { "ls", "-l", 0 };
  char *awk[] = { "sort", 0 };
  char *sort[] = { "pwd", 0 };
  
  // int i;
  // for ( i = 0; i < 2; ++i)
  // {
  //   printf("%s\n", ls[i]);
  // }
  struct command cmd [] = { {ls}, {sort},{awk}};

  return fork_pipes (4, cmd);
}