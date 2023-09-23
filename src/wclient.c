#include <stdio.h>
#include <stdbool.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "./utils.h"

#define BUFFER_SIZE 1024
#define SERVER_PORT 8080
#define SERVER_IP "192.168.0.107"

void try_receive(SOCKET socket_fd, char* buffer, int max_buffer_size) {
  int bytes_received;

  while ((bytes_received = recv(socket_fd, buffer, max_buffer_size, 0)) > 0) {
    if (bytes_received >= max_buffer_size) {
      printf("[] Buffer overflow\n");
      printf("[] Received %d bytes, but max is %d bytes\n", bytes_received, max_buffer_size);
      printf("[] Buffer: %s\n", buffer);
      break;
    }

    printf("> %s\n", buffer);
    break;
  }

  if (bytes_received == SOCKET_ERROR) {
    perror("recv() failed");
    closesocket(socket_fd);
    WSACleanup();
    exit(EXIT_FAILURE);
  }
}

int main(void) {
  WSADATA wsaData;
  int error;
  char buffer[BUFFER_SIZE] = { 0 };

  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    perror("WSAStartup() failed");
    return 1;
  }

  SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);

  if (client_socket == INVALID_SOCKET) {
    perror("socket() failed");
    WSACleanup();
    return 1;
  }

  struct sockaddr_in server_addr = {
    .sin_family = AF_INET,
    .sin_port = htons(SERVER_PORT),
  };

  error = inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

  if (error <= 0) {
    perror("inet_pton() failed");
    closesocket(client_socket);
    WSACleanup();
    return 1;
  }

  error = connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

  if (error == SOCKET_ERROR) {
    perror("connect() failed");
    closesocket(client_socket);
    WSACleanup();
    return 1;
  }

  printf("Connected to %s:%d\n\n", SERVER_IP, SERVER_PORT);

  while (true) {
    print_current_time();
    printf("> ");
    fgets(buffer, sizeof(buffer), stdin);

    size_t len = strlen(buffer);

    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }

    error = send(client_socket, buffer, strlen(buffer), 0);

    if (error == SOCKET_ERROR) {
        perror("send() failed");
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    if (!strcmp(buffer, "quit")) {
        break;
    }

    print_current_time();
    try_receive(client_socket, buffer, BUFFER_SIZE);
  }

  closesocket(client_socket);
  WSACleanup();

  return 0;
}
