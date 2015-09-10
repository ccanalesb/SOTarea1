#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

void in_file(char **cmd,int in){
  int fdin = open(cmd[in + 1], O_RDONLY);
  dup2(fdin, STDIN_FILENO);   
}


void out_file(char **cmd, int out){
  int fdout = open(cmd[out + 1],O_WRONLY);
  dup2(fdout, STDOUT_FILENO);
  if (execvp(*cmd, cmd) < 0) {    /*Se ejecuta el comando a través de la llamada a sistema, 
                    en caso de error, es decir, que no sea un comando de sistema, 
                    se ejecuta el siguiente error */
    printf("Comando Inválido\n");
  }
  else{
    printf("System error\n"); //cualquier otro error, se ve reflejado acá
  }
}

void pipe_in(char **cmd, int plec,int pid){
  /*Proceso dentro del proceso, se  necesita generar un proceso para pasarle los datos
                                de uno al otro */
  int fd[2];
  if ((pid = fork()) < 0) 
  {
    printf("mayday, mayday, call 911\n");
    exit(1);
  } 

  else if (pid) 
  {                 
    dup2(fd[1], 1);
    close(fd[0]);
    execvp(*cmd, cmd);
  } 
  else {
    dup2(fd[0], 0);
    close(fd[1]);
    execvp(cmd[plec + 1], &cmd[plec + 1]);
  }
}
int main(void)
{
  char buf[1000];
  int pid;
  int status;
  int i, pos;
  char **cmd;
  int in, out, plec, amp,ext;
  

  printf(">>");  /* print prompt (printf requires %% to print %) */

  while (fgets(buf, 1000, stdin) != NULL) {
    i = (strlen(buf) - 1);
    buf[i] = 0;       /* replace newline with null */
    int j;
    for ( j = 0; buf[j]!= '\0'; ++j)
    {
      printf("%c", buf[j]);
    }
    printf("\n");
    
    pos = 1;
    while (i)
      if (buf[i--] == ' ')
        pos++;
    cmd = calloc(pos + 1, sizeof (char *));
    i = in = out = plec = amp = 0; //se resetean los valores en cada iteración

    cmd[i++] = strtok(buf, " ");
    /*se recorre el arreglo hasta encontrar un esacio en blanco
      buscando por cada uno de los tipos,
      Si se encuentra de alguno de los tipos, se guarda la posición en la
      que se encontró */

    while ((cmd[i] = strtok(NULL, " "))) {  
      if(strcmp(cmd[i],"<")==0){
        in = i;
        cmd[i] = NULL;
      }
      else if (strcmp(cmd[i],">")==0){
        out = i;
        cmd[i] = NULL;
      }
      else if (strcmp(cmd[i],"|")==0){
        plec = i;
        cmd[i] = NULL;
      }
      else if (strcmp(cmd[i],"&&")==0){
        amp = i;
        cmd[i] = NULL;
      }
      else if (strcmp(cmd[i],"exit")==0)
      {
        ext= i;
        cmd[i] = NULL;
      }
      i++;
    }

    /*Se compruebas los tipos de datos que se leyeron más arriba, comprobando que se tiene que hacer para cada una de las
    entradas distintas*/
    if ((pid = fork()) < 0) 
    {
      printf("mayday, mayday, call 911\n");
      exit(1);
    } 
    else if (pid==0) {                   
      if (in) {
        in_file(cmd,in);
      }

      if (out) {
        out_file(cmd,out);
      }
      if (plec) {
        pipe_in(cmd,plec,pid);
      }
      if (ext)
      {
        exit(0);
      }
      if (!in && !out && !plec && !ext) { 
        if (execvp(*cmd, cmd)) {
          printf("Error en Comando");
          exit(1);
        }
      }
    } 
    else if (pid && !amp)
    waitpid(pid, &status, 0);          

    free(cmd);                            

    printf(">>");
  }
  exit(0);
}