#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <sys/stat.h>

#define ARGC_LEN 1

#include <lex/lex.h>

int main(int argc,
         char **argv)
{
    switch (argc) {
    case 2:
        if (strcmp(argv[1], "rbase") == 0)
            lex_rbase();
        if (strcmp(argv[1], "init") == 0)
            lex_init();
        else return fprintf(stderr, "No valid command: %s\n", argv[1]);
        break;
    case 4:
    case 5:
        if (strcmp(argv[1], "rebase") == 0)
            lex_rebase(argc, argv);
        else return fprintf(stderr, "No valid command: %s\n", argv[1]);
        break;
    }
    return 0;
}
