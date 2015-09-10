#include <stdio.h> // Maneja I/O en sistema
#include <unistd.h> //Maneja las llamadas a sistema
#include <string.h>

void  parse(char *line, char **argv)
{
     while (*line != '\0') {   
          while (*line == ' ' || *line == '\t' || *line == '\n')
               *line++ = '\0';     /* replace white spaces with 0    */
          *argv++ = line;          /* save the argument position     */
          while (*line != '\0' && *line != ' ' && 
                 *line != '\t' && *line != '\n') 
               line++;             /* skip the argument until ...    */
     }
     *argv = '\0';                 /* mark the end of argument list  */
}


struct Types
{
	int pos;
	int type; /* si type es 1 => "&&"
				 si type es 2 = > "|"
				 si type es 3 = > "<"
				 si type es 4 = > ">  */
};


int main()
{
	char  cmd[1000];  //Linea donde se guardará el input del usuario           
	char  *argv[20];  //guardará las posiciones de los argumentos
	struct Types types[50];
	char *cmds[20];
	
	int size=0;
	// printf("%s\n", pipe);
	// la consola funciona con todas las llamadas de sistema, pero acá muestro algunas.
	printf("Esta consola acepta los siguientes comandos\n");
	printf("\tls \n");
	printf("\tpwd\n");
	printf("\tnano\n");
	printf("\tdate\n");
	printf("Para más información del comando, ejecute: man <comando>\n");

	while(1){

		printf("\n>> "); //solo para que se vea más lindo.
		fgets(cmd,sizeof(cmd),stdin); // lee la linea de entrada por stdin
		size_t len = strlen(cmd) -1;
		if (cmd[len]=='\n')
		{
			cmd[len]='\0'; //se elimina el ultimo caracter del fgets
		}
	
		int i;
		
		parse(cmd, argv); //Divide la entrada, entre el comando y los argumentos
		for ( i = 0; argv[i]!='\0'; i++)
		{
			if (strcmp(argv[i],"&&")==0)
			{
				types[size].pos=i;
				types[size].type=1;
				size++;	
			}
			else if (strcmp(argv[i],"|")==0)
			{
				types[size].pos=i;
				types[size].type=2;
				size++;	
			}
			else if (strcmp(argv[i],">")==0 | strcmp(argv[i],">>")==0)
			{
				types[size].pos=i;
				types[size].type=4;
				size++;	
			}
			else if (strcmp(argv[i],"<")==0 | strcmp(argv[i],"<<")==0)
			{
				types[size].pos=i;
				types[size].type=3;
				size++;	
			}
		}
		for ( i = 0; i < size; i++)
		{
			if (types[i].type==1)
			{
				printf("%d - &&\n",types[i].pos);
			}
			else if (types[i].type==2)
			{
				printf("%d - |\n",types[i].pos);
			}
			else if (types[i].type==3)
			{
				printf("%d - <\n",types[i].pos);
			}
			else if (types[i].type==4)
			{
				printf("%d - >\n",types[i].pos);
			}
		}
		for (i = 0; i < size; i++)
		{
			int cant= types[i].pos-1;
			strncpy(argv[i],cmds[i],cant);
		}
		for (i = 0; i < size; i++)
		{
			printf("%s\n", cmds[i]);
			
		}
		if (size==0)
		{
			int pid = fork(); //se crea el proceso padre

			if (pid==-1){ //se comprueba que el proceso no sea hijo, si es el mismo padre, ejecuta el siguiente error
				printf("mayday, mayday, call 911\n");
			}
			
			else if (pid == 0){ //si el pid del proceso es 0, entonces es un hijo, todo puede ejecutarse sin problema
				if (execvp(*argv, argv) < 0) {    /*Se ejecuta el comando a través de la llamada a sistema, 
													en caso de error, es decir, que no sea un comando de sistema, 
													se ejecuta el siguiente error */
					printf("Comando Inválido\n");
	          	}
				else{
					printf("System error\n"); //cualquier otro error, se ve reflejado acá
				}
			}
			else wait(NULL); // Espera a que el proceso termine
		}

		size=0;

	}
	
	return 0;
}