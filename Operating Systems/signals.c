#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>

int childpid;
int information;


void sighandler(int signo)
{
	if(signo == SIGINT)
	{
		information = 1;
		if(childpid==0){
			printf("SIGINT signal recieved for child\n");
		}
		if(childpid!=0){
			printf("SIGINT signal recieved for parent\n");
			kill(childpid,SIGKILL);
			wait(NULL);
			exit(0);
		}
	}
}

int main()
{
	int ret;
	information = 0;
	if(signal(SIGINT,sighandler) == SIG_ERR)
	{
		printf("Failed to register the signal handler\n");
	}

	ret = fork();

	if(ret == -1)
	{
		printf("failed to fork a new process\n");
		exit(1);
	}

	childpid = ret;
	while(1)
	{
		sleep(1);
	}
}
