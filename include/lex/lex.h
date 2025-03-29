int print_dirs(const char *path, int recursive)
{
    struct dirent *direntp = NULL;
    DIR *dirp = NULL;
    size_t path_len;

    if (!path)
        return -1;
    path_len = strlen(path);  

    if (!path || !path_len || (path_len > _POSIX_PATH_MAX))
        return -1;

    dirp = opendir(path);
    if (dirp == NULL)
        return -1;

    while ((direntp = readdir(dirp)) != NULL) {
        struct stat fstat;
        char full_name[_POSIX_PATH_MAX + 1];

        if ((path_len + strlen(direntp->d_name) + 1) > _POSIX_PATH_MAX)
            continue;

        strcpy(full_name, path);
        if (full_name[path_len - 1] != '/')
            strcat(full_name, "/");
        strcat(full_name, direntp->d_name);

        if ((strcmp(direntp->d_name, ".") == 0) ||
            (strcmp(direntp->d_name, "..") == 0))
            continue;

        if (stat(full_name, &fstat) < 0)
            continue;
        if (S_ISDIR(fstat.st_mode)) {
            printf(" * [%s]\n", full_name + 5);
            if (recursive)
                print_dirs(full_name, 1);
        }
    }

    (void)closedir(dirp);
    return 0;
}

int lex_tfa(int argc)
{
    fprintf(stderr, "Too few arguments\n  Expected %d, Got %d\n", ARGC_LEN, argc);
    return 1;
}

void lex_gbase(void)
{
    int fd = open(".lex/dev/BASE", O_RDONLY);
    off_t f_len = lseek(fd, 0, SEEK_END);

    lseek(fd, 0, SEEK_SET);

    char *buffer = (char*) malloc(f_len + 1);

    read(fd, buffer, f_len);

    printf("%s\n", buffer);

    close(fd);
    free(buffer);
}

void lex_rbase(void)
{
    int fd = open(".lex/main/BASE", O_RDONLY);
    off_t f_len = lseek(fd, 0, SEEK_END);

    lseek(fd, 0, SEEK_SET);

    char *buffer = (char*) malloc(f_len + 1);

    read(fd, buffer, f_len);

    printf("%s\n", buffer);

    close(fd);
    free(buffer);
}

void lex_enbase(int argc, char **argv)
{
    char *flag = "NONE";
    char *message = "NONE";
    char *rbs = (char*) malloc(100);

    if (argc == 4) {
        flag = argv[2];
        message = argv[3];
        free(rbs);
        rbs = (char*) malloc(strlen(message) + strlen("ENBASED") + strlen("Message: ") + 2);
        sprintf(rbs, "ENBASED\nMessage: %s", message);
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

    fd = open(".lex/dev/snapshot.tar", O_CREAT | O_RDWR);
    chmod(".lex/dev/snapshot.tar", 0777);
    write(fd, buffer, f_len);
    close(fd);

    fclose(fopen(".lex/dev/BASE", "w+"));

    fd = open(".lex/dev/BASE", O_WRONLY);
    if (strcmp(flag, "--message") == 0) write(fd, rbs, strlen(rbs));
    close(fd);

    free(buffer);
    free(rbs);
}

void lex_repobase(char *repo)
{
    char *spath = (char*) malloc(1000);

    system("tar cf snapshot.tar --exclude=.lex --exclude=snapshot.tar .");

    int fd = open("snapshot.tar", O_RDONLY);

    off_t f_len = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    char *buffer = (char*) malloc(f_len * 2 + 1);
    read(fd, buffer, f_len);

    close(fd);

    sprintf(spath, ".lex/%s/snapshot.tar", repo);

    fd = open(spath, O_CREAT | O_RDWR);
    chmod(spath, 0777);
    write(fd, buffer, f_len);
    close(fd);

    free(spath);
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

    fd = open(".lex/main/snapshot.tar", O_CREAT | O_RDWR);
    chmod(".lex/main/snapshot.tar", 0777);
    write(fd, buffer, f_len);
    close(fd);

    fclose(fopen(".lex/main/BASE", "w+"));

    fd = open(".lex/main/BASE", O_WRONLY);
    if (strcmp(flag, "--message") == 0) write(fd, rbs, strlen(rbs));
    close(fd);

    free(buffer);
    free(rbs);
}

void lex_derepobase(char *repo)
{
    char *spath = (char*) malloc(1000);
    sprintf(spath, "tar xf .lex/%s/snapshot.tar", repo);
    system(spath);
    free(spath);
}

void lex_debase(void)
{
    system("tar xf .lex/dev/snapshot.tar");
}

void lex_unbase(void)
{
    system("tar xf .lex/main/snapshot.tar");
}

void lex_pull(int argc, char **argv)
{
    if (strcmp(argv[2], "--main") == 0)
        lex_unbase();
    else if (strcmp(argv[2], "--dev") == 0)
        lex_debase();
    else
        lex_derepobase(argv[2] + 2);
}

void lex_commit(int argc, char **argv)
{
    if (strcmp(argv[2], "--main") == 0)
        lex_rebase(2, NULL);
    else if (strcmp(argv[2], "--dev") == 0)
        lex_enbase(2, NULL);
    else
        lex_repobase(argv[2] + 2);
}

void lex_repos(void)
{
    print_dirs(".lex", 0);
}

void lex_newbase(char *name)
{
    char *s = (char*) malloc(1000);
    sprintf(s, ".lex/%s", name);
    mkdir(s, 0777);
    free(s);
}

void lex_init(void)
{
    mkdir(".lex", 0777);
    mkdir(".lex/main", 0777);
    mkdir(".lex/dev", 0777);

    mkdir(".lex/testing", 0777);
    mkdir(".lex/pending", 0777);
    mkdir(".lex/trunk", 0777);
    mkdir(".lex/other", 0777);

    open(".lex/main/BASE", O_CREAT, S_IRUSR | S_IWUSR);
    chmod(".lex/main/BASE", 0777);

    open(".lex/dev/BASE", O_CREAT, S_IRUSR | S_IWUSR);
    chmod(".lex/dev/BASE", 0777);
}
