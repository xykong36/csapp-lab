/* $begin forkprob1 */
#include "csapp.h"

int main() 
{
    int i=0;

    //for (i = 0; i < 2; i++) {
		printf("## pid = %d, i=%d ##\n", getpid(), i);
		sleep(3);
	  Fork();
		printf("### pid = %d, i=%d ###\n", getpid(), i);
	  Fork();
		//}
    printf("pid = %d, hello\n", getpid());
    exit(0);
}
/* $end forkprob1 */
