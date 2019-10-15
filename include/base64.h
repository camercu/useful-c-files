#ifndef _BASE64_H_
#define _BASE64_H_

#include <stdlib.h>

#ifndef BASE64_PAD
#   define BASE64_PAD '='
#endif /* BASE64_PAD */

typedef enum {
    B64_STANDARD,   /* RFC 4648 "base64" */
    B64_URL         /* RFC 4648 "base64url" */
} b64_encoding_t;

char *b64encode(const char *in, const size_t inlen, const b64_encoding_t charset, size_t *outlen);
char *b64decode(const char *in, const size_t inlen, const b64_encoding_t charset, size_t *outlen);

#endif /* _BASE64_H_ */
