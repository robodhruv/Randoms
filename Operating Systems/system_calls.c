#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

int main(int argc, char const *argv[])
{
	pid_t pid[12];
	int i, status;
	for (i = 0; i <= 10; i++) {
		if ((pid[i] = fork()) == 0) {
			// Child Process executes
			printf("Child %d is born with PID %d\n", i, getpid());
			sleep(((11 - i) % 5));
			exit(20 + i);
		}
	}

	for (int i = 0; i <= 10; i++) {
		pid_t pidi = wait(&status);
		if (WIFEXITED(status)) {
			printf("Child %d exited with status %d\n", pidi, WEXITSTATUS(status));
		}
	}


	return 0;
}