#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 50
void error_handling(char *message);

int main(int argc, char *argv[]) {
	int sd;
	FILE *fp;

	char buf[BUF_SIZE];
	int read_cnt;
	struct sockaddr_in serv_adr;
	
	if (argc != 3) {
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}

	fp = fopen("receive.dat", "wb");
	sd = socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	connect(sd, (struct sockaddr*)&serv_adr, sizeof(serv_adr));

	while ((read_cnt = read(sd, buf, BUF_SIZE)) != 0)
		fwrite((void*)buf, 1, read_cnt, fp);
	/*
	fwrite(): 파일에 버퍼 내용을 쓰기
	(void*)buf: 파일 내용에 출력할 메모리 포인터
	1: 데이터 하나의 크기
	read_cnt: 쓰기할 데이터의 개수
	fp: 대상 파일 스트림

	리턴값: 출력한 데이터 개수를 반환하며, 오류가 발생하면 -1을 반환
	
	*/

	puts("Received file data");
	write(sd, "Thank you", 10);
	fclose(fp);
	close(sd);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}