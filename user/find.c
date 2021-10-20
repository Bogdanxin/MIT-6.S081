#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

/**
 * 1. 利用递归思路，每层目录都进行递归查看
 * 2. 利用 read 方法读取目录，并且查看每个目录中的节点
 * 3. 将节点 name 和 path 进行拼接
 * 4. 如果节点为文件并且 name 为 file_name，就将拼接后的名称打印出来
 *    如果为目录，就进一步递归查找
 * @param path 当前路径
 * @param file_name 需要查找的文件名
 */
void find(char* path, char* file_name) {

    int fd;
    char buf[512], *p;
    struct dirent de;
    struct stat st;

    // 首先判断是否可以 open path 路径，如果不能，打印错误
    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "error! cannot open %s\n", path);
        return;
    }

    // 查看 fd 描述符的状态，把状态写到 st 中
    if (fstat(fd, &st) < 0) {
        fprintf(2, "error! cannot open %s\n", path);
        // 关闭文件描述符
        close(fd);
        return;
    }

    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        if (de.inum == 0) {
            continue;
        }

        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;

        if (stat(buf, &st) < 0) {
            fprintf(2, "error: cannot stat %s\n", buf);
        }

        switch(st.type) {
            case T_FILE:
                if (strcmp(file_name, de.name) == 0) {
                    printf("%s\n", buf);
                }
                break;

            case T_DIR:
                if (strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0) {
                    find(buf, file_name);
                }
                break;
        }

    }
    
    close(fd);
    return;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(2, "error usage: find dir file\n");
        exit(1);
    }

    char* path = argv[1];
    char* file_name = argv[2];

    find(path, file_name);

    exit(0);
}