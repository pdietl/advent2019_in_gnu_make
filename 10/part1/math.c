#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>

#include <gnumake.h>

#define YES 1
#define NO 0

#define ERROR_PREFIX "ARITHMETIC ERROR: "

typedef struct {
    const char *filenm;
    unsigned long lineno;
    unsigned long offset;
} floc;

extern const floc *reading_file;

int plugin_is_GPL_compatible;

void throw_error(const char *str, int *error, const char *func)
{
    printf("function %s, string '%s'\n", func, str);
    gmk_floc fl = { reading_file->filenm, reading_file->lineno };
    const char *format_string = "$(error " ERROR_PREFIX "function (%s) String '%s' in not representable as a long!)";
    int size = snprintf(NULL, 0, format_string, func, str);
    char *s = gmk_alloc(size + 1);
    sprintf(s, format_string, func, str);
    gmk_eval(s, &fl);
    gmk_free(s);
    *error = YES;
}

long strtol_or_die(const char *str, int *error, const char *funcname)
{
    char *end;
    long ret;
    errno = 0;
    
    ret = strtol(str, &end, 0);

    if (*end != '\0' ||
        errno == EINVAL ||
        errno == ERANGE ||
        *str == '\0') {

        throw_error(str, error, funcname);
        return 0;
    }

    *error = NO;
    return ret;
}

double strtod_or_die(const char *str, int *error, const char *funcname)
{
    char *end;
    double ret;
    errno = 0;
    
    ret = strtod(str, &end);

    if (*end != '\0' ||
        errno == EINVAL ||
        errno == ERANGE ||
        *str == '\0') {

        throw_error(str, error, funcname);
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
        temp = strtol_or_die(n, &err, __func__);
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
        total = strtol_or_die(n, &err, __func__);
        if (err == YES)
            return NULL;
        n = strtok(NULL, " \t");
        while (n) {
            temp = strtol_or_die(n, &err, __func__);
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
        temp = strtol_or_die(n, &err, __func__);
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
        total = strtol_or_die(n, &err, "div");
        if (err == YES)
            return NULL;
        n = strtok(NULL, " \t");
        while (n) {
            temp = strtol_or_die(n, &err, "div");
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

char *modulus(const char *nm, unsigned argc, char **argv)
{
    long a, b;
    int err = 0;
    a = strtol_or_die(argv[0], &err, __func__);
    if (err == YES)
        return NULL;
    b = strtol_or_die(argv[1], &err, __func__);
    if (err == YES)
        return NULL;

    long res = a % b;

    int size = snprintf(NULL, 0, "%ld", res) + 1;
    char *buf = gmk_alloc(size);
    snprintf(buf, size, "%ld", res);
    return buf;
}
char *eq(const char *nm, unsigned argc, char **argv)
{
    long a, b;
    int err = 0;
    a = strtol_or_die(argv[0], &err, __func__);
    if (err == YES)
        return NULL;
    b = strtol_or_die(argv[1], &err, __func__);
    if (err == YES)
        return NULL;

    char *res = a == b ? "1" : "";

    int size = snprintf(NULL, 0, "%s", res) + 1;
    char *buf = gmk_alloc(size);
    snprintf(buf, size, "%s", res);
    return buf;
}
 
char *ne(const char *nm, unsigned argc, char **argv)
{
    long a, b;
    int err = 0;
    a = strtol_or_die(argv[0], &err, __func__);
    if (err == YES)
        return NULL;
    b = strtol_or_die(argv[1], &err, __func__);
    if (err == YES)
        return NULL;

    char *res = a != b ? "1" : "";

    int size = snprintf(NULL, 0, "%s", res) + 1;
    char *buf = gmk_alloc(size);
    snprintf(buf, size, "%s", res);
    return buf;
}
 
char *lt(const char *nm, unsigned argc, char **argv)
{
    long a, b;
    int err = 0;
    a = strtol_or_die(argv[0], &err, __func__);
    if (err == YES)
        return NULL;
    b = strtol_or_die(argv[1], &err, __func__);
    if (err == YES)
        return NULL;

    char *res = a < b ? "1" : "";

    int size = snprintf(NULL, 0, "%s", res) + 1;
    char *buf = gmk_alloc(size);
    snprintf(buf, size, "%s", res);
    return buf;
}

char *le(const char *nm, unsigned argc, char **argv)
{
    long a, b;
    int err = 0;
    a = strtol_or_die(argv[0], &err, __func__);
    if (err == YES)
        return NULL;
    b = strtol_or_die(argv[1], &err, __func__);
    if (err == YES)
        return NULL;

    char *res = a <= b ? "1" : "";

    int size = snprintf(NULL, 0, "%s", res) + 1;
    char *buf = gmk_alloc(size);
    snprintf(buf, size, "%s", res);
    return buf;
}

char *gt(const char *nm, unsigned argc, char **argv)
{
    long a, b;
    int err = 0;
    a = strtol_or_die(argv[0], &err, __func__);
    if (err == YES)
        return NULL;
    b = strtol_or_die(argv[1], &err, __func__);
    if (err == YES)
        return NULL;

    char *res = a > b ? "1" : "";

    int size = snprintf(NULL, 0, "%s", res) + 1;
    char *buf = gmk_alloc(size);
    snprintf(buf, size, "%s", res);
    return buf;
}

char *gt_f(const char *nm, unsigned argc, char **argv)
{
    long a, b;
    int err = 0;
    a = strtol_or_die(argv[0], &err, __func__);
    if (err == YES)
        return NULL;
    b = strtol_or_die(argv[1], &err, __func__);
    if (err == YES)
        return NULL;

    char *res = a > b ? "1" : "";

    int size = snprintf(NULL, 0, "%s", res) + 1;
    char *buf = gmk_alloc(size);
    snprintf(buf, size, "%s", res);
    return buf;
}

char *ge(const char *nm, unsigned argc, char **argv)
{
    double a, b;
    int err = 0;
    a = strtod_or_die(argv[0], &err, __func__);
    if (err == YES)
        return NULL;
    b = strtod_or_die(argv[1], &err, __func__);
    if (err == YES)
        return NULL;

    char *res = a >= b ? "1" : "";

    int size = snprintf(NULL, 0, "%s", res) + 1;
    char *buf = gmk_alloc(size);
    snprintf(buf, size, "%s", res);
    return buf;
}

char *absolute(const char *nm, unsigned argc, char **argv)
{
    long a;
    int err = 0;
    a = strtol_or_die(argv[0], &err, "abs");
    if (err == YES)
        return NULL;

    if (a < 0)
        a = -a;

    int size = snprintf(NULL, 0, "%ld", a) + 1;
    char *buf = gmk_alloc(size);
    snprintf(buf, size, "%ld", a);
    return buf;
}


char *itan2(const char *nm, unsigned argc, char **argv)
{
    long x, y;
    int err = 0;
    x = strtol_or_die(argv[0], &err, "itan2");
    if (err == YES)
        return NULL;
    y = strtol_or_die(argv[1], &err, "itan2");
    if (err == YES)
        return NULL;

    double res = atan2(y, x);

    int size = snprintf(NULL, 0, "%lf", res) + 1;
    char *buf = gmk_alloc(size);
    snprintf(buf, size, "%lf", res);
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
    gmk_add_function ("mod", modulus, 2, 0, GMK_FUNC_DEFAULT);
    gmk_add_function ("eq", eq, 2, 0, GMK_FUNC_DEFAULT);
    gmk_add_function ("ne", ne, 2, 0, GMK_FUNC_DEFAULT);
    gmk_add_function ("gt", gt, 2, 0, GMK_FUNC_DEFAULT);
    gmk_add_function ("ge", ge, 2, 0, GMK_FUNC_DEFAULT);
    gmk_add_function ("lt", lt, 2, 0, GMK_FUNC_DEFAULT);
    gmk_add_function ("le", le, 2, 0, GMK_FUNC_DEFAULT);
    gmk_add_function ("abs", absolute, 1, 0, GMK_FUNC_DEFAULT);
    gmk_add_function ("atan2", itan2, 2, 0, GMK_FUNC_DEFAULT);
    return 1;
}
