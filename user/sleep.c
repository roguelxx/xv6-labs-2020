//
// Created by lxx on 2021/4/20.
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(2, "Usage: sleep time; lack of args.\n");
        exit(1);
    }
    if (argc > 2) {
        fprintf(2, "Usage: sleep time; too much args.\n");
        exit(1);
    }
    int time = atoi(argv[1]);
    sleep(time);
    exit(0);
}

