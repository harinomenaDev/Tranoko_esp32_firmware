#pragma once

#include <ctype.h>

bool isBlank(const char* str) {
    if (str == nullptr || str[0] == '\0') {
        return true;
    }

    while (*str) {
        if (!isspace((unsigned char)*str)) {
            return false;
        }
        str++;
    }

    return true;
}