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

int count, requests_processed;
pthread_mutex_t mutex; 
pthread_cond_t fill; 
pthread_cond_t empty;

struct Node
{
  int data;
  struct Node* next;
};

struct Node* head;

void push(struct Node** head_ref, int new_data)
{
  struct Node* new_node = (struct Node*) malloc(sizeof(struct Node));
  new_node->data  = new_data;
  new_node->next = (*head_ref); 
  (*head_ref)    = new_node; 
}

void print_produced(int num) {

  printf("Produced %d\n", num);
}

void print_consumed(int num, int worker) {

  printf("Consumed %d by worker %d\n", num, worker);
  
}

void *generate_requests_loop(void *data)
{
  int thread_id = *((int *)data);
  int i;

  for(i = 0; i< total_items; i++)
    {
      pthread_mutex_lock(&mutex);
      while (count >= max_buf_size){
	     pthread_cond_wait(&empty,&mutex);
      }
     
      // buffer[fill_index] = item_to_produce;
      // fill_index = (fill_index + 1)%max_buf_size;

      // Add to the linked list

      push(&head, item_to_produce);

      print_produced(item_to_produce);
      
      item_to_produce++;
      count++;

      
      pthread_cond_broadcast(&fill);
      pthread_mutex_unlock(&mutex);
    } 
    return 0;
}

void *consume_requests_loop(void* data)
{
  int processes_per_thread = 0;
  int thread_id = *((int *)data);
  int max_processes_per_thread = (total_items - (num_workers-1)*total_items/num_workers);

  while(processes_per_thread < max_processes_per_thread && requests_processed < total_items)
  {
  	pthread_mutex_lock(&mutex);

  	while(count==0 && requests_processed<total_items){
  		pthread_cond_wait(&fill, &mutex);
  	}

  	if (requests_processed>=total_items){
  		pthread_cond_broadcast(&fill);
  		pthread_mutex_unlock(&mutex);
  		return 0;
  	}

  	// Remove from linked list

  	// int consumed_number = buffer[use_index];
  	// use_index = (use_index+1)%max_buf_size;
  	// count--;

  	struct Node *temp = head;
  	int consumed_number = temp->data;
  	head = head->next;
  	free(temp);


  	requests_processed++;
  	processes_per_thread++;
  	count--;
  	// print("Consuming... (count)")
  	print_consumed(consumed_number, thread_id);

  	pthread_cond_signal(&empty);
  	pthread_mutex_unlock(&mutex);
  }

  return 0;
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
  head = NULL; // Declare empty linked list
  // buffer = malloc(sizeof(int)*max_buf_size);

  int worker_thread_ids[num_workers];
  pthread_t worker_threads[num_workers];
  for(i=0;i<num_workers;i++)
  	worker_thread_ids[i] = i*10+10;

  //create master producer thread
  pthread_create(&master_thread, NULL, generate_requests_loop, (void *)&master_thread_id);
  // printf("Master Thread Created\n");

  for(i = 0; i<num_workers; i++){
    pthread_create(&worker_threads[i], NULL, consume_requests_loop, (void*) &worker_thread_ids[i]);
  }

  //wait for all threads to complete
  pthread_join(master_thread, NULL);
  printf("master joined\n");

  for(i = 0; i<num_workers; i++){
    pthread_join(worker_threads[i], NULL);
    printf("Worker %i joined \n", worker_thread_ids[i]);
  }

  free(head);
  
  return 0;
}
