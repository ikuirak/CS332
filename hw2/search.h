#ifndef SEARCH_H
#define SEARCH_H
#include <sys/stat.h>
#include <limits.h>

typedef struct {
    int show_attrs, size_set, pattern_set, max_depth;
    long max_size;
    char *pattern;
} Options;

typedef int (*print_fn)(const char *path, const char *name,
                        const struct stat *st, int depth);


extern Options opts;


int traverse(const char *path, const char *name, int depth, print_fn print);
int print_name(const char *path, const char *name, const struct stat *st, int depth);
#endif