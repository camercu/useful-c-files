#include "utils.h"
#include <stdint.h>

/**
 * @brief: host-endianness-independent implementation of ntohl()
 * @see: https://stackoverflow.com/a/2100549/5202294
 */
uint32_t ntohl(uint32_t n) {
    unsigned char *np = (unsigned char *)&n;

    return ((uint32_t)np[0] << 24) | ((uint32_t)np[1] << 16) |
           ((uint32_t)np[2] << 8) | (uint32_t)np[3];
}
