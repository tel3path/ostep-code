/*
 * Based on the example of the same name from the "Intro" chapter of 
 * "Operating Systems: Three Easy Pieces", 
 * Remzi H. Arpaci-Dusseau and Andrea C. Arpaci-Dusseau,
 * Arpaci-Dusseau Books, August, 2018 (Version 1.00)
 * (https://github.com/remzi-arpacidusseau/ostep-code/blob/master/intro/cpu.c)
 *
 * To launch several instances in succession, run as follows:
 * prompt > ./cpu A & ./cpu B & ./cpu C & ./cpu D &
 *
 * Unfortunately, probably not suitable to be added to the CHERI examples library,
 * as in its current form it requires user input.
 * 
 * However, the interesting thing about running this in a CHERI environment with stdin,
 * is that the address of str is the same for every process,
 * whereas in a non-CHERI environment it is always different.
*/

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
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
    if (argc != 2) 
    {
        fprintf(stderr, "usage: cpu <string>\n");
        exit(1);
    }

    char *str = argv[1];
    
#if (defined(__aarch64__) && defined(__CHERI_CAPABILITY_WIDTH__) &&                                \
     !defined(__CHERI_PURE_CAPABILITY__))
    printf("\nOn morello-hybrid, the address of str should be different for each process.\n");
#endif

#ifdef __CHERI_PURE_CAPABILITY__
    printf("\nOn morello-purecap or riscv64-purecap, if you launch multiple instances of "
    "this program on the command line, like so:\n");\
    printf("\nprompt> ./cpu_stdin A & ./cpu_stdin B & ./cpu_stdin C & ./cpu_stdin D &\n");
    printf("\nthe address of str will be the same for each process.\n");
    pp_cap(str);
#else
    printf("Address of str = %p\n", &str);
#endif

    while (1) {
        printf("%s\n", str);
        spin(1);
    }
    
    free(str);
    
    return 0;
}

