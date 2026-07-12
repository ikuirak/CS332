#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "search.h"

static void indent(int depth)
{
    for (int i = 0; i < depth; i++)
        putchar('\t');
}

int print_name(const char *path, const char *name,
               const struct stat *st, int depth)
{
    (void)path; (void)st;      /* unused for now */
    indent(depth);
    printf("%s\n", name);
    return 0;
}

int traverse(const char *path, const char *name, int depth, print_fn print)
{
    struct stat st;
    struct dirent **entries;
    char child[PATH_MAX];

    if (lstat(path, &st) == -1) {
        perror(path);
        return -1;
    }

    if (!S_ISDIR(st.st_mode)) {          /* file: just print it */
        print(path, name, &st, depth);
        return 0;
    }

    print(path, name, &st, depth);       /* directory name on its own line */

    int n = scandir(path, &entries, NULL, alphasort);
    if (n == -1) {
        perror(path);
        return -1;
    }
    for (int i = 0; i < n; i++) {
        if (strcmp(entries[i]->d_name, ".") && strcmp(entries[i]->d_name, "..")) {
            snprintf(child, sizeof child, "%s/%s", path, entries[i]->d_name);
            traverse(child, entries[i]->d_name, depth + 1, print);
        }
        free(entries[i]);
    }
    free(entries);
    return 0;
}