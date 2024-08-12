#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    int p[2];
    pipe(p);
    int pid = fork();
    if (pid > 0) {
    	char buff[1] = "H";
	write(p[1], buff, 1);
	wait(0);

	char newBuff[1];
	read(p[0], newBuff, 1);
	printf("%d: received pong\n", getpid());
    }
    else if (pid == 0) {
	char buff1[1];
	read(p[0], buff1, 1);
	printf("%d: received ping\n", getpid());

	char buff2[1] = "D";
	write(p[1], buff2, 1);
	close(p[0]);
	close(p[1]);
    }
    
    return 0;
}
