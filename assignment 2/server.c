#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>

#define BUF_SIZE 100
#define MAX_CLNT 2

void error_handling(char *msg);
void handling_child(int sig);

int main(int argc, char* argv[])
{
	int serv_sock;
	int clnt_sock[MAX_CLNT];

	int clnt_sock_1;
	int clnt_sock_2;

	int fd[2][2];
	int str_len, state;
	int i;
	int currentChildNum = 0;

	char buf[BUF_SIZE];

	struct sockaddr_in serv_adr;
	struct sockaddr_in clnt_adr[MAX_CLNT];
	struct sigaction act;

	pid_t pid[MAX_CLNT];
	socklen_t adr_sz;

	if (argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	/*signal handling*/
	act.sa_handler = handling_child;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	state = sigaction(SIGCHLD, &act, 0);

	/* Create server socket(listening socket) */
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);

	/* Initialize address information */
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	/* Allocate adress information */
	if (bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");

	/* Client can access server */
	if (listen(serv_sock, 2) == -1)
		error_handling("listen() error");

	/* Pipe */
	pipe(fd[0]);
	pipe(fd[1]);

	/* connect clients*/
	for (i = 0; i < MAX_CLNT; i++)
	{
		/* Accept client's request */
		adr_sz = sizeof(clnt_adr);
		clnt_sock[i] = accept(serv_sock, (struct sockaddr *)&clnt_adr[i], &adr_sz);

		if (clnt_sock[i] == -1)
			continue;
		else
			printf("* A client connected. Socket id: %d\n", clnt_sock[i]);
	}
	puts("Both clients connected");

	/*making child processes*/
	while (currentChildNum < MAX_CLNT)
	{
		pid[currentChildNum] = fork();

		if (pid[currentChildNum] == -1) //error
		{
			return -1;
		}
		else if (pid[currentChildNum] == 0) //child
		{
			close(serv_sock);
			printf("* Child process %d (pid=%d) created for client with socket id : %d\n", currentChildNum + 1, getpid(), clnt_sock[currentChildNum]);

			for (;;)
			{
				read(fd[currentChildNum][0], buf, BUF_SIZE);
				if (strcmp(buf, "Q\n") == 0 || strcmp(buf, "q\n") == 0)
				{
					puts("client disconnected....");
					write(clnt_sock[currentChildNum], &buf, BUF_SIZE);
					close(clnt_sock[currentChildNum]);
					sleep(1);
					exit(0);
				}
				write(clnt_sock[currentChildNum], &buf, BUF_SIZE);
				printf("* Child process (pid=%d) sending the message\n", getpid());
			}
		}
		currentChildNum++;
		sleep(1);
	}
	if (currentChildNum == MAX_CLNT)
	{
		close(clnt_sock[0]);
		close(clnt_sock[1]);

		for (;;) {
			printf("Input message (for sending): ");
			fgets(buf, BUF_SIZE, stdin);
			if (strcmp(buf, "Q\n") == 0 || strcmp(buf, "q\n") == 0)
			{
				write(fd[0][1], buf, BUF_SIZE);
				write(fd[1][1], buf, BUF_SIZE);

				waitpid(-1, NULL, 0);
				exit(0);
			}

			puts("* Piping the message to child process 1");
			puts("* Piping the message to child process 2");

			write(fd[0][1], buf, BUF_SIZE);
			write(fd[1][1], buf, BUF_SIZE);
			/*for terminal print*/
			sleep(1);
		}
	}
	close(serv_sock);
	return 0;
}

void handling_child(int sig)
{
	pid_t pid;
	int status;
	pid = waitpid(-1, &status, WNOHANG);
}

void error_handling(char *msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}