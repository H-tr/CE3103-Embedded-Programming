#include <pthread.h>
#include <sched.h>

pthread_attr_t tattr; // Priotity of a thread can be changed through the thread's attribute
pthread_t tid;
int newprio = 20;
struct sched_param param;

// initialized with default attributes
pthread_attr_init (&tattr);

// get existing scheduling param
pthread_attr_getschedparam (&tattr, &param);

// set the priority; others are unchanged
param.sched_priority = newprio;

// setting the new scheduling param
pthread_attr_setschedparam(&tattr, &param);

// create thread with new priority specified
pthread_create(&tid, &tattr, func, arg);