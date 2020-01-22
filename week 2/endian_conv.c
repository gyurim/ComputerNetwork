#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
	unsigned short host_port = 0x1234;
	unsigned short net_port;
	unsigned long host_addr = 0x123456a8;
	unsigned long net_addr;

	net_port = htons(host_port);
	net_addr = htnol(host_addr);

	//'%#x'가 뭔지 모르시는 분들은 '서식지정자'를 검색해보세요.
	//일단 여기서는 출력시 16진수 소문자 출력으로 이해를 하시면 됩니다.

	printf("Host ordered port: %#x \n", host_port);
	printf("Network ordered prot: %#x \n", net_port);
	printf("Host ordered address: %#x \n", host_addr);
	printf("Network ordered address: %#x \n", net_addr);
	return 0;
}