#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    int c, show_attrs = 0, size_set = 0, pattern_set = 0, max_depth = 0;
    long max_size = 0;
    char *pattern = NULL;
    const char *start = ".";

    while ((c = getopt(argc, argv, "Ss:f:")) != -1) {
        switch (c){
        case 's':
            size_set = 1;
            max_size = atol(optarg);
            break;
        case 'S':
            show_attrs = 1;
            break;
        case 'f':
            pattern_set = 1;
            pattern = optarg;                   /* first arg */
            max_depth = atoi(argv[optind]);     /* second arg (grabs manually)*/
            optind++;                           
            break;
        default:
            fprintf(stderr, "Usage %s [-S] [-s size] [-f pattern depth] [dir]\n", argv[0]);
            exit(1);    
        }
    }
    if (optind < argc)
        start = argv[optind];

    /* temp */
    printf("start=%s S=%d s=%d(%ld) f=%d(%s,%d)\n",
            start, show_attrs, size_set, max_size,
            pattern_set, pattern ? pattern : "-", max_depth);
    return 0;
}