#ifndef _COMPANY_H_
#define _COMPANY_H_

#include "driver.h"

company* init_company();
void* company_start_activity();
void deliver_vaccines();
void wait_for_consumption();

#endif
