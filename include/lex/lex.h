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
    char *flag = "NONE";
    char *message = "NONE";
    char *rbs = (char*) malloc(100);

    if (argc == 4) {
        flag = argv[2];
        message = argv[3];
        free(rbs);
        rbs = (char*) malloc(strlen(message) + strlen("REBASED") + strlen("Message: ") + 2);
        sprintf(rbs, "REBASED\nMessage: %s", message);
    }

    if (strcmp(flag, "NONE") != 0 && strcmp(flag, "--message") != 0) {
        fprintf(stderr, "No valid flag found\n");
        return;
    }

    system("tar cf snapshot.tar --exclude=.lex --exclude=snapshot.tar .");

    int fd = open("snapshot.tar", O_RDONLY);

    off_t f_len = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    char *buffer = (char*) malloc(f_len * 2 + 1);
    read(fd, buffer, f_len);

    close(fd);

    fd = open(".lex/snapshot.tar", O_CREAT | O_RDWR);
    chmod(".lex/snapshot.tar", 0777);
    write(fd, buffer, f_len);
    close(fd);

    fd = open(".lex/BASE", O_WRONLY);
    if (strcmp(flag, "--message") == 0) write(fd, rbs, strlen(rbs));
    close(fd);

    free(buffer);
    free(rbs);
}

void lex_init(void)
{
    mkdir(".lex", 0777);
    open(".lex/BASE", O_CREAT, S_IRUSR | S_IWUSR);
    chmod(".lex/BASE", 0777);
    printf("Initialized LEX dir (.lex)\n");
}
