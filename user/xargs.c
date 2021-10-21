#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"
#define STD_INPUT 0

int main(int argc, char *argv[]){
    char* paramv[MAXARG];
	char buf[1024];
	int read_line;
	int param_index = 0;

	char* command = argv[1];
	for (int i = 1; i < argc; i++) {
		paramv[param_index++] = argv[i]; 
	}
	while ((read_line = read(STD_INPUT, buf, 1024)) > 0) {
		if (fork() == 0){
			int tmp_index = 0;
			char* tmp_arg = (char*) malloc (sizeof(buf));
			for (int i = 0; i < read_line; i++) {
				if (buf[i] == ' ' || buf[i] == '\n') {
					tmp_arg[tmp_index] = 0;
					paramv[param_index++] = tmp_arg;
					tmp_index = 0;
					tmp_arg = (char*) malloc (sizeof(buf));
				} else {
					tmp_arg[tmp_index++] = buf[i];
				}
			}
			tmp_arg[tmp_index] = 0;
			paramv[param_index] = 0;
			exec(command, paramv);
		} else {
			wait(0);
		}
	}

	exit(0);
}