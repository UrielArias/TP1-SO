#include <sys/stat.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include "communication.h"
#include "errorHandler.h"

#define HASH_LENGTH 32
#define PATH_SIZE 60