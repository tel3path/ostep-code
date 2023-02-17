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

#ifdef __CHERI_PURE_CAPABILITY__
#include "../../include/common.h"
#endif

intmax_t get_time() 
{
    struct timeval t;
    intptr_t rc = gettimeofday(&t, NULL);
    assert(rc == 0);
    return (intmax_t) t.tv_sec + (intmax_t) t.tv_usec/1e6;
}

void spin(int howlong) 
{
    intmax_t t = get_time();
    while ((get_time() - t) < (intmax_t) howlong)
        ; // do nothing in loop
}

int main(int argc, char *argv[]) 
{
    size_t *p; 
    p = malloc(sizeof(size_t));
    assert(p != NULL);
    printf("(%ld) addr pointed to by p: %p\n", (size_t) getpid(), p);
    *p = sizeof(size_t);
    
#if (defined(__aarch64__) && defined(__CHERI_CAPABILITY_WIDTH__) &&                                \
     !defined(__CHERI_PURE_CAPABILITY__))
    printf("\nOn morello-hybrid, the address of p should be different for each instance.\n");
#endif

#ifdef __CHERI_PURE_CAPABILITY__
    printf("\nOn morello-purecap or riscv64-purecap, if you launch multiple instances of "
    "this program at once in zsh, the address of p will be the same for each process.\n");
    
    pp_cap(p);
#endif

    while (1) 
    {
        spin(1);
        *p = *p + 1;
        printf("(%d) value of p: %ld\n", getpid(), *p);
    }
    
    free(p);
    
    return 0;
}

