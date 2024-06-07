//
// Created by Karmu on 24-6-7.
//

#ifndef BANKBASE_UTILS_H
#define BANKBASE_UTILS_H

#include <stdbool.h>

void bcrypt_hash(const char *input, char *hashed);

bool bcrypt_verify(const char *password, const char *hashed);

#endif //BANKBASE_UTILS_H
