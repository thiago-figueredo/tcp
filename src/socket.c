#include "./socket.h"

int create_chat_socket(const uint16_t port) {
  int error;
  struct sockaddr client_addr;

  int fd = socket(AF_INET, SOCK_STREAM, 0);

  if (!handle_error(fd, "socket() failed")) {
    close_and_die(fd);
  };

  struct sockaddr_in addr = (struct sockaddr_in) {
    .sin_family = AF_INET,
    .sin_port = htons(port),
    .sin_addr = { .s_addr = INADDR_ANY },
  };

  error = bind(fd, (struct sockaddr*)&addr, sizeof(addr));

  if (!handle_error(error, "bind() failed")) {
    close_and_die(fd);
  }

  return fd;
}