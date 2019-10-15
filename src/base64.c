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
    const char *end = in + inlen;
    char *out = NULL, *outp = NULL;

    *outlen = ((inlen+2)/3)*4;  /* includes room for padding */
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

    uint32_t buf;
    int tripos, padding;
    char byte;
    while (in < end) {
        buf = 0;
        /* load 3 bytes into buffer */
        for (tripos = 0; (tripos < 3) && (in < end); tripos++) {
            buf = (buf << 8) | *in++;
        }

        /* fix up buf if didn't read full 3 bytes */
        padding = 3 - tripos;
        buf <<= padding * 2;

        /* output 4 bytes by splitting the 3 bytes into 6-bit chunks */
        for (int i = tripos; i >= 0; i--) {
            byte = (buf >> (6 * i)) & 0x3f;
            debug("byte = 0x%02x, b64 = %c", byte, basis[(int)byte]);
            *outp++ = basis[(int)byte];
        }
    }

    /* add padding to end if required */
    for (int i = 0; i < padding; i++) {
        *outp++ = BASE64_PAD;
    }

    return out;
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
        padding = 4 - quadpos;
        if (padding == 3) {
            errno = EINVAL;
            free(out);
            *outlen = 0;
            return NULL;
        }

        /* fix up buffer if didn't load full 4 bytes */
        buf <<= 6 * padding;

        /* output 3 bytes from buffer */
        *outp++ = (buf >> 16) & 0xff;
        *outp++ = (buf >> 8) & 0xff;
        *outp++ = buf & 0xff;
    }

    *outlen = outp - out - padding;
    return out;
}
