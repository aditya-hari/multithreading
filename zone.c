#include "zone.h"
#include "queue.h"

queue waiting_zones;
queue active_zones;
int active_students;

zone* init_zone(int id){
    zone* zne = malloc(sizeof(zone));
    zne->id = id+1;
    zne->slots = -1;
    zne->available_slots = -1;
    zne->parent = NULL;
    zne->current_students = NULL;
    zne->current_students_idx = 0;
    pthread_mutex_init(&zne->zone_lock, NULL);
    return zne;
}

void* zone_start_activity(void* args){
    zone* zne = (zone*) args;
    while(1){
        pthread_mutex_lock(&zne->zone_lock);
        if(zne->parent == NULL)
            enqueue(&waiting_zones, zne);
        pthread_mutex_unlock(&zne->zone_lock);
        while(zne->parent == NULL);
        initialize_vaccination(zne);
    }
}

void initialize_vaccination(zone* zne){
    company* parent = zne->parent;

    // while(zne->quantity > 0){
    int slots = (rand()%8)+1;
    if(slots > zne->quantity){
        slots = zne->quantity;
    }
    pthread_mutex_lock(&student_lock);
    if(active_students == 0){
        pthread_mutex_unlock(&student_lock);
        return;
    };
    if(slots > active_students){
        slots = active_students;
    }
    pthread_mutex_unlock(&student_lock);
    // pthread_mutex_lock(&zne->zone_lock);
    zne->available_slots = slots;
    zne->current_students = malloc(sizeof(student*)*slots);
    for(int i = 0; i < slots; i++)
        zne->current_students[i] = NULL;
    printf("Vaccination zone %d is ready to vaccinate with %d slots\n", zne->id, zne->available_slots);
    // pthread_mutex_unlock(&zne->zone_lock);
    pthread_mutex_lock(&active_zones.queue_lock);
    enqueue(&active_zones, zne);
    // pthread_mutex_lock(&zne->zone_lock);
    while(zne->available_slots){
        // pthread_mutex_lock(&student_lock);
        // printf("ZONE - %d %d\n", zne->id, zne->available_slots);
        if(active_students == 0 && zne->available_slots < slots) 
            break;
        // pthread_mutex_unlock(&student_lock);
    }

    // pthread_mutex_unlock(&zne->zone_lock);
    dequeue(&active_zones);
    pthread_mutex_unlock(&active_zones.queue_lock);
    // printf("HERE? %d %d\n", slots, zne->quantity);
    printf("Vaccination zone %d is entering vaccination phase with %d students\n", zne->id, zne->current_students_idx);
    zne->quantity = zne->quantity - zne->current_students_idx;
    for(int i = 0; i < zne->current_students_idx; i++){
        student* std = zne->current_students[i];
        if(std == NULL) break;
        int chance = (rand()%100)+1;
        printf("Student %d on vaccination zone %d has been vaccinated with a success probability of %d\n", std->id, zne->id, zne->parent->success_chance);
        if(chance < zne->parent->success_chance)
            std->success = 1;
        else
            std->success = 0;
    }
    zne->current_students_idx = 0;
    // }
    // pthread_mutex_lock(&zne->zone_lock);
    // zne->parent = NULL;
    // printf("Vaccination zone %d has run out of vaccines\n", zne->id);
    // pthread_mutex_lock(&parent->company_lock);
    // parent->active_batches--;
    // pthread_mutex_unlock(&parent->company_lock);
    // pthread_mutex_unlock(&zne->zone_lock);
}

void start_vaccination(zone* zne){
    pthread_mutex_unlock(&active_zones.queue_lock);
    // printf("HERE? %d %d\n", slots, zne->quantity);
    printf("Vaccination zone %d is entering vaccination phase with %d students\n", zne->id, zne->current_students_idx);
    zne->quantity = zne->quantity - zne->current_students_idx;
    for(int i = 0; i < zne->current_students_idx; i++){
        student* std = zne->current_students[i];
        if(std == NULL) break;
        int chance = (rand()%100)+1;
        printf("Student %d on vaccination zone %d has been vaccinated with a success probability of %d\n", std->id, zne->id, zne->parent->success_chance);
        if(chance < zne->parent->success_chance)
            std->success = 1;
        else
            std->success = 0;
    }
    zne->current_students_idx = 0;
}
