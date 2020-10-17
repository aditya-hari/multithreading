#include "company.h"
#include "queue.h"

extern queue waiting_zones;

company* init_company(int id, int success_chance){
    company* cmp = malloc(sizeof(company));
    cmp->id = id;
    cmp->success_chance = success_chance;
    cmp->total_batches = 0;
    cmp->active_batches = 0;
    pthread_mutex_init(&cmp->company_lock, NULL);
    return cmp;
}

void* company_start_activity(void *arg){
    while(1){
        company* cmp = (company *) arg;
        int wait_time = (rand()%3)+2;
        cmp->total_batches = (rand()%4)+1;
        cmp->active_batches = cmp->total_batches;
        printf("Production company %d is preparing %d batches which have success probability of %d\n", cmp->id, cmp->total_batches, cmp->success_chance);
        sleep(wait_time);
        printf("Production company %d has prepared %d batches which have success probability of %d\n", cmp->id, cmp->total_batches, cmp->success_chance);
        deliver_vaccines(cmp);
        wait_for_consumption(cmp);
    }
}

void deliver_vaccines(company* cmp){
    zone* zne = NULL;
    for(int i = 0; i < cmp->total_batches; i++){
        while(zne == NULL) 
            zne = dequeue(&waiting_zones);
        pthread_mutex_lock(&zne->zone_lock);
        printf("Company %d has delivered vaccines to zone %d, resuming vaccinations\n", cmp->id, zne->id);
        zne->quantity = 3;
        zne->parent = cmp;
        pthread_mutex_unlock(&zne->zone_lock);
        zne = NULL;
    }
}

void wait_for_consumption(company* cmp){
    while(cmp->active_batches!=0);
    printf("All vaccines prepared by company %d have been consumed. Resuming production\n", cmp->id);
}