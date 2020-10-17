#include "queue.h"

node* init_node(zone* val){
    node* n = malloc(sizeof(node));
    n->val = val;
    n->next = NULL;
    return n;
}

void init_queue(queue* q){
    pthread_mutex_init(&q->queue_lock, NULL);
    q->head = NULL;
    q->tail = NULL;
}

void enqueue(queue* q, zone* val){
    node* new = init_node(val);
    pthread_mutex_lock(&q->queue_lock);
    if(q->head == NULL)
        q->head = new;
    else
        q->tail->next = new;
    q->tail = new;
    pthread_mutex_unlock(&q->queue_lock);
}

zone* dequeue(queue* q){
    zone* out;
    pthread_mutex_lock(&q->queue_lock);
    if(q->head == NULL)
        out = NULL;
    else{
        out = q->head->val;
        q->head = q->head->next;
    }
    pthread_mutex_unlock(&q->queue_lock);
    return out;
}


