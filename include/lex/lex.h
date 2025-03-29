int lex_tfa(int argc)
{
    fprintf(stderr, "Too few arguments\n  Expected %d, Got %d\n", ARGC_LEN, argc);
    return 1;
}

void lex_rbase(void)
{
    int fd = open(".lex/BASE", O_RDONLY);
    off_t f_len = lseek(fd, 0, SEEK_END);

    lseek(fd, 0, SEEK_SET);

    char *buffer = (char*) malloc(f_len + 1);

    read(fd, buffer, f_len);

    printf("%s\n", buffer);

    close(fd);
    free(buffer);
}

void lex_rebase(int argc, char **argv)
{
    char *fname = argv[2];
    char *flag;
    char *message = "NONE";
    char *rbs = (char*) malloc(1);

    if (argc == 5) flag = argv[3];
    if (argc == 5) message = argv[4];

    if (strcmp(flag, "NONE") != 0 && strcmp(flag, "--message") != 0) {
        fprintf(stderr, "No valid flag found\n");
        return;
    }

    char *ename = (char*) malloc((strlen(fname) + strlen(".lex")) * 2);
    if (argc == 5) rbs = (char*) malloc(strlen(message) + strlen(fname) + strlen("REBASED: ") + strlen("Message: ") + 2);

    int fd = open(fname, O_RDONLY);
    off_t f_len = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    char *buffer = (char*) malloc(f_len * 2 + 1);
    read(fd, buffer, f_len);
    close(fd);

    sprintf(ename, "%s/%s", ".lex", fname);
    if (strcmp(flag, "--message") == 0) sprintf(rbs, "REBASED: %s\nMessage: %s", fname, message);

    fd = open(ename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    chmod(ename, 0777);
    write(fd, buffer, f_len);
    close(fd);

    fd = open(".lex/BASE", O_WRONLY);
    if (strcmp(flag, "--message") == 0) write(fd, rbs, strlen(rbs));
    close(fd);

    free(buffer);
    free(ename);
    free(rbs);
}

void lex_init(void)
{
    mkdir(".lex", 0777);
    close(open(".lex/BASE", O_CREAT, S_IRUSR | S_IWUSR));
    chmod(".lex/BASE", 0777);
    printf("Initialized LEX dir (.lex)\n");
}
