#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
extern char **environ;
int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		printf("Usage %s <1st arg> <2st arg>\n", argv[0]);
		exit(1);
	}

        pid_t pid;
        int ret;

        ret = execlp(argv[1], argv[2], NULL);
        fprintf(stderr, "execl error: %s\n",strerror(errno));
        printf("execl:%d\n",ret);
        return 0;
}

