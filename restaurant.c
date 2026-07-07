#include "common.h"

struct Order order_queue[MAX_ORDERS];
int order_count=0,order_in_ptr=0,order_out_ptr=0;
pthread_mutex_t queue_mutex,log_mutex,billing_mutex;
pthread_cond_t cond_queue_not_full,cond_queue_not_empty;
sem_t *table_semaphore,*id_lock_semaphore;

void write_to_log(char *message){
 pthread_mutex_lock(&log_mutex);
 FILE *f=fopen(LOG_FILE,"a");
 if(f){fprintf(f,"[Restaurant] %s\n",message); fclose(f);}
 printf("%s\n",message);
 pthread_mutex_unlock(&log_mutex);
}

void write_to_billing(struct Order o){
 pthread_mutex_lock(&billing_mutex);
 FILE *f=fopen(BILLING_FILE,"a");
 if(f){fprintf(f,"Bill for Customer #%d: %s - Price: $%.2f\n",o.customer_id,o.item_name,o.item_price); fclose(f);}
 pthread_mutex_unlock(&billing_mutex);
}

void* cook_function(void *arg){
 int cook_id=*((int*)arg); char log_buffer[256];
 sprintf(log_buffer,"[Cook #%d] " CYAN "Ready and waiting." RESET,cook_id);
 write_to_log(log_buffer);
 while(1){
  struct Order cur;
  pthread_mutex_lock(&queue_mutex);
  while(order_count==0) pthread_cond_wait(&cond_queue_not_empty,&queue_mutex);
  cur=order_queue[order_out_ptr];
  order_out_ptr=(order_out_ptr+1)%MAX_ORDERS; order_count--;
  sprintf(log_buffer,"[Cook #%d] Taking %s for Cust #%d.",cook_id,cur.item_name,cur.customer_id);
  write_to_log(log_buffer);
  pthread_cond_signal(&cond_queue_not_full);
  pthread_mutex_unlock(&queue_mutex);
  sleep(5+rand()%4);
  sprintf(log_buffer,"[Cook #%d] Finished %s for Cust #%d.",cook_id,cur.item_name,cur.customer_id);
  write_to_log(log_buffer);
  kill(cur.customer_pid,SIGUSR1);
  write_to_billing(cur);
 }
 return NULL;
}

int main(){
 int pipe_fd; pthread_t cooks[NUM_COOKS]; int ids[NUM_COOKS];
 unlink(RESTAURANT_PIPE); sem_unlink(TABLE_SEMAPHORE); sem_unlink(ID_LOCK_SEMAPHORE);
 fclose(fopen(LOG_FILE,"w")); fclose(fopen(BILLING_FILE,"w"));
 FILE *fp=fopen(ID_COUNTER_FILE,"w"); fprintf(fp,"1"); fclose(fp);
 pthread_mutex_init(&queue_mutex,NULL); pthread_mutex_init(&log_mutex,NULL); pthread_mutex_init(&billing_mutex,NULL);
 pthread_cond_init(&cond_queue_not_full,NULL); pthread_cond_init(&cond_queue_not_empty,NULL);
 table_semaphore=sem_open(TABLE_SEMAPHORE,O_CREAT,0666,NUM_TABLES);
 id_lock_semaphore=sem_open(ID_LOCK_SEMAPHORE,O_CREAT,0666,1);
 mkfifo(RESTAURANT_PIPE,0666);
 pipe_fd=open(RESTAURANT_PIPE,O_RDONLY);
 for(int i=0;i<NUM_COOKS;i++){ids[i]=i+1; pthread_create(&cooks[i],NULL,cook_function,&ids[i]);}
 printf("Restaurant started.\n");
 while(1){
  struct Order o;
  if(read(pipe_fd,&o,sizeof(o))>0){
   pthread_mutex_lock(&queue_mutex);
   while(order_count==MAX_ORDERS) pthread_cond_wait(&cond_queue_not_full,&queue_mutex);
   order_queue[order_in_ptr]=o; order_in_ptr=(order_in_ptr+1)%MAX_ORDERS; order_count++;
   pthread_cond_signal(&cond_queue_not_empty);
   pthread_mutex_unlock(&queue_mutex);
  }
 }
}
