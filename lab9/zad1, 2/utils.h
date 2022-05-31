#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include "utils.h"

#define REINDEER_NUM 9
#define ELF_NUM 10
#define ELVES_REQUIRED 3

#define XD printf("Tester buddy\n");
#define XDD printf("Something gone wrong!\n");

#define randrange(from, to) (rand() % (to + 1 - from) + from)

#define rand_working_sleep() usleep(randrange(2 * 1000000, 5 * 1000000))
#define rand_solving_sleep() usleep(randrange(1 * 1000000, 2 * 1000000))
#define rand_chilling_sleep() usleep(randrange(5 * 1000000, 10 * 1000000))
#define rand_delivering_sleep() usleep(randrange(2 * 1000000, 4 * 1000000))

#endif