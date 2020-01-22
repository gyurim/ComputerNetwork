#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 50
void error_handling(char *message);

int main(int argc, char* argv[])
{
	int serv_sd, clnt_sd;
	// ���� ��Ĺ�� ���� Ŭ���̾�Ʈ�� ���� ��û�� Ȯ���ϵ��� ����
	// Ŭ���̾�Ʈ ��Ĺ���� �����͸� �ۼ���
	FILE* fp;
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
	//udp�� socket(PF_INET< SOCK_DGRAM, 0);

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	bind(serv_sd, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
	listen(serv_sd, 5);

	while (1)
	{
		read_cnt = fread((void*)buf, 1, BUF_SIZE, fp);
		//��Ʈ�� fp���� BUF_SIZE���� ���Ҹ� ������ �迭�� �о�´�.
		// �� ������ ũ��� 1 byte�̰� fp�� buf�� ����Ű�� �迭�� �ְԵȴ�. 
		// 1 byte �� BUF_SIZE �� ��ŭ ���� 
		// ����: �о���� ������ ������ ���ϵ�
		
		if (read_cnt < BUF_SIZE) {
			//���� ��ȯ���� BU_SIZE���� ������ �ѹ��� ��� �����͸� �����Ѵٴ� ��
			write(clnt_sd, buf, read_cnt);
			break;
		}
		//���� read_cnt�� BUF_SIZE���� ũ�ų� ���ٸ�
		//������ �ݺ��ؼ� �����Ѵ�.
		write(clnt_sd, buf, BUF_SIZE);
	}

	shutdown(clnt_sd, SHUT_WR);
	// clnt ������ write ��Ʈ���� close
	// clnt ������ data ������ ���Ѵ�
	read(clnt_sd, buf, BUF_SIZE);
	// clnt ������ server�κ��� ���۵� �ڷḦ �о����
	// close �Ǳ� ���� �� �̻� ������ ������ ���������� buf �����ϴ� ������
	// buf�� ����Ǿ��ִ� �� ������ Ȯ��
	printf("message from client: %s \n", buf);

	fclose(fp);
	// ��� data�� �� ���۵Ǿ��ٸ� close
	close(clnt_sd);
	close(serv_sd);
	return 0;
}

void error_handling(char *buf)
{
	fputs(buf, stderr);
	fputc('\n', stderr);
	exit(1);
}