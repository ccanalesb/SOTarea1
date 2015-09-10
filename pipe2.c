#include <stdlib.h>
#include <stdio.h>

char *cmd1[] = { "ls", "-al", "/", 0 };
char *cmd2[] = { "tr", "a-z", "A-Z", 0 };
char *cmd3[] = { "sort", "-r", 0};

void
runsource1(int pfd[],char *cmd[])	/* run the first part of the pipeline, cmd1 */
{
	int pid;	/* we don't use the process ID here, but you may wnat to print it for debugging */

	switch (pid = fork()) {

	case 0: /* child */
		dup2(pfd[1], 1);	/* this end of the pipe becomes the standard output */
		close(pfd[0]); 		/* this process don't need the other end */
		execvp(cmd[0], cmd);	/* run the command */
		perror(cmd[0]);	/* it failed! */

	default: /* parent does nothing */
		break;

	case -1:
		perror("fork");
		exit(1);
	}
}


void
rundest(int pfd[],char *cmd[])	/* run the second part of the pipeline, cmd2 */
{
	int pid;

	switch (pid = fork()) {

	case 0: /* child */
		dup2(pfd[0], 0);	/* this end of the pipe becomes the standard input */
		close(pfd[1]);		/* this process doesn't need the other end */
		execvp(cmd[0], cmd);	/* run the command */
		perror(cmd[0]);	/* it failed! */

	default: /* parent does nothing */
		break;

	case -1:
		perror("fork");
		exit(1);
	}
}

int
main(int argc, char **argv)
{
	int pid, status;
	int fd[3];

	pipe(fd);

	runsource1(fd,cmd1);
	runsource1(fd,cmd2);
	rundest(fd,cmd3);

	close(fd[0]); 
	close(fd[1]); 	/* this is important! close both file descriptors on the pipe */

	while ((pid = wait(&status)) != -1)	/* pick up all the dead children */
		fprintf(stderr, "process %d exits with %d\n", pid, WEXITSTATUS(status));
	exit(0);
}


