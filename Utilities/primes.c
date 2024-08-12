#include "kernel/types.h" 
#include "kernel/stat.h"
#include "user/user.h"

void runProcess(int d, int pipeR);

int main() {
	int p[2];
	pipe(p);
	int arr[34] = {2, 3, 4, 5, 6, 7, 8, 9, 10,
	              11, 12, 13, 14, 15, 16, 17, 
		      18, 19, 20, 21, 22, 23, 24, 
		      25, 26, 27, 28, 29, 30, 31, 
		      32, 33, 34, 35 };
	
	int num = 0;
	int divisible = arr[num];
	printf("prime %d\n", arr[num]);
	num++;
	if (arr[num] % divisible != 0) {
	    int pid = fork();
	    if (pid > 0) {
		    close(p[0]);
	        for(int x = num; x < sizeof(arr) / sizeof(arr[0]); x++) { 
	            if(arr[x] % divisible != 0) {
			        write(p[1], &arr[x], sizeof(arr[x]));
	            }
	        }
	        close(p[1]);
	 	    wait(0);	//wait for the child to finish
		    exit(0);
	    }
	    else if(pid == 0) {
		    close(p[1]);
		    runProcess(divisible, p[0]);
		    close(p[0]);
		    exit(0);
	    }
	}
	return 1;
}


void runProcess(int divisible, int pipeR) {
    int buff = 0;
    
    //find next prime number if no more then exit
    while(read(pipeR, &buff, sizeof(buff)) > 0) {
        if(buff % divisible != 0) {
	        divisible = buff;
            printf("prime %d\n", buff);
	        break;
        }
	    if(buff > 35) {
	        exit(0);
	    }
    }
    if(buff == 0) {
        exit(0);
    } 
    //once prime number is found create pipe and fork
    int p[2];
    pipe(p);
    int pid = fork();
    if(pid > 0) {
        while(read(pipeR, &buff, sizeof(buff)) > 0) {
            if(buff % divisible != 0) {
                write(p[1], &buff, sizeof(buff));
            }
        }
	    close(p[1]);
	    wait(0);
    }
    if(pid == 0) {
	    close(p[1]);
        runProcess(divisible, p[0]);
        close(p[0]);
	    exit(0);
    }
}
