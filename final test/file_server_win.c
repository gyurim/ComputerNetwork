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
	// 서버 소캣을 통해 클라이언트의 접속 요청을 확인하도록 설정
	// 클라이언트 소캣으로 데이터를 송수신
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
	//udp는 socket(PF_INET< SOCK_DGRAM, 0);

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	bind(serv_sd, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
	listen(serv_sd, 5);

	while (1)
	{
		read_cnt = fread((void*)buf, 1, BUF_SIZE, fp);
		//스트림 fp에서 BUF_SIZE개의 원소를 가지는 배열을 읽어온다.
		// 각 원소의 크기는 1 byte이고 fp가 buf가 가리키는 배열에 넣게된다. 
		// 1 byte 씩 BUF_SIZE 번 만큼 읽음 
		// 성공: 읽어들인 원소의 개수가 리턴됨
		
		if (read_cnt < BUF_SIZE) {
			//만약 반환값이 BU_SIZE보다 작으면 한번에 모든 데이터를 전송한다는 뜻
			write(clnt_sd, buf, read_cnt);
			break;
		}
		//만약 read_cnt가 BUF_SIZE보다 크거나 같다면
		//여러번 반복해서 전송한다.
		write(clnt_sd, buf, BUF_SIZE);
	}

	shutdown(clnt_sd, SHUT_WR);
	// clnt 소켓의 write 스트림을 close
	// clnt 소켓은 data 전송을 못한다
	read(clnt_sd, buf, BUF_SIZE);
	// clnt 소켓은 server로부터 전송된 자료를 읽어들임
	// close 되기 전에 더 이상 보낼것 없는지 마지막으로 buf 점검하는 과정임
	// buf에 저장되어있는 건 없는지 확인
	printf("message from client: %s \n", buf);

	fclose(fp);
	// 모든 data가 다 전송되었다면 close
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