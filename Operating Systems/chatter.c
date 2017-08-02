#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

void char_at_a_time(char *str) {
	while(*str != '\0') {
		putchar(*str++);
		fflush(stdout);
		usleep(50);
	}
}

int main(int argc, char const *argv[])
{
	if (fork() == 0) {
		// Child process
		char_at_a_time("1111111111");
	} else {
		// Parent process
		char_at_a_time("0000000000");
		// wait();
	}
	return 0;
}