#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define READEND 0
#define WRITEEND 1

int main(int argc, char* argv[]) {
    if (argc != 1) {
        fprintf(2, "error ! usage : pingpong\n");
        exit(1);
    }

    char buf[1];
    int p[2];
    int pid = fork();
    pipe(p);
    if (pid == 0) {
        close(p[WRITEEND]);
        read(p[READEND], buf, 1);
        printf("%d: received ping\n", getpid());
        close(p[READEND]);
        write(p[WRITEEND], "1", 1);
        close(p[WRITEEND]);
    } else {
        close(p[READEND]);
        write(p[WRITEEND], "1", 1);
        close(p[WRITEEND]);
        wait(0);
        read(p[READEND], buf, 1);
        printf("%d: received pong\n", getpid());
        close(p[READEND]);
    }

    exit(0);
}