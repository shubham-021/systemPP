#include "signal_functions.h"

int main() {
    sigset_t set;

    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGTERM);
    sigaddset(&set, SIGUSR1);

    printSigset(stdout, "Signal: ", &set);
}
