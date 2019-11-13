#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>

// int main(int argc, char* argv[])
// {
// 	if (argc!=2){
// 		fprintf(stderr, "Usage: %s <command>\n", argv[0]);
// 		exit(1);
// 	}

// 	return system(argv[1]);
// }

int main(int argc, char* argv[]) {

	pid_t childPid;
	int status;

	if (argc!=2){
		fprintf(stderr, "Usage: %s <command>\n", argv[0]);
		exit(1);
	}

	char * command = argv[1];

	switch(childPid = fork()){
		case -1: 	// fork failed
			status = -1;
			break;
		
		case 0:		// child created -> sxecute command
			execl("/bin/sh", "sh", "-c", command, (char *) NULL);

		default:
			while(waitpid(childPid, &status, 0) == -1){
				if(errno != EINTR) {
					status = -1;
					break;
				}
			}
			break;
	}

	return status;
}