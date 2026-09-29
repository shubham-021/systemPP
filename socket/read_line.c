#include "read_line.h"
#include <errno.h> /* gives you access to errno, a special variable used by many system/ library calls to tell you why an operation failed */
#include <unistd.h>

/* Some of the error codes:
 * ENOENT: No such file/ directory
 * EACCES: Permission denied
 * EEXIST: Already exists
 * EINVAL: Invalid argument
 * EBADF: Bad file descriptor
 * EAGAIN: Try again: when operation temporarily can't proceed
 * EWOULDBLOCK: Would block
 * ENOMEM: Not enough memory
 * EMFILE: Too many open files
 * ENFILE: Too many open files system-wide
 * EPIPE: Broken pipe
 * ECONNREFUSED: Connection refused
 * ECONNRESET: Connection reset
 * ETIMEOUT: Operation timed out
 * EADDRINUSE: Address already in use
 * EADDRNOTAVAIL: Address unavailable
 * ENOTSTOCK: Not a socket
 * EISDIR: Is a directory
 * ENOTDIR: Not a directory
 * EINTR: Interrupted System call
 */

ssize_t readLine(int fd, void *buffer, size_t n){
    ssize_t numRead; /* # of bytes fetched by last read */
    size_t totRead; /* Total bytes read so far */
    char *buf;
    char ch;

    if(n <= 0 || buffer == NULL) {
        errno = EINVAL;
        return -1;
    }

    buf = buffer;

    totRead = 0;
    while(1) {
        numRead = read(fd, &ch, 1);

        if(numRead == -1){
            if(errno == EINTR)
                continue;
            else
                return -1;
        } else if (numRead == 0) {
            if(totRead == 0)
                return 0;
            else
                break;
        } else {
            if (totRead < n - 1) {
                totRead++;
                *buf++ = ch;
            }

            if(ch == '\n')
                break;
        }
    }

    *buf = '\0';
    return totRead;
}
