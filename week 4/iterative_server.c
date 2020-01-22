#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char* message);

int main(int argc, char *argv[]) {
	int serv_sock, clnt_sock;
	char message[BUF_SIZE];
	int str_len, i;

	struct sockaddr_in serv_adr;
	struct sockaddr_in clnt_adr;
	socklen_t clnt_adr_sz;

	if (argc != 2) {//argc는 port번호와 ~를 return 받음 따라서 2개가 아니면 다시 return 받아라
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1) 
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY); // INADDR_ANY : 자신의 ip가 들어감
	serv_adr.sin_port = htons(atoi(argv[1]));
	//주소를 담은 ~ 초기화

	if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");

	if (listen(serv_sock, 5) == -1) // listen함수부터 소켓 만들어짐
		error_handling("listen() error");
	// 이제 client에서 오는 connect 형을 받을 수 있다.

	clnt_adr_sz = sizeof(clnt_adr);

	for (i = 0; i < 5; i++) {
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz); // 대기열에 클라이언트가 있으면 바로 넣음
		if (clnt_sock == -1)
			error_handling("accept() error");
		else
			printf("Connected client %d \n", i + 1);

		while (str_len = read(clnt_sock, message, BUF_SIZE) != 0)
			write(clnt_sock, message, str_len);

		close(clnt_sock);
	}
	close(serv_sock);
	return 0;
}

void error_handling(char *message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}