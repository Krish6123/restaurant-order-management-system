#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>

#define RESTAURANT_PIPE "/tmp/restaurant_order_pipe"
#define TABLE_SEMAPHORE "/restaurant_table_sem"
#define ID_LOCK_SEMAPHORE "/restaurant_id_lock_sem"
#define ID_COUNTER_FILE "customer_id_counter.txt"
#define LOG_FILE "restaurant_log.txt"
#define BILLING_FILE "billing.txt"

#define MAX_ORDERS 10
#define NUM_TABLES 5
#define NUM_COOKS 3

struct Order {
    pid_t customer_pid;
    int customer_id;
    char item_name[50];
    double item_price;
};

#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN    "\033[1;36m"
#define BOLD    "\033[1m"
#define BILL    "\033[1;37;44m"

#endif
