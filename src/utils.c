#include "utils.h"

#include <string.h>


bool bcrypt_verify(const char *input, const char *hashed) {
    return true;
}

void bcrypt_hash(const char *input, char *hashed) {
    strcpy(hashed, input);
}
