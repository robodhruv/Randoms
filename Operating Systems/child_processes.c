#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

int main(int argc, char const *argv[])
{
	pid_t cpid;
	printf("Main Process = %d, Parent PID = %d\n", getpid(), getppid());

	cpid = fork(); // This is where the child process is created
	printf("%d\n", cpid);// This is expected to give different values for different processes
	if (cpid != 0)
	{
		printf("Original: Parent = %d, Self = %d\n", getppid(), getpid());
	} else {
		printf("Child: Parent = %d, Self = %d\n", getppid(), getpid());
	}
	
	return 0;
}