#include "base64.h"

#include <stdint.h>
#include <string.h>
#include "dbg.h"


static const char b64e_std[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char b64e_url[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
#define INVALID (-1)

char *b64encode(const char *in, const size_t inlen, const b64_encoding_t charset, size_t *outlen)
{
    (void)in;(void)charset;
    *outlen = ((inlen+2)/3)*4;  /* includes room for padding */
    return NULL;
}


char *b64decode(const char *in, const size_t inlen, const b64_encoding_t charset, size_t *outlen)
{
    const char *end = in + inlen;
    char *out = NULL, *outp = NULL;

    *outlen = ((inlen+3)/4)*3;  /* Upper bound, corrected later */
    out = outp = calloc(1, *outlen + 1);
    if (!out) {
        return NULL;
    }

    /* select appropriate character set */
    const char *basis;
    if (charset == B64_URL) {
        basis = b64e_url;
    } else {
        basis = b64e_std;
    }

    /* initialize the decoding table */
    char dtable[256];
    memset(dtable, INVALID, sizeof(dtable));
    for (size_t i = 0; i < strlen(basis); i++) {
        dtable[(int)basis[i]] = (char)i;
    }

    uint32_t buf;
    int quadpos;
    char byte;
    int padding = 0;
    while (in < end) {
        buf = 0;
        /* load 4 bytes into buffer */
        for (quadpos = 0; (quadpos < 4) && (in < end); quadpos++) {
            byte = dtable[(int)*in++];
            if (byte == INVALID) {
                in = end;
                break;
            }
            buf = (buf << 6) | byte;
        }

        /* ensure appropriate padding */
        if (quadpos == 1) {
            errno = EINVAL;
            free(out);
            *outlen = 0;
            return NULL;
        } else {
            padding = 4 - quadpos;
        }

        /* fix up buffer if didn't load full 4 bytes */
        buf <<= 6 * padding;
        /* move bytes to prep for output */
        buf = (buf << 8) | (buf >> 24);

        /* output 3 bytes from buffer */
        for (int i = 0; i < 3; i++) {
            buf = (buf << 8) | (buf >> 24); /* rotate left 1 byte */
            *outp++ = buf & 0xff;
        }
    }

    *outlen = outp - out - padding;
    return out;
}
