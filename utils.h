///////////////////////////////////
// Utility Macros
// for common tasks
///////////////////////////////////

#ifndef __util_macros_h__
#define __util_macros_h__

#include <errno.h>  /* errno */

#ifdef __cplusplus
extern "C" {
#endif

#define ABS(VALUE) ((VALUE) < 0 ? -(VALUE) : (VALUE))
#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define MAX(A, B) ((A) > (B) ? (A) : (B))
#define SWAP(A, B) do { (A) ^= (B); (B) ^= (A); (A) ^= (B); } while (0)

/* macro to restart a system call that was interrupted by a signal.
   Source: Kerrisk, Michael. The Linux Programming Interface: A Linux and UNIX
   System Programming Handbook. No Starch Press. San Francisco, 2010. pg. 443 */
#define NO_EINTR(STMT) while ((STMT) == -1 && errno == EINTR);
/* example usage:
   NO_EINTR(cnt = read(fd, buf, BUF_SIZE));
   check(cnt != -1, "read() failed with other than EINTR"); */

/* this typedef just makes signal handlers more obvious,
   and allows re-defining the signal() function as wrapper for sigaction() */
typedef void (*sig_handler_t)(int signo);

#ifdef __cplusplus
}
#endif

#endif /* __util_macros_h__ */
