#ifndef _DRIVER_H_
#define _DRIVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

extern pthread_mutex_t student_lock;
extern int active_students; 

typedef struct production_company company;
typedef struct student student; 
typedef struct vaccination_zone zone;

struct production_company{
    int id;
    int total_batches;
    int active_batches;
    int success_chance;
    pthread_mutex_t company_lock;
};

struct student{
    int id;
    int success;
    int round; 
    int waiting;
};

struct vaccination_zone{
    int id;
    int slots;
    int quantity;
    company *parent;
    int available_slots;
    pthread_mutex_t zone_lock;
    student** current_students;
    int current_students_idx;
};

#endif