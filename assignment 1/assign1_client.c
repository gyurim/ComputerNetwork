#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message);

int main(int argc, char *argv[])
{
	// 1. CREATES A TCP SOCKET
	int client_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (client_socket == -1)
		error_handling("socket() error");

	const int BUFFER_SIZE = 20;

	char buffer[BUFFER_SIZE];
	char snd_buffer[BUFFER_SIZE];

	struct sockaddr_in server_address;
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;

	// 2. CONNECTS TO TCP LOCAL SERVER (BY ADDRESS: ip=127.0.0.1, port=1234)
	server_address.sin_port = htons(1234);
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1)
		error_handling("connect() error!");


	// 3. RECEIVES (A BYTE) A LENGTH OF THE INCOMING DATA
	char expected_data_length;
	/* RECEIVE A BYTE AND ASSIGN IT TO THE VARIABLE ABOVE*/
	read(client_socket, &expected_data_length, 1);

	// 4. RECEIVES THE INCOMING DATA
	char received_length;
	/* RECEIVE THE DATA AND ASSIGN IT'S LENGTH TO THE VARIABLE ABOVE */
	received_length = (char)read(client_socket, buffer, BUFFER_SIZE);

	if (received_length != expected_data_length)
	{
		printf("Mismatch in data length! Expected data length: %d, received data length: %d\n", (int)expected_data_length, (int)received_length);
	}
	// 5. PRINTS OUT THE RECEIVED DATA
	else {
		write(0, buffer, (int)received_length);
	}

	// SEND A NUMBER (A BYTE) VALUE 0 TO SERVER
	snd_buffer[0] = '0';
	write(client_socket, snd_buffer, BUFFER_SIZE);

	// CLOSES THE SOCKET
	close(client_socket);
	return 0;
}

void error_handling(char *message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
