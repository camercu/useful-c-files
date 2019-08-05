/**
 * @brief Network programming functions
 * @file network.c
 */

#include "network.h"

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

/**
 * @brief Get sockaddr, IPv4 or IPv6.
 */
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

/**
 * @brief Set recv timeout socket option in milliseconds.
 * @returns Result of setsockopt() call
 */
int setsockopt_rcvtimeo(int sockfd, int millis) {
    struct timeval timeout = {0};
    if (millis < -1) {
        errno = EINVAL;
        return -1;
    }
    timeout.tv_sec = millis / 1000;
    timeout.tv_usec = (millis % 1000) * 1000;
    return setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout,
                      sizeof(timeout));
}

/**
 * @brief Set socket option REUSEADDR to val.
 * @returns Result of setsockopt() call
 */
int setsockopt_reuseaddr(int sockfd, bool val) {
    int optval = val;
    return setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval,
                      sizeof(optval));
}

/**
 * @brief Send all data in buf through sockfd.
 * @returns Bytes sent on success, -1 on error.
 */
ssize_t sendall(int sockfd, void *buf, size_t *len) {
    size_t total = 0;
    size_t bytesleft = *len;
    ssize_t n;

    while (total < *len) {
        n = send(sockfd, (char *)buf + total, bytesleft, 0);
        if (n == -1) {
            break;
        }
        total += n;
        bytesleft -= n;
    }
    *len = total;
    return n == -1 ? -1 : total;
}

/**
 * @brief Receives all data through sockfd until timeout or connection closes.
 * @returns Bytes received on success, -1 on error.
 */
ssize_t recv_timeout(int sockfd, void **buf, size_t *len, int timeout_millis) {
    size_t total = 0, allocated;
    ssize_t nbytes;
    void *new;

    if (!buf || !len) {
        errno = EINVAL;
        return -1;
    }
    allocated = *len ? *len : RECVBUFSZ;

    if (-1 == setsockopt_rcvtimeo(sockfd, timeout_millis)) {
        return -1;
    }

    do {
        if (total >= allocated - 1)
            allocated += RECVBUFSZ;
        new = realloc(*buf, allocated);
        if (!new) {
            *len = total;
            return -1;
        }
        *buf = new;
        if (-1 == (nbytes = recv(sockfd, (char *)*buf + total,
                                 allocated - total - 1, 0))) {
            break;
        }
        total += nbytes;
    } while (nbytes > 0);
    *((char *)(*buf) + total) = '\0';
    *len = total;

    if (nbytes == -1 && !(errno == EWOULDBLOCK || errno == EAGAIN)) {
        return -1;
    }

    return total;
}

/**
 * @brief Receive bytes until delim is found.
 * @returns Bytes received on success, @c -1 on error
 */
ssize_t recv_delim(int sockfd, void **buf, size_t *len, void *delim,
                   size_t delim_len) {
    size_t total = 0, allocated;
    size_t nbytes;
    void *new;

    if (!buf || !len) {
        errno = EINVAL;
        return -1;
    }
    allocated = *len ? *len : RECVBUFSZ;

    while (!memmem(*buf, total, delim, delim_len)) {
        if (total >= allocated - 1)
            allocated += RECVBUFSZ;
        new = realloc(*buf, allocated);
        if (!new) {
            *len = total;
            return -1;
        }
        *buf = new;
        if (-1 == (nbytes = recv(sockfd, (char *)*buf + total,
                                 allocated - total - 1, 0))) {
            break;
        }
        total += nbytes;
    }
    *((char *)(*buf) + total) = '\0';
    *len = total;
    return nbytes > 0 ? total : -1;
}

/**
 * @brief Receive a given count of bytes.
 * @returns Bytes received, @c -1 on error. Note: bytes received could be less
 * than count if the socket is closed normally while receiving.
 */
ssize_t recv_count(int sockfd, void **buf, size_t count) {
    size_t total = 0;
    size_t nbytes;
    void *new = NULL;

    if (!buf) {
        errno = EINVAL;
        return -1;
    }
    new = calloc(1, count + 1);
    if (!new) {
        return -1;
    }

    while (0 <
           (nbytes = recv(sockfd, (char *)*buf + total, count - total, 0))) {
        total += nbytes;
    }
    *((char *)(*buf) + total) = '\0';
    return nbytes == -1 ? -1 : total;
}

/**
 * @returns Socket descriptor connected to server:port, @c -1 on error
 */
int tcp_client_connect(const char *host, const char *port) {
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(host, port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "[client] getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    /* loop through all the results and connect to the first we can */
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) ==
            -1) {
            perror("[client] socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("[client] connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        perror("[client] failed to connect");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s,
              sizeof s);
    printf("[client] connecting to %s:%s\n", s, port);

    freeaddrinfo(servinfo); /* all done with this structure */
    return sockfd;
}

/**
 * @returns Socket descriptor listening on 'port', @c -1 on error
 */
int tcp_server_listen(const char *port) {
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    /* hints.ai_flags |= AI_NUMERICSERV; */

    if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "[server] getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    /* loop through all the results and bind to the first we can */
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) ==
            -1) {
            perror("[server] socket");
            continue;
        }

        if (setsockopt_reuseaddr(sockfd, true) == -1) {
            perror("[server] setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("[server] bind");
            continue;
        }

        break;
    }
    freeaddrinfo(servinfo); /* all done with this structure */

    if (p == NULL) {
        perror("[server] failed to connect\n");
        return -1;
    }

    if (listen(sockfd, SERVER_BACKLOG) == -1) {
        perror("[server] listen");
        return -1;
    }
    return sockfd;
}
