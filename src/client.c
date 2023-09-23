#include <stdio.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "./utils.h"

#define BUFFER_SIZE 1024
#define SERVER_PORT 8080
#define SERVER_IP "127.0.0.1"

void try_receive(int socket_fd, char* buffer, long max_buffer_size) {
  long bytes_received;

  while ((bytes_received = recv(socket_fd, buffer, max_buffer_size, 0)) > 0) {
    if (bytes_received >= max_buffer_size) {
      printf("[] Buffer overflow\n");
      printf("[] Received %ld bytes, but max is %ld bytes\n", bytes_received, max_buffer_size);
      printf("[] Buffer: %s\n", buffer);
      break;
    }

    printf("> %s\n", buffer);
    break;
  }

  if (!handle_error(bytes_received, "recv() failed")) {
    close_and_die(socket_fd);
  }
}

int main(void) {
  int error;
  char buffer[BUFFER_SIZE] = {0};

  int client_socket = socket(AF_INET, SOCK_STREAM, 0);

  if (!handle_error(client_socket, "socket() failed")) {
    close_and_die(client_socket);
  }

  struct sockaddr_in server_addr = {
    .sin_family = AF_INET,
    .sin_port = htons(SERVER_PORT),
  };

  error = inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

  if (error <= 0) {
    close_and_die(client_socket);
  }

  if (!handle_error(error, "inet_pton() failed")) {
    close_and_die(client_socket);
  };  

  error = connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

  if (!handle_error(error, "inet_pton() failed")) {
    close_and_die(client_socket);
  };

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

    if (!handle_error(error, "send() failed")) {
      close_and_die(client_socket);
    }

    if (!strcmp(buffer, "quit")) {
      break;
    }

    print_current_time();
    try_receive(client_socket, buffer, BUFFER_SIZE);
  }

  close_socket(client_socket);

  return 0;
}