#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 50
void error_handling(char *message);

int main(int argc, char argv[]) {
	int sd;
	FILE* fp;

	char buf[BUF_SIZE];
	int read_cnt;
	struct sockaddr_in serv_adr;
	if (argc != 3) {
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	fp = fopen("receive.dat", "wb");
	sd = socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	connect(sd, (struct sockaddr*)&serv_adr, sizeof(serv_adr));

	while ((read_cnt = read(sd, buf, BUF_SIZE)) != 0)
		//�������� ���� �ڷḦ buf�� �о��
		fwrite((void*)buf, 1, read_cnt, fp);
		//buf�� ����Ǿ��ִ� data�� �����Ѵ�

	puts("Received file data");
	write(sd, "Thank you", 10);
	// ��� ������ �޴µ� �����ϸ� thank you�� ����
	fclose(fp);
	close(sd);
	//socket �ݱ�
	return 0;
}

void error_handling(char *buf)
{
	fputs(buf, stderr);
	fputc('\n', stderr);
	exit(1);
}