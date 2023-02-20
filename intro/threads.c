#include <assert.h>
#include <inttypes.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>

#ifdef __linux__
#include <semaphore.h>
#endif

volatile size_t counter = 0; 
size_t loops;

void *worker(void *arg) 
{
    size_t i;
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
    printf("Initial value : %zu\n", counter);
#if (!defined(__CHERI_CAPABILITY_WIDTH__) && !defined(__CHERI_PURE_CAPABILITY__))
    assert(pthread_create(&p1, NULL, worker, NULL) == 0);
    assert(pthread_create(&p2, NULL, worker, NULL) == 0);
    assert(pthread_join(p1, NULL) == 0);
    assert(pthread_join(p2, NULL) == 0);
#else
    
#endif
    printf("Final value   : %zu\n", counter);
    return 0;
}

