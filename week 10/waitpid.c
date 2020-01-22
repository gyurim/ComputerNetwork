#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
	int status; // �ڽ��� ��ȯ�� ���� ������ ����
	pid_t pid = fork();
	if (pid == 0) {
		sleep(15);
		return 24;
	}
	else {
		while (!waitpid(-1, &status, WNOHANG))
		{
			//waitpid(-1, ? ,?) => �ƹ� ���μ����� �Ҹ�Ǳ⸦ ��ٸ�
			// nonblocking 
			sleep(3);
			puts("sleep 3sec");
		}

		if (WIFEXITED(status))
			printf("Child send %d \n", WEXITSTATUS(status));
	}
	return 0;
}