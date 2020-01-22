#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 30

int main(int argc, char* argvp[])
{
	fd_set reads, temps;
	//temps�� ������ �ֵ� �־���
	int result, str_len;
	char buf[BUF_SIZE];
	struct timeval timeout;
	//timeout�� ���� ����

	FD_ZERO(&reads);
	//�����Ⱚ�� �޸𸮿� ���� ���� �־ FD_ZERO����
	FD_SET(0, &reads); // 0 is standard input(console)
	// 0�� Ű���带 �ǹ�. Ű����� �Է� �ްڴ�.
	/*
	timeout.tv_sec = 5;
	timeout.tv_usec= 5000;
	*/

	while (1)
	{
		temps = reads;
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;
		result = select(1, &temps, 0, 0, &timeout);

		// ����� temps�� select�� �־��ش�.
		// select�� ù��° ���� 1 : 0�� �ϳ� �ۿ� ���
		// �������� ���⸸ ����
		// ���ܻ�Ȳ�� ��� X
		// 5�� �ڿ��� blocking Ǯ���� ��

		if (result == -1) {
			puts("select() error!");
			break;
		}
		else if (result == 0) {
			// timeout(5��)�϶� select�� 0�� ��ȯ
			puts("Time-out!");
		}
		else
		{
			if (FD_ISSET(0, &temps))
			{
				//temps�ȿ� ��ȭ�� �־����� Ȯ������
				str_len = read(0, buf, BUF_SIZE);
				buf[str_len] = 0;
				printf("message from console: %s", buf);
			}
		}
	}
	return 0;
}