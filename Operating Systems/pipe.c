#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
	int ret, fd[2];
	int a;
	if (pipe(fd) == -1)
	{
		printf("failed to create pipe\n");
		exit(1);
	}

	ret = fork();

	if(ret == -1)
	{
		printf("failed to fork a new process\n");
		exit(1);
	}

	if(ret == 0)
	{
		//child process

		//fd[0] is reading end..child only reads
		close(fd[1]);
		while(read(fd[0],&a,sizeof(a)) > 0)
		{
			printf("%d\n",a);
		}
	}
	else
	{
		//fd[1] is writing end..parent only writes
		close(fd[0]);
		a=2;
		write(fd[1],&a,sizeof(a));
		a=3;
		write(fd[1],&a,sizeof(a));
		close(fd[1]);
		wait(NULL);
		exit(0);
	}

}
