#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 50
void error_handling(char *message);

int main(int argc, char *argv[]) {
	int serv_sd, clnt_sd;
	FILE *fp;
	char buf[BUF_SIZE];
	int read_cnt;

	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t clnt_adr_sz;

	if (argc != 2) {
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}

	fp = fopen("file_server.c", "rb");
	serv_sd = socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	bind(serv_sd, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
	listen(serv_sd, 5);

	clnt_adr_sz = sizeof(clnt_adr);
	clnt_sd = accept(serv_sd, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);

	while (1) {
		read_cnt = fread((void*)buf, 1, BUF_SIZE, fp);
		/*
		fread() : 파일을 통해 데이터를 읽어 들임
		(void*)buf : 파일 내용을 읽어 들일 메모리 포인터
		1 : 데이터 하나의 크기
		BUF_SIZ: 읽어들일 데이터의 개수
		fp: 대상 파일 스트림

		리턴값: 읽어들인 데이터 개수를 반환하며, 오류가 발생하면 -1 반환
		*/
		if (read_cnt < BUF_SIZE) {
			write(clnt_sd, buf, read_cnt);
			break;
		}
		write(clnt_sd, buf, BUF_SIZE);
		/*
		write() : open함수로 열기를 한 파일에 쓰기
		clnt_sd: 파일 descriptor
		buf: 파일에 쓰기를 할 내용을 담은 버퍼
		BUF_SIZE: 쓰기할 바이트 개수
		*/
	}
	shutdown(clnt_sd, SHUT_WR);
	read(clnt_sd, buf, BUF_SIZE);
	/*
	read(): 파일의 데이터를 입력받는 함수
	clnt_sd: 데이터를 전송해주는 대상을 가리키는 파일 descriptor
	buf: 수신한 데이터를 저장할 버퍼를 가리키는 포인터
	BUF_SIZE: 수신할 최대 바이트 수
	*/
	printf("Message from client: %s \n", buf);

	fclose(fp);
	close(clnt_sd); close(serv_sd);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}