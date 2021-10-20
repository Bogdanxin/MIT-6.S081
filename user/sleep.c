#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char * argv[]) {
    if (argc != 2) {
        fprintf(2, "error !\n");
        exit(1);
    }

    int times = atoi( argv[1]);
    if (times < 0) {
        fprintf(2, "error !\n");
        exit(1);
    } else {
        fprintf(1, "time to sleep!\n");
        sleep(times);
        fprintf(1, "time to wake up\n");
    }
    exit(0);
} 