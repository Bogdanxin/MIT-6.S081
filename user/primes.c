#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define READEND 0
#define WRITEEND 1
#define NUM 35
/**
 * 设置 pl 和 pr 两种管道，pl 代表从父进程创建的管道，pr 代表子进程的管道： pl | 当前进程 | pr 
 * 我们首先通过质数 2 进行筛选，如果能够整除 2 ，说明这个数一定不是质数，直接过滤掉，
 * 把剩下的数传到下一个子进程中，这时能够判断出来 3 肯定是质数
 * （或者说，只要是没有过滤掉的最小数，就一定是质数 eg：3 不能整除 2，并且 3 最靠近于 2，所以他一定是质数）
 * @param pl 左端管道传入的数据
 */
void child(int *pl) {
    int pr[2];
    int n;

    close(pl[WRITEEND]);
    int tmp = read(pl[READEND], &n, sizeof (int));
    //close(pl[READEND]);
    if (tmp == 0) {
        // printf("prime %d\n", n);
        exit(0);
    }

    pipe(pr);
    if (fork() == 0) {
        child(pr);
    } else {
        close(pr[READEND]);
        printf("prime %d\n", n, getpid());
        int prime = n;
        while (read(pl[READEND], &n, sizeof (int)) != 0) {
            if (n % prime != 0) {
                write(pr[WRITEEND], &n, sizeof (int));
            }
        }
        close(pr[WRITEEND]);
        wait((int *) 0);
        exit(0);
    }
}

int main(int argc, char* argv[]) {
    int pl[2];
    
    pipe(pl);
    if (fork() == 0) {
        child(pl);
    } else {
        close(pl[READEND]);
        for (int i = 2; i <= 35; i++) {
            write(pl[WRITEEND], &i, sizeof (int));
        }
        close(pl[WRITEEND]);
        wait((int*) 0);
        
    }
    exit(0);
}