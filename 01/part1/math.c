#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <gnumake.h>

#define YES 1
#define NO 0

#define ERROR_PREFIX "*** ARITHMETIC ERROR: "

int plugin_is_GPL_compatible;

void throw_error(const char *str, int *error)
{
    fprintf(stderr, ERROR_PREFIX "String '%s' is not representable as a long!\n", str);
    *error = YES;
}

long strtol_or_die(const char *str, int *error)
{
    char *end;
    long ret;
    errno = 0;
    
    ret = strtol(str, &end, 0);

    if (*end != '\0' ||
        errno == EINVAL ||
        errno == ERANGE) {

        throw_error(str, error);
        return 0;
    }

    *error = NO;
    return ret;
}

char *add(const char *nm, unsigned argc, char **argv)
{
    long temp, total = 0;
    int err = 0;
    char *n = strtok(argv[0], " \t");
    while (n) {
        temp = strtol_or_die(n, &err);
        if (err == YES)
            return NULL;
        total += temp;
        n = strtok(NULL, " \t");
    }
    int size = snprintf(NULL, 0, "%ld", total) + 1;
    char *buf = gmk_alloc(size);
    snprintf(buf, size, "%ld", total);
    return buf;
}

char *sub(const char *nm, unsigned argc, char **argv)
{
    long temp, total = 0;
    int err = 0;
    char *n = strtok(argv[0], " \t");
    if (n) {
        total = strtol_or_die(n, &err);
        if (err == YES)
            return NULL;
        n = strtok(NULL, " \t");
        while (n) {
            temp = strtol_or_die(n, &err);
            if (err == YES)
                return NULL;
            total -= temp;
            n = strtok(NULL, " \t");
        }
    }
    int size = snprintf(NULL, 0, "%ld", total) + 1;
    char *buf = gmk_alloc(size);
    snprintf(buf, size, "%ld", total);
    return buf;
}

char *mul(const char *nm, unsigned argc, char **argv)
{
    long temp, total = 1;
    int err = 0;
    char *n = strtok(argv[0], " \t");
    while (n) {
        temp = strtol_or_die(n, &err);
        if (err == YES)
            return NULL;
        total *= temp;
        n = strtok(NULL, " \t");
    }
    int size = snprintf(NULL, 0, "%ld", total) + 1;
    char *buf = gmk_alloc(size);
    snprintf(buf, size, "%ld", total);
    return buf;
}

char *divide(const char *nm, unsigned argc, char **argv)
{
    long temp, total = 0;
    int err = 0;
    char *n = strtok(argv[0], " \t");
    if (n) {
        total = strtol_or_die(n, &err);
        if (err == YES)
            return NULL;
        n = strtok(NULL, " \t");
        while (n) {
            temp = strtol_or_die(n, &err);
            if (err == YES)
                return NULL;
            if (temp == 0) {
                fprintf(stderr, ERROR_PREFIX "Cannot divide by zero!\n");
                return NULL;
            }
            total /= temp;
            n = strtok(NULL, " \t");
        }
    }
    int size = snprintf(NULL, 0, "%ld", total) + 1;
    char *buf = gmk_alloc(size);
    snprintf(buf, size, "%ld", total);
    return buf;
}
 
int math_gmk_setup()
{
    // 0 means no limit
    // name, func_ptr, min args, max args, expand or no?
    gmk_add_function ("add", add, 1, 0, GMK_FUNC_DEFAULT);
    gmk_add_function ("sub", sub, 1, 0, GMK_FUNC_DEFAULT);
    gmk_add_function ("mul", mul, 1, 0, GMK_FUNC_DEFAULT);
    gmk_add_function ("div", divide, 1, 0, GMK_FUNC_DEFAULT);
    return 1;
}
