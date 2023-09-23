#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "./utils.h"

void close_socket(int fd) {
  handle_error(shutdown(fd, SHUT_RDWR), "shutdown() failed");
}

void close_and_die(int fd) {
  close_socket(fd);
  exit(EXIT_FAILURE);
}

void panic(const char* msg) {
  fprintf(stderr, "%s: %s\n", msg, strerror(errno));
}

bool handle_error(int condition, const char* msg) {
  if (condition < 0) {
    panic(msg);
    return false;
  }

  return true;
}

void hexdump(const char* data) {
  assert(data != NULL);

  const size_t length = strlen(data);

  for (size_t i = 0; i < length; ++i) {
    printf("%02x ", data[i]);
  }

  printf("\n");
}

void try_respond(
  int socket_fd, 
  char* buffer, 
  long max_buffer_size,
  char* msg
) {
  if (buffer == NULL) {
    return;
  }

  long bytes_received;

  while ((bytes_received = recv(socket_fd, buffer, max_buffer_size, 0)) > 0) {
    if (bytes_received >= max_buffer_size) {
      printf("[] Buffer overflow\n");
      printf("[] Received %ld bytes, but max is %ld bytes\n", bytes_received, max_buffer_size);
      printf("[] Buffer: %s\n", buffer);
      break;
    }

    if (!strcmp(buffer, "quit")) {
      close_and_die(socket_fd);
    }

    printf("> %s\n", buffer);
    try_send(socket_fd, msg);
    memset(buffer, 0, BUFFER_SIZE);
  }

  if (!handle_error(bytes_received, "recv() failed")) {
    close_and_die(socket_fd);
  }
}

void try_send(int socket_fd, char* buffer) {
  if (buffer == NULL) {
    return;
  }

  long buffer_len = strlen(buffer);
  long bytes_sent = 0;
  long total_bytes_sent = 0;

  while (total_bytes_sent < buffer_len) {
    long bytes_sent = send(socket_fd, buffer + total_bytes_sent, buffer_len - total_bytes_sent, 0);

    if (bytes_sent < 0) {
      break;
    }

    printf("[] Sent %ld bytes\n", bytes_sent);
    printf("[] Sent %.*s\n", (int)bytes_sent, buffer + total_bytes_sent);

    total_bytes_sent += bytes_sent;
  }

  if (!handle_error(bytes_sent, "send() failed")) {
    close_and_die(socket_fd);
  }

}


void print_current_time() {
  time_t currentTime;
  time(&currentTime);

  struct tm *localTime = localtime(&currentTime);

  int year = localTime->tm_year + 1900;  // Years since 1900
  int month = localTime->tm_mon + 1;     // Months (0-11, so add 1)
  int day = localTime->tm_mday;          // Day of the month
  int hour = localTime->tm_hour;         // Hours
  int minute = localTime->tm_min;        // Minutes
  int second = localTime->tm_sec;        // Seconds

  printf("%04d-%02d-%02d %02d:%02d:%02d", year, month, day, hour, minute, second);
}
