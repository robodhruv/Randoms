#include "semaphore.h"
#include <stdlib.h>		// for srand, rand
#include <unistd.h>
#include "time.h"
#include <iostream>
#include <vector>
#define BUFFER_CAP 2
using namespace std;

int finite_num = 50; // some random iterations
Semaphore empty_count = Semaphore(BUFFER_CAP);
Semaphore filled_count = Semaphore(0);
int buffer_size = 0;
Semaphore buffer_lock = Semaphore(1);

int produce_item(){
	cout<<endl<<"Produced"<<endl;
	return 0;
}

void put_item_in_buffer(int item){
	buffer_lock.down();
	// some action
	cout<< endl<< "Added | Buffer size : " << ++buffer_size << endl;
	buffer_lock.up();
	return;
}

int remove_item_from_buffer(){
	buffer_lock.down();
	// some action
	cout<< endl<< "Removed | Buffer size : " << --buffer_size << endl;
	buffer_lock.up();
	return 0;
}

void producer(){
	while(--finite_num > 0){ 	// finite_num shared but dont care
		int item = produce_item();
		empty_count.down();
		put_item_in_buffer(item);
		filled_count.up();
		sleep(rand()%2);
	}
}

void consumer(){
	while(--finite_num > 0){
		cout<<endl<<"Try to remove"<<endl;
		filled_count.down();
		int item = remove_item_from_buffer();
		empty_count.up();
		// consume item
		sleep(rand()%2);
	}
}

void *thread_run(void *data){

	int thread_id = *((int *)data);
	if (thread_id == 0) producer();
	else consumer();

}

int main(int argc, char *argv[]){
	srand (time(NULL));
	int total_threads = 2;
	vector<pthread_t> thr(total_threads);
	vector<int> tid(total_threads);
	for(int i=0; i < total_threads; i++){
		tid[i] = i;
		pthread_create(&thr[i], NULL, thread_run, (void *)&tid[i]);
	}

	for(int i=0; i < total_threads; i++){
		pthread_join(thr[i], NULL);
	}
	exit(0);
}