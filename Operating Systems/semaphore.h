#include "pthread.h"
using namespace std;

class Semaphore{
	int count;	//value of semaphore
	pthread_mutex_t lk; 	//lock used by up and down methods
	pthread_cond_t condvar; 	//conditional variable used by up and down methods

public:
	Semaphore(int N);
	void down();
	void up(); 
};