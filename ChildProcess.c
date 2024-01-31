/*
 * Author: Suha Memon
 * This code has been completed entirely by me.
 * Description of purpose of program: ChildProcess.c
    * This has a main method
    * 
    * The main method handles all execve("ChildProcess", NULL, NULL);
    * This would handle all of the timing of my program
    * 
    * main initializes a handler for SIGINT, so that it can essentially validate that the program runs for 10 seconds
    * at the 5 second mark, it does kill(ppid, SIGUSR1) back to the parent process to 
    * alert the user that 5 seconds have passed
    * then, it checks to see if 10 seconds pass.
    * when that happens, i do: kill(ppid, SIGINT); so that the ParentProcess knows that the process is complete
    * and that the main one needs to be killed
*/

// needed to add the below line to resolve CIMS errrors with implicit killing
#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
int secs = 0;

/*
 * main:
 * 
 * Purpose: take care of all timer operations as explained above
 * 
 * Parameters: int argc, char **argv
 * 
 * Return value: int but really just 0
 */

int main(int argc, char **argv) {
    pid_t ppid = getppid();
    while (secs < 10) {
        sleep(1);
        secs++;
        if (secs == 5) {
            kill(ppid, SIGUSR1); // Notify parent after 5 seconds
        }
    }
    kill(ppid, SIGINT); // Notify parent after 10 seconds
    return 0;
}
