#include "csapp.h"

void initjobs()
{
}

void addjob(int pid)
{
}

void deletejob(int pid)
{
}

/* $begin procmask1 */
/* WARNING: This code is buggy! */
void handler(int sig)
{
    int olderrno = errno;
    sigset_t mask_all, prev_all;
    pid_t pid;

    Sigfillset(&mask_all);
    while ((pid = waitpid(-1, NULL, 0)) > 0) { /* Reap a zombie child */
        Sigprocmask(SIG_BLOCK, &mask_all, &prev_all);
        // when the parent reaps a terminated child in the SIGCHLD signal handler, it deletes the child from the job list
        deletejob(pid); /* Delete the child from the job list */
        Sigprocmask(SIG_SETMASK, &prev_all, NULL);
    }
    if (errno != ECHILD)
        Sio_error("waitpid error");
    errno = olderrno;
}
    
int main(int argc, char **argv)
{
    int pid;
    sigset_t mask_all, prev_all;

    Sigfillset(&mask_all);
    Signal(SIGCHLD, handler);
    initjobs(); /* Initialize the job list */

    while (1) {
        if ((pid = Fork()) == 0) { /* Child process */
            Execve("/bin/date", argv, NULL);
        }
        Sigprocmask(SIG_BLOCK, &mask_all, &prev_all); /* Parent process */  
        // after the parent creates a new child process, it adds the child to the job list
        addjob(pid);  /* Add the child to the job list */
        Sigprocmask(SIG_SETMASK, &prev_all, NULL);    
    }
    exit(0);
}
/* $end procmask1 */

/*
classic syncrhonization error known as a race. 
In this case, the race is between the call to addjob in the main routine and 
the call to deletejob in the handler. 
*/
