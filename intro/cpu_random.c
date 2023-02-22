/*
 * Based on the example of the same name from the "Intro" chapter of 
 * "Operating Systems: Three Easy Pieces", 
 * Remzi H. Arpaci-Dusseau and Andrea C. Arpaci-Dusseau,
 * Arpaci-Dusseau Books, August, 2018 (Version 1.00)
 * (https://github.com/remzi-arpacidusseau/ostep-code/blob/master/intro/cpu.c)
 * 
 * This is a companion piece to cpu_stdin.c, which requires user input and therefore
 * probably is not suitable to be added to the example library.
 * 
 * However, the interesting thing about running cpu_stdin in a CHERI environment
 * is that the address of str is the same for every instance,
 * whereas in a non-CHERI environment it is always different.
 *
 * This version uses a randomized value for each instance, in place of user input.
 * Run as follows in zsh:
 * prompt> ./cpu_random & ./cpu_random & ./cpu_random & ./cpu_random &
 *
 * The interesting thing about running cpu_random in a CHERI environment
 * is that for instances having the same the value of str 
 * (which may be the case for two or more instances, since the randomization
 * of the value of str is time-based), the capabilities in the respective
 * processes will have the same address.
 *
 * Compare this to cpu_stdin, which has a different value for str each time,
 * and has the same address for str's capability each time.
*/

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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

void spin(intptr_t howlong) 
{
    intmax_t t = get_time();
    while ((get_time() - t) < (intmax_t) howlong)
        ; // do nothing in loop
}

int main(int argc, char *argv[])
{    
    char *alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    srand((unsigned)time(NULL));
    size_t index = rand() % 52;
    char *str = &alphabet[index];

#if (defined(__aarch64__) && defined(__CHERI_CAPABILITY_WIDTH__) &&                                \
     !defined(__CHERI_PURE_CAPABILITY__))
    printf("\nOn morello-hybrid, the address of str should be different for each instance.\n");
#endif

#ifdef __CHERI_PURE_CAPABILITY__
    printf("\nOn morello-purecap or riscv64-purecap, if you launch multiple instances of "
    "this program at once in zsh, the address of str will be the same for each "
    "process having the same value of str.\n");

    printf("\nPID = %d\n", getpid());
    pp_cap(str);
#else
    printf("\nPID = %d\n", getpid());
    printf("Address of str = %p\n", &str);
#endif

    while (1) {
        printf("%s\n", str);
        spin(1);
    }
    
    free(str);
    
    return 0;
}

