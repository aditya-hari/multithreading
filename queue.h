#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "driver.h"

typedef struct node node;
typedef struct queue queue;

struct node{
    zone* val;
    node* next;
};

struct queue{
    node* head;
    node* tail;
    pthread_mutex_t queue_lock;
};

void init_queue();
node* init_node();
void enqueue();
zone* dequeue();

extern queue waiting_zones;
extern queue active_zones; 

#endif