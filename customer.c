#include "common.h"
#include <time.h>
volatile sig_atomic_t order_ready=0;
void order_ready_handler(int sig){order_ready=1;}
int get_customer_id(){
 int id; sem_t *lock=sem_open(ID_LOCK_SEMAPHORE,0);
 sem_wait(lock);
 FILE *f=fopen(ID_COUNTER_FILE,"r"); fscanf(f,"%d",&id); fclose(f);
 f=fopen(ID_COUNTER_FILE,"w"); fprintf(f,"%d",id+1); fclose(f);
 sem_post(lock); sem_close(lock); return id;
}
int main(){
 signal(SIGUSR1,order_ready_handler);
 srand(time(NULL)^getpid());
 struct Order o; o.customer_pid=getpid(); o.customer_id=get_customer_id();
 char *items[]={"Pizza","Burger","Pasta","Salad","Steak","Fish & Chips"};
 double prices[]={12.99,8.99,10.5,7.0,18.75,14.0};
 int c=rand()%6; strcpy(o.item_name,items[c]); o.item_price=prices[c];
 sem_t *table=sem_open(TABLE_SEMAPHORE,0);
 int pipe_fd=open(RESTAURANT_PIPE,O_WRONLY);
 printf("Customer #%d waiting table\n",o.customer_id);
 sem_wait(table);
 printf("Ordering %s\n",o.item_name);
 write(pipe_fd,&o,sizeof(o));
 while(!order_ready) sleep(1);
 printf("Food ready! Eating...\n");
 sleep(6+rand()%5);
 printf("Bill Paid %.2f\n",o.item_price);
 sem_post(table);
 close(pipe_fd); sem_close(table);
}
