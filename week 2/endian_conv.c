#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
	unsigned short host_port = 0x1234;
	unsigned short net_port;
	unsigned long host_addr = 0x123456a8;
	unsigned long net_addr;

	net_port = htons(host_port);
	net_addr = htnol(host_addr);

	//'%#x'�� ���� �𸣽ô� �е��� '����������'�� �˻��غ�����.
	//�ϴ� ���⼭�� ��½� 16���� �ҹ��� ������� ���ظ� �Ͻø� �˴ϴ�.

	printf("Host ordered port: %#x \n", host_port);
	printf("Network ordered prot: %#x \n", net_port);
	printf("Host ordered address: %#x \n", host_addr);
	printf("Network ordered address: %#x \n", net_addr);
	return 0;
}