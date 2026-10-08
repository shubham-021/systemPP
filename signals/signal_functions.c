#include <stdio.h>
#define _GNU_SOURCE
#include <string.h>
#include <signal.h>
#include "../lib/tlpi_hdr.h"
#include "signal_functions.h"

/*  Do not casually call these functions from inside a signal handler, because internally they use fprintf(),
*   and fprintf() is not async-signal-safe.
*
*   An async-signal-safe function is a function that POSIX guarantees you can safely call from a signal handler.
*   for example:
*       write() -> async-signal-safe
*       printf(), fprintf(), malloc(), free() -> not async-signal-safe
*
*   Async-signal-safe ?
*       Asynchronous:
*           Suppose your program is doing: fprintf(stdout, "Hello"); , Internally, fprintf() is doing a bunch of work
*           Now imagine:
*               Your thread --> fprintf() --- SIGINT arrives here --> signal handler runs
*           The signal handler runs in the middle of whatever the thread was doing. That's what "asynchronous" means here:
*           the interruption can happen at essentially any point.
*
*       Signal-safe:
*           A function is async-signal-safe if it can safely be called from a signal handler, even if the signal interrupted
*           even if the signal interrupted the program at an arbitrary point.
*
*           For example, POSIX considers write() async-signal-safe. So:
*           void handler(int sig) {
*               write(STDOUT_FILENO, "SIGINT\n", 7);
*           }
*           is okay
*
*   Why isn't fprintf() safe ?
*       fprintf() isn't just: write(fd, buffer, size); , it maintains internal state
*       for example, conceptually FILE might contain:
*           FIlE
*            |- buffer
*            |- current position
*            |- buffer size
*            |- flags
*            |- lock
*            |- other internal state
*
*           fprintf()
*             |- parse format string
*             |- convert argument
*             |- manage FILE structure
*             |- manage stdio buffer
*             |- acquire locks
*             |- update internal state
*             |- eventually -> write()
*
*       Imagine your program is currently doing: fprintf(stout, "Hello"); and internally it is doint its work and then a signal arrives. Your handler does: fprintf(stdout, "Signal!");
*       Now you are asking fprint() to operate on the same stdio machinery while the previous fprint() may have been interupted halway
*       through modifying it.
*
*       That can result in corrupted state or a deadlock.
*
*       The lock problem:
*           Imagine fprintf() internally has acquired a lock:
*           main thread -> fprintf() -> (acquire stdout lock, modify buffer) (<- signal arrives) -> handler() -> fprintf() -> tries to acquire lock -> waits forever
*
*       The two fprintf are not necessarily "mixing the output" directly.
*       The problem is that first fprintf() may have left the stdout object in a temporary state that assumes fprintf will continue and finish
*/

void printSigset(FILE *of, const char *prefix, const sigset_t *sigset) {
    int sig, cnt;

    cnt = 0;
    for(sig = 1; sig < NSIG; sig++){
        if(sigismember(sigset, sig)) {
            cnt++;
            fprintf(of, "%s%d (%s)\n", prefix, sig, strsignal(sig));
        }
    }

    /* NSIG is a constant/macro that represents the number of signal numbers available on a system( more precisely, one greater than the highest signal number on many UNIX systems) */

    if(cnt == 0)
        fprintf(of, "%s<empty signal set>\n", prefix);
}

int printSigMask(FILE *of, const char *msg){
    sigset_t currMask;
    if(msg != NULL)
        fprintf(of, "%s", msg);

    if(sigprocmask(SIG_BLOCK, NULL, &currMask) == -1)
        return -1;

    printSigset(of, "\t\t", &currMask);

    return 0;
}

int printPendingSigs(FILE *of, const char *msg){
    sigset_t pendingSigs;

    if(msg != NULL)
        fprintf(of, "%s", msg);

    if(sigpending(&pendingSigs) == -1)
        return -1;

    printSigset(of, "\t\t", &pendingSigs);

    return 0;
}
