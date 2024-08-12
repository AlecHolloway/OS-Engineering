#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int time;
    if(argc == 1) {
        printf("error\n");
	exit(0);
    }
    else {
	time = atoi(argv[1]);
    }
    printf("%d\n", time);
    sleep(time);
    exit(0);
}


