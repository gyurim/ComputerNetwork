#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message);

int main()
{
	// 1. CREATES A TCP SERVER SOCKET
	int server_socket = socket(PF_INET, SOCK_STREAM, 0);

	if (server_socket == -1)
		error_handling("socket() error");

	FILE* file = fopen("data.txt", "rb");
	// The file descriptor is returned in FILE* file

	const int BUFFER_SIZE = 20;
	char buffer[BUFFER_SIZE];
	char rcv_buffer[BUFFER_SIZE];
	int read_cnt; // file length that buffer read;

	int str_len;

	struct sockaddr_in server_address;
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	// 2. BINDS THE SERVER SOCKET TO PORT 1234
	server_address.sin_port = htons(1234);
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1)
		error_handling("bind() error");

	// 2. LISTENS TO CONNECTION REQUESTS
	if (listen(server_socket, 2) == -1)
		error_handling("listen() error");

	// 3. ITERATIVE APPROACH
	int n;
	for (n = 0; n < 2; n++)
	{
		struct sockaddr_in clnt_adr;
		socklen_t clnt_adr_sz = sizeof(clnt_adr);

		// 3. ACCEPTS A CONNECTION REQUEST FROM CLIENT
		int client_socket = accept(server_socket, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		if (client_socket == -1)
			error_handling("accept() error");

		// 4.1. LOADS THE FILE CONTENT TO A BUFFER, AND NOTES THE LENGTH OF THE FILE
		// The length of the file is stored in read_cnt
		read_cnt = fread(buffer, 1, BUFFER_SIZE, file);
		char r_cnt = (char)(read_cnt);
		rewind(file);

		// 4.2. SENDS THE LENGTH OF THE FILE (DATA) TO CLIENT
		write(client_socket, &r_cnt, sizeof(r_cnt));

		// 4.3. SENDS THE CONTENT OF THE FILE	
		write(client_socket, buffer, read_cnt);

		// 4.4. TERMINATES ITS OUTPUT (TCP HALF-CLOSE)
		shutdown(client_socket, SHUT_WR);

		// 4.5. RECEIVES A NUMBER (A BYTE) FROM CLIENT
		read(client_socket, rcv_buffer, BUFFER_SIZE);

		// 4.5. PRINTS OUT THE RECEIVED NUMBER
		printf("Received: %s \n", rcv_buffer);

		// 4.6. CLOSES THE CLIENT SOCKET	
		close(client_socket);
	}

	// 5. CLOSES THE FILE AND SERVER SOCKET
	fclose(file);
	close(server_socket);

	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}