#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
void error_handling(char *message);

/*
***********************************************************
*			기능 상, inet_addr 함수와 동일                *                                   
-----------------------------------------------------------
 점이 찍힌 10진수로 표현된 문자열을 전달하면, 해당 문자열 
 정보를 참조해서 IP 주소 정보를 32비트 정수형으로 반환
***********************************************************
*/

int main(int argc, char *argv[])
{
	char *addr = "127.232.124.79";
	struct sockaddr_in addr_inet;

	if (!inet_aton(addr, &addr_inet.sin_addr))
		error_handling("Conversion error");
	else
		printf("Network ordered integer addr: %#x \n",
			addr_inet.sin_addr.s_addr);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}