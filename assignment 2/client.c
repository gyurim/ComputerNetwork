#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 100
void error_handling(char *msg);

int main(int argc, char **argv)
{
	char msg_buf[BUF_SIZE];

	int str_len;
	int sock;

	struct sockaddr_in serv_adr;

	if (argc != 3)
	{
		printf("Usage : %s, <IP>, <port>\n", argv[0]);
		exit(1);
	}
	/* Create socket */
	sock = socket(PF_INET, SOCK_STREAM, 0);

	/* Initialize server's address information */
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	/* Request connection to server */
	if (connect(sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("connect() error");
	else
		puts("Connected..............");

	for (;;){
		str_len = read(sock, msg_buf, BUF_SIZE);
		if (strcmp(msg_buf, "Q\n") == 0 || strcmp(msg_buf, "q\n") == 0)
		{
			close(sock); 
			return 0;
		}
		printf("Received : %s", msg_buf);
	}
}

void error_handling(char *msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}