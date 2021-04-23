//
// Created by lxx on 2021/4/21.
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

void
do_find(char *path, char *pattern) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        exit(1);
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        exit(1);
    }

    if (st.type == T_FILE) {
        fprintf(2, "Usage: find directory pattern; %s is not a directory.\n", path);
        close(fd);
        exit(1);
    }

    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
        printf("find: path too long\n");
        close(fd);
        exit(1);
    }
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
            continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if (stat(buf, &st) < 0) {
            printf("find: cannot stat %s\n", buf);
            continue;
        }
        if (st.type == T_DIR) {
            do_find(buf, pattern);
            continue;
        }
        if (strcmp(de.name, pattern) == 0) {
            printf("%s\n", buf);
        }
    }
    close(fd);
}

int
main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(2, "Usage: find directory pattern; lack of args.\n");
        exit(1);
    }
    if (argc > 3) {
        fprintf(2, "Usage: find directory pattern; too much args.\n");
        exit(1);
    }
    char *path = argv[1];
    char *pattern = argv[2];
    do_find(path, pattern);
    exit(0);
}
