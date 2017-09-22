#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>


int main()
{
	char *shm, *s;
	char c;
	key_t shmkey;
	int   shmid;
	int   shmsize;

	//key, similar to naming for our shared memory segment
	shmkey = 5555;
	//size of shared memory
	shmsize = 32;

	//create the segment with arguments KEY , SIZE , MODE (currently creating with linux permissions 666) 
	if ((shmid = shmget(shmkey, shmsize, IPC_CREAT | 0666)) < 0) {
        printf("Error while creating segment. SHMGET failed.\n");
        exit(1);
    }

    //attach the memory to our address space for shm pointer
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        printf("Error while attaching segment. SHMAT failed.\n");
        exit(1);
    }

    s = shm;
    *s++='0';
    for (c = 'a'; c <= 'z'; c++)
        *s++ = c;
    *s = '\0';

    while (*shm != '1')
        sleep(1);

	return 0;
}