#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 30

int main(int argc, char* argvp[])
{
	fd_set reads, temps;
	//temps에 복사한 애들 넣어줌
	int result, str_len;
	char buf[BUF_SIZE];
	struct timeval timeout;
	//timeout값 따로 설정

	FD_ZERO(&reads);
	//쓰레기값이 메모리에 있을 수도 있어서 FD_ZERO해줌
	FD_SET(0, &reads); // 0 is standard input(console)
	// 0은 키보드를 의미. 키보드로 입력 받겠다.
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

		// 복사된 temps를 select에 넣어준다.
		// select의 첫번째 인자 1 : 0번 하나 밖에 없어서
		// 데이터의 쓰기만 관찰
		// 예외상황은 고려 X
		// 5초 뒤에는 blocking 풀리게 함

		if (result == -1) {
			puts("select() error!");
			break;
		}
		else if (result == 0) {
			// timeout(5초)일때 select는 0을 반환
			puts("Time-out!");
		}
		else
		{
			if (FD_ISSET(0, &temps))
			{
				//temps안에 변화가 있었는지 확인해줌
				str_len = read(0, buf, BUF_SIZE);
				buf[str_len] = 0;
				printf("message from console: %s", buf);
			}
		}
	}
	return 0;
}