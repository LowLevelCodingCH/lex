#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

#define ARGC_LEN 1

#include <lex/lex.h>

int main(int argc,
         char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Too few arguments\n");
        return 1;
    }

    switch (argc) {
    case 0:
    case 1:
        return 1;
    case 2:
        if (strcmp(argv[1], "rbase") == 0)
            lex_rbase();
        if (strcmp(argv[1], "gbase") == 0)
            lex_gbase();
        if (strcmp(argv[1], "repos") == 0)
            lex_repos();
        if (strcmp(argv[1], "init") == 0)
            lex_init();
        if (strcmp(argv[1], "rebase") == 0)
            lex_rebase(argc, argv);
        if (strcmp(argv[1], "rebase") == 0)
            lex_rebase(argc, argv);
        if (strcmp(argv[1], "unbase") == 0)
            lex_unbase();
        if (strcmp(argv[1], "enbase") == 0)
            lex_enbase(argc, argv);
        if (strcmp(argv[1], "debase") == 0)
            lex_debase();
        break;
    case 3:
        if (strcmp(argv[1], "newbase") == 0)
            lex_newbase(argv[2]);
        if (strcmp(argv[1], "pull") == 0)
            lex_pull(argc, argv);
        if (strcmp(argv[1], "commit") == 0)
            lex_commit(argc, argv);
        break;
    case 4:
        if (strcmp(argv[1], "rebase") == 0)
            lex_rebase(argc, argv);
        if (strcmp(argv[1], "enbase") == 0)
            lex_enbase(argc, argv);
        break;
    }

    return 0;
}
