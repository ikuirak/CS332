#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "search.h"

Options opts;       /* global options */

int main(int argc, char *argv[]){
    int c, show_attrs = 0, size_set = 0, pattern_set = 0, max_depth = 0;
    long max_size = 0;
    char *pattern = NULL;
    const char *start = ".";

    while ((c = getopt(argc, argv, "Ss:f:")) != -1) {
        switch (c){
        case 's':
            opts.size_set = 1;
            opts.max_size = atol(optarg);
            break;
        case 'S':
            opts.show_attrs = 1;
            break;
        case 'f':
            opts.pattern_set = 1;
            opts.pattern = optarg;                   /* first arg */
            opts.max_depth = atoi(argv[optind]);     /* second arg (grabs manually)*/
            optind++;                           
            break;
        default:
            fprintf(stderr, "Usage %s [-S] [-s size] [-f pattern depth] [dir]\n", argv[0]);
            exit(1);    
        }
    }
    if (optind < argc)
        start = argv[optind];

    return traverse(start, start, 0, print_name) == -1 ? 1 : 0;

    return 0;
}