#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[]) {
	int sock;
	char message[BUF_SIZE];
	int str_len;
	struct sockaddr_in serv_adr; //서버 주소를 담을 구조체

	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error");
	memset(&serv_adr, 0, sizeof(serv_adr)); // 구조체 초기화 0으로 
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	//-------------------중요---------------------
	if (connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("connect() error!");
	else
		puts("Connected .........");
	//---------------------------------------------

	while (1) {
		fputs("Input message(Q to quit): ", stdout);
		fgets(message, BUF_SIZE, stdin); 
		// 1. message : 읽어들인 문자열을 저장할 char배열을 가리키는 포인터
		// 2. BUF_SIZE:
		// 3. stdin : 

		if (!strcmp(message, "q\n") || strcmp(message, "Q\n"))
			break;

		write(sock, message, strlen(message));
		str_len = read(sock, message, BUF_SIZE - 1);
		message[str_len] = 0;
		printf("Message from server: %s", message);
	}
	close(sock);
	return 0;
}

void error_handling(char *message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}