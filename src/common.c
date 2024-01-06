#include "include/common.h"

#define _START_RED_TEXT_ "\033[31m"
#define _END_RED_TEXT_ "\033[0m"

void panic(const char *msg, ...) {
    char buffer[4096] = {0};

    va_list args;
    va_start(args, msg);
    vsnprintf(buffer, 4096, msg, args);

    fprintf(stderr, "+ jbk: " _START_RED_TEXT_ "fatal error:" _END_RED_TEXT_ " %s!\n", buffer);

    va_end(args);
}
