#include <assert.h>
#include <inttypes.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>

#ifdef __linux__
#include <semaphore.h>
#endif

intmax_t get_time() 
{
    struct timeval t;
    int rc = gettimeofday(&t, NULL);
    assert(rc == 0);
    return (intmax_t) t.tv_sec + (intmax_t) t.tv_usec/1e6;
}

void Spin(int howlong) 
{
    intmax_t t = get_time();
    while ((get_time() - t) < (intmax_t) howlong)
        ; // do nothing in loop
}

volatile int counter = 0; 
int loops;

void *worker(void *arg) 
{
    int i;
    for (i = 0; i < loops; i++) 
    {
        counter++;
    }
    return NULL;
}

int main(int argc, char *argv[]) 
{
    if (argc != 2) 
    { 
        fprintf(stderr, "usage: threads <loops>\n"); 
        exit(1); 
    } 
    loops = atoi(argv[1]);
    pthread_t p1, p2;
    printf("Initial value : %d\n", counter);
    assert(pthread_create(&p1, NULL, worker, NULL) == 0);
    assert(pthread_create(&p2, NULL, worker, NULL) == 0);
    assert(pthread_join(p1, NULL) == 0);
    assert(pthread_join(p2, NULL) == 0);
    printf("Final value   : %d\n", counter);
    return 0;
}

