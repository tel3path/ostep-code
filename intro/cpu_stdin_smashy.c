/*
 * Based on the example of the same name from the "Intro" chapter of 
 * "Operating Systems: Three Easy Pieces", 
 * Remzi H. Arpaci-Dusseau and Andrea C. Arpaci-Dusseau,
 * Arpaci-Dusseau Books, August, 2018 (Version 1.00)
 * (https://github.com/remzi-arpacidusseau/ostep-code/blob/master/intro/cpu.c)
 *
 * To launch several instances at once, run in zsh as follows:
 * prompt > ./cpu_stdin A & ./cpu_stdin B & ./cpu_stdin C & ./cpu_stdin D &
 *
 * Unfortunately, probably not suitable to be added to the CHERI examples library,
 * as in its current form it requires user input.
 * 
 * However, the interesting thing about running this in a CHERI environment with stdin,
 * is that the address of str is the same for every instance,
 * whereas in a non-CHERI environment it is always different.
*/

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    if (argc != 2) 
    {
        fprintf(stderr, "usage: cpu_stdin_smashy <string>\n");
        printf("To attempt to smash the stack:\n");
        printf("prompt> ./cpu_stdin_smashy <string longer than 27>\n");
        printf("To launch multiple instances from the command line:\n");
        printf("prompt> ./cpu_stdin_smashy <string> & ./cpu_stdin_smashy <string> &\n");
        exit(1);
    }

    char *str = malloc(27);
    strcpy(str, argv[1]);
    
#if (defined(__aarch64__) && defined(__CHERI_CAPABILITY_WIDTH__) &&                                \
     !defined(__CHERI_PURE_CAPABILITY__))
    printf("\nOn morello-hybrid, the address of str should be different for each process.\n");
    printf("It should run despite your attempt to smash the stack with input longer than 27 characters.\n");
#endif

#ifdef __CHERI_PURE_CAPABILITY__
    printf("\nOn morello-purecap or riscv64-purecap, if you launch multiple instances of "
    "this program at once in zsh, the address of str should be the same for each "
    "process.\n");
    printf("If you try to smash the stack with input longer than 27 characters,\n");
    printf("the program will SIGPROT, but any instances with valid input will keep running.\n");

    printf("\nPID = %d, str=%s\n", getpid(), str);
    pp_cap(str);
#else
    printf("\nPID = %d, str=%s\n", getpid(), str);
    printf("Address of str = %p\n", &str);
#endif

    while (1) {
        printf("\nPID = %d, str=%s\n", getpid(), str);
        spin(1);
    }
    
    free(str);
    
    return 0;
}

