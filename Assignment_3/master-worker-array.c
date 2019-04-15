#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/types.h>

int item_to_produce;
int total_items, max_buf_size, num_workers;
// declare any global data structures, variables, etc that are required
// e.g buffer to store items, pthread variables, etc

int* buffer;
int fill_index, use_index, count, requests_processed;
pthread_mutex_t mutex; 
pthread_cond_t fill; 
pthread_cond_t empty; 

// pthread_mutex_t mutex; = PTHREAD_MUTEX_INITIALIZER;
// pthread_cond_t fill; = PTHREAD_COND_INITIALIZER;
// pthread_cond_t empty; = PTHREAD_COND_INITIALIZER;


void print_produced(int num) {

  printf("Produced %d\n", num);
}

void print_consumed(int num, int worker) {

  printf("Consumed %d by worker %d\n", num, worker);
  
}


/* produce items and place in buffer (array or linked list)
 * add more code here to add items to the buffer (these items will be consumed
 * by worker threads)
 * use locks and condvars suitably
 */
void *generate_requests_loop(void *data)
{
  int thread_id = *((int *)data);
  int i;

  // printf("Master Thread ID: %i\n", thread_id);

  for(i = 0; i< total_items; i++)
    {
      // printf("Locked\n");
      pthread_mutex_lock(&mutex);
      // printf("Inside mutex. item_to_produce: %i. total_items: %i \n", item_to_produce, total_items);
      while (count >= max_buf_size){
      	// printf("going on wait!!!!!!!\n");
	     pthread_cond_wait(&empty,&mutex);
      }
     
	  print_produced(item_to_produce);

      buffer[fill_index] = item_to_produce;
      fill_index = (fill_index + 1)%max_buf_size;
      
      item_to_produce++;
      count++;

      
      pthread_cond_broadcast(&fill);
      pthread_mutex_unlock(&mutex);
    }
  
  // return 0;
}

//write function to be run by worker threads
//ensure that the workers call the function print_consumed when they consume an item

void *consume_requests_loop(void* data)
{
  // int i;
  int thread_id = *((int *)data);
  // printf("In Child Thread: %i. Total Items: %i \n", thread_id, total_items);

  while(requests_processed < total_items)
  {
  	pthread_mutex_lock(&mutex);
  	if (requests_processed==total_items){
  		printf("Return thread %i \n", thread_id);
  		return 0;
  	}

  	while(count==0 && requests_processed!=total_items)
  		pthread_cond_wait(&fill, &mutex);

  	int consumed_number = buffer[use_index];
  	// printf("In Child: use_index: %i, buffer[use_index]: ", use_index, buffer[use_index]);
  	use_index = (use_index+1)%max_buf_size;
  	count--;
  	requests_processed++;

  	print_consumed(consumed_number, thread_id);
  	// printf("Consumed %d by worker %d. requests_processed: %i. use_index: %i \n", consumed_number, thread_id, requests_processed, use_index);
  	pthread_cond_broadcast(&fill);
  	pthread_cond_signal(&empty);
  	pthread_mutex_unlock(&mutex);
  }
}

int main(int argc, char *argv[])
{
  int i, check_error_variable;
 
  int master_thread_id = 0;
  pthread_t master_thread;


  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init (&empty, NULL);
  pthread_cond_init (&fill, NULL);

  item_to_produce = 0;
  fill_index = 0;
  use_index = 0;
  count = 0;
  requests_processed =0;
  
  if (argc < 4) {
    printf("./master-worker #total_items #max_buf_size #num_workers e.g. ./exe 10000 1000 4\n");
    exit(1);
  }
  else {
    num_workers = atoi(argv[3]);
    total_items = atoi(argv[1]);
    max_buf_size = atoi(argv[2]);
  }
  printf("num_workers: %i, total_items: %i, max_buf_size: %i \n", num_workers, total_items, max_buf_size);
  
  // Initlization code for any data structures, variables, etc
  buffer = malloc(sizeof(int)*max_buf_size);

  int worker_thread_ids[num_workers];
  pthread_t worker_threads[num_workers];
  for(i=0;i<num_workers;i++)
  	worker_thread_ids[i] = i*100+10;

  //create master producer thread
  pthread_create(&master_thread, NULL, generate_requests_loop, (void *)&master_thread_id);
  printf("Master Thread Created\n");

  //create worker consumer threads
  // printf("Starting child creation\n");
  for(i = 0; i<num_workers; i++){
    // printf("Worker Thread ID: %i\n", worker_thread_ids[i]);
    check_error_variable = pthread_create(&worker_threads[i], NULL, consume_requests_loop, (void*) &worker_thread_ids[i]);
  }


  //wait for all threads to complete
  pthread_join(master_thread, NULL);
  printf("master joined\n");

  for(i = 0; i<num_workers; i++){
  	// printf("Worker %i joining \n", worker_thread_ids[i]);
    pthread_join(worker_threads[i], NULL);
    printf("Worker %i joined \n", worker_thread_ids[i]);
  }

  //deallocate and free up any memory you allocated

  pthread_exit(NULL);
  
  return 0;
}
