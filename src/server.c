#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "./utils.h"

int main(void) {
  printf("Listening on port %d...\n", SERVER_PORT);

  int error;
  struct sockaddr client_addr;

  int server_socket = socket(AF_INET, SOCK_STREAM, 0);

  if (!handle_error(server_socket, "socket() failed")) {
    close_and_die(server_socket);
  };

  struct sockaddr_in server_addr = (struct sockaddr_in) {
    .sin_family = AF_INET,
    .sin_port = htons(SERVER_PORT),
    .sin_addr = { .s_addr = INADDR_ANY },
  };

  bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

  error = listen(server_socket, MAX_PENDING_CONNECTIONS);

  if (!handle_error(error, "listen() failed")) {
    close_and_die(server_socket);
  }

  while (true) {
    char buffer[BUFFER_SIZE] = {0};
    socklen_t client_addr_len = sizeof(client_addr);

    int client_socket = accept(server_socket, &client_addr, &client_addr_len);

    if (!handle_error(client_socket, "accept() failed")) {
      close_and_die(server_socket);
    }

    print_current_time();
    try_respond(client_socket, buffer, BUFFER_SIZE, buffer);
    close_socket(client_socket);
  }

  close_socket(server_socket);

  return 0;
}