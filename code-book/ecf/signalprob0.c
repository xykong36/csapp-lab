#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>
#include "csapp.h"

/* $begin signalprob0 */
volatile long counter = 2;

void handler1(int sig) 
{
    sigset_t mask, prev_mask;

    Sigfillset(&mask);
    Sigprocmask(SIG_BLOCK, &mask, &prev_mask);  /* Block sigs */
    Sio_putl(--counter);
    Sigprocmask(SIG_SETMASK, &prev_mask, NULL); /* Restore sigs */

    _exit(0);
}

int main() 
{
    pid_t pid;
    sigset_t mask, prev_mask;

    // global variable counter == 2
    printf("%ld", counter);
    fflush(stdout);

    signal(SIGUSR1, handler1);
    if ((pid = Fork()) == 0) {
        while(1) {};
    }
    // send the signal to the child process pid
    Kill(pid, SIGUSR1); 
    // -1: wait for any child process
    Waitpid(-1, NULL, 0);

    // which is the first ? 
    Sigfillset(&mask);
    Sigprocmask(SIG_BLOCK, &mask, &prev_mask);  /* Block sigs */
    printf("%ld", ++counter);
    Sigprocmask(SIG_SETMASK, &prev_mask, NULL); /* Restore sigs */

    exit(0);
}
/* $end signalprob0 */
