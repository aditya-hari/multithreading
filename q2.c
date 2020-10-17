#include "student.h"
#include "zone.h"
#include "company.h"
#include "driver.h"
#include "queue.h"

#define STUDENTS (int) 20
#define COMPANIES (int) 1
#define ZONES (int) 3

pthread_mutex_t student_lock = PTHREAD_MUTEX_INITIALIZER;
int active_students = 0;
int total_students = STUDENTS;

extern queue waiting_zones;
extern queue active_zones;

void main(){
    init_queue(&waiting_zones);
    init_queue(&active_zones);

    pthread_t student_threads[STUDENTS];
    pthread_t company_threads[COMPANIES];
    pthread_t zone_threads[ZONES];

    student students[STUDENTS];
    company companies[COMPANIES];
    zone zones[ZONES];

    for(int i = 0; i < STUDENTS; i++){
        student *std = init_student(i);
        pthread_create(&student_threads[i], NULL, student_start_activity, std);
    }

    for(int i = 0; i < COMPANIES; i++){
        company *cmp = init_company(i, 90);
        pthread_create(&company_threads[i], NULL, company_start_activity, cmp);
    }

    for(int i = 0; i < ZONES; i++){
        zone *zne = init_zone(i);
        // enqueue(&waiting_zones, zne);
        pthread_create(&zone_threads[i], NULL, zone_start_activity, zne);
    }

    // // for(int i = 0; i < 5; i++){
    // //     pthread_join(company_threads[i], NULL);
    // // }

    // // for(int i = 0; i < 5; i++){
    // //     pthread_join(zone_threads[i], NULL);
    // // }


    for(int i = 0; i < STUDENTS; i++){
        pthread_join(student_threads[i], NULL);
    }
    printf("Simulation over");
    // node *temp = waiting_zones.head;
    // while(temp){
    //     printf("%d ", temp->val->id);
    //     temp = temp->next;
    // }
    // printf("\n");
    // for(int i = 0; i < 5; i++){
    //     zone* zne = dequeue(&waiting_zones);
    //     if(zne == NULL) printf("whoops");
    //     else printf("%d ", zne->id);
    // }
}