/*
 * Based on the example from the "Intro" chapter of 
 * "Operating Systems: Three Easy Pieces", 
 * Remzi H. Arpaci-Dusseau and Andrea C. Arpaci-Dusseau,
 * Arpaci-Dusseau Books, August, 2018 (Version 1.00)
 * (https://github.com/remzi-arpacidusseau/ostep-code/blob/master/intro/mem.c)
 * 
 * Run as follows in zsh:
 * prompt> ./mem &; ./mem & 
*/

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>

intmax_t GetTime() 
{
    struct timeval t;
    intptr_t rc = gettimeofday(&t, NULL);
    assert(rc == 0);
    return (intmax_t) t.tv_sec + (intmax_t) t.tv_usec/1e6;
}

void Spin(int howlong) 
{
    intmax_t t = GetTime();
    while ((GetTime() - t) < (intmax_t) howlong)
        ; // do nothing in loop
}

int main(int argc, char *argv[]) 
{
    intptr_t *p; 
    p = malloc(sizeof(intptr_t));
    assert(p != NULL);
    printf("(%ld) addr pointed to by p: %p\n", (intptr_t) getpid(), p);
    *p = sizeof(intptr_t);
    while (1) 
    {
        Spin(1);
        *p = *p + 1;
        printf("(%d) value of p: %ld\n", getpid(), *p);
    }
    return 0;
}

