// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "errorHandler.h"

void abortError (const char * message) {
    perror(message);
    exit(EXIT_FAIL);
}
