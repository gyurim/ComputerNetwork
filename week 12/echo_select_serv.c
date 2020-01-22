#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 100
void error_handling(char *buf);

int main(int argc, char *argv[]) {
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	struct timeval timeout;
	fd_set reads, cpy_reads;

	socklen_t adr_sz;
	int fd_max, str_len, fd_num, i;
	char buf[BUF_SIZE];
	if (argc != 2) {
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	FD_ZERO(&reads);
	FD_SET(serv_sock, &reads);
	//reads: 관찰대상 목록 
	//&reads 에는 3번이 등록될 것이다
	//0, 1은 이미 할당?되어있어서?
	fd_max = serv_sock;

	while (1) {
		cpy_reads = reads;
		// select 하고 나면 받은 값만 1로 하고 나머지는 0으로 초기화되기 때문에
		// while루프 안에 cpy_reads=reads를 해줘야함
		// ★중요
		timeout.tv_sec = 5;
		timeout.tv_usec = 50000;

		if ((fd_num = select(fd_max + 1, &cpy_reads, 0, 0, &timeout)) == -1)
			break;
		//0부터했으니까 1을 더해주는 것임
		//&cpy_reads: 관찰대상


		if (fd_num == 0)
			continue;

		for (i = 0; i < fd_max + 1; i++) {
			// 3번째에 socket이 들어있음 따라서 for문 돌다가 
			if (FD_ISSET(i, &cpy_reads))
			{
				//i가 serv_sock이 맞으면 accept해서 실제 연결 요청을 수락해줌

				if (i == serv_sock)//connection request!
				{
					adr_sz = sizeof(clnt_adr);
					clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
					FD_SET(clnt_sock, &reads);
					//실제 reads에다 set해줌(copy가 아니라)
					if (fd_max < clnt_sock)
						fd_max = clnt_sock;
					// fd_max update
					printf("connected client: %d \n", clnt_sock);
				}
				else // read message!
				{
					str_len = read(i, buf, BUF_SIZE);
					if (str_len == 0)// close request!
					{
						FD_CLR(i, &reads);
						close(i);
						printf("closed client: %d \n", i);
					}
					else {
						//4번 clnt에 버퍼 써라
						write(i, buf, str_len); //echo!
					}
				}
			}
		}
	}
	close(serv_sock);
	return 0;
}

void error_handling(char *buf)
{
	fputs(buf, stderr);
	fputc('\n', stderr);
	exit(1);
}