#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
	int status; // 자식이 반환한 값을 저장할 변수
	pid_t pid = fork();
	if (pid == 0) {
		sleep(15);
		return 24;
	}
	else {
		while (!waitpid(-1, &status, WNOHANG))
		{
			//waitpid(-1, ? ,?) => 아무 프로세스나 소멸되기를 기다림
			// nonblocking 
			sleep(3);
			puts("sleep 3sec");
		}

		if (WIFEXITED(status))
			printf("Child send %d \n", WEXITSTATUS(status));
	}
	return 0;
}