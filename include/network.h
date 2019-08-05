#ifndef _network_h_
#define _network_h_

#include <stdbool.h>
#include <sys/socket.h>

#define SERVER_BACKLOG 5
#define RECVBUFSZ 1024

/**
 * @brief Get sockaddr, IPv4 or IPv6.
 */
void *get_in_addr(struct sockaddr *sa);

/**
 * @brief Set recv timeout socket option in milliseconds.
 * @returns Result of setsockopt() call
 */
int setsockopt_rcvtimeo(int sockfd, int millis);

/**
 * @brief Set socket option REUSEADDR to val.
 * @returns Result of setsockopt() call
 */
int setsockopt_reuseaddr(int sockfd, bool val);

/**
 * @brief Send all data in buf through sockfd.
 * @returns Bytes sent on success, -1 on error.
 */
ssize_t sendall(int sockfd, void *buf, size_t *len);

/**
 * @brief Opens a TCP socket and connects to server (host:port).
 * @returns Socket descriptor connected to server:port, @c -1 on error
 */
int tcp_client_connect(const char *host, const char *port);

/**
 * Opens a TCP socket, binds to the port, and listens on all interfaces.
 * @returns Socket descriptor listening on 'port', @c -1 on error
 */
int tcp_server_listen(const char *port);

#endif /* _network_h_ */
