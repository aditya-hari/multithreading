#include "student.h"
#include "queue.h"

extern int active_students;
extern queue active_zones;
extern queue waiting_zones;
extern pthread_mutex_t student_lock;

student* init_student(int id){
    student *std = malloc(sizeof(student));
    std->id = id+1;
    std->round = 1;
    std->success = -1;
    std->waiting = 1;
    return std;
}

void* student_start_activity(void *arg){
    student* std = (student *) arg;
    sleep(rand()%10);
    while(1){
        pthread_mutex_lock(&student_lock);
        active_students++;
        pthread_mutex_unlock(&student_lock);
        printf("Student %d has arrived for their vaccination round %d\n", std->id, std->round);
        search_for_zone(std);
        if(std->success == 1){
            printf("DONE - Student %d has tested positive for antibodies\n", std->id);
            return &std->success;
        }
        else{
            printf("Student %d has tested negative for antibodes\n", std->id);
            std->success = -1;
            std->waiting = 1;
            if(std->round > 3){
                printf("Student %d has failed his 3rd test. They have been sent back home\n", std->id);
                return &std->success;
            }
            std->round++;
        }
    }
}

void search_for_zone(student* std){
    zone* zne;
    printf("Student %d waiting to be allocated a slot in a vaccination zone\n", std->id);
    node* temp = active_zones.head;

    while(std->waiting == 1){
        while(active_zones.head == NULL);
        pthread_mutex_lock(&active_zones.queue_lock);
        zne = active_zones.head->val;
        if(zne != NULL){
            pthread_mutex_lock(&zne->zone_lock);
            // printf("STUDENT - %d %d %d %d\n", std->id, zne->id, zne->available_slots, active_students);
            if(zne->available_slots > 0){
                zne->available_slots--;
                zne->current_students[zne->current_students_idx] = malloc(sizeof(student));
                zne->current_students[zne->current_students_idx++] = std;
                pthread_mutex_lock(&student_lock);
                printf("Student %d allocated a slot in vaccination zone %d\n", std->id, zne->id);
                std->waiting = 0;
                active_students--;
                pthread_mutex_unlock(&student_lock);
            }
            pthread_mutex_unlock(&zne->zone_lock);
        }
        pthread_mutex_unlock(&active_zones.queue_lock);
    }
    while(std->success == -1);
}