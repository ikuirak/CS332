#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "search.h"
#include <time.h>
#include <unistd.h>

static void indent(int depth)
{
    for (int i = 0; i < depth; i++)
        putchar('\t');
}

static void mode_to_string(mode_t m, char *buf)
{
    buf[0] = (m & S_IRUSR) ? 'r' : '-';
    buf[1] = (m & S_IWUSR) ? 'w' : '-';
    buf[2] = (m & S_IXUSR) ? 'x' : '-';
    buf[3] = (m & S_IRGRP) ? 'r' : '-';
    buf[4] = (m & S_IWGRP) ? 'w' : '-';
    buf[5] = (m & S_IXGRP) ? 'x' : '-';
    buf[6] = (m & S_IROTH) ? 'r' : '-';
    buf[7] = (m & S_IWOTH) ? 'w' : '-';
    buf[8] = (m & S_IXOTH) ? 'x' : '-';
    buf[9] = '\0';
}

static int check_filters(const char *name, const struct stat *st, int depth)
{
    (void)name; (void)depth;             /* used in step 6 */
    if (opts.size_set && st->st_size > opts.max_size)
        return 0;
    return 1;
}

int print_name(const char *path, const char *name,
               const struct stat *st, int depth)
{
    indent(depth);
    if (S_ISLNK(st->st_mode)) {
        char target[PATH_MAX + 1];
        ssize_t n = readlink(path, target, PATH_MAX);
        if (n >= 0) {
            target[n] = '\0';            /* readlink wont terminate */
            printf("%s (%s)\n", name, target);
            return 0;
        }
    }
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
        if (check_filters(name, &st, depth))
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

int print_attrs(const char *path, const char *name,
                const struct stat *st, int depth)
{
    char perms[10], timebuf[64];
    long size = S_ISDIR(st->st_mode) ? 0 : (long)st->st_size;

    mode_to_string(st->st_mode, perms);
    strftime(timebuf, sizeof timebuf, "%a %a %e %H:%M:%S %Y"
            localtime(&st->st_atime));

    indent(depth);
    /* symlink target handling same as print_name */
    print("%s (%ld, %s, %s)\n", name, size, perms, timebuf);
    return 0;
}
