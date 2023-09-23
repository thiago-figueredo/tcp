#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include "./wutils.h"

void close_socket(SOCKET fd) {
  handle_error(shutdown(fd, SD_BOTH), "shutdown() failed");
  closesocket(fd);
}

void close_and_die(SOCKET fd) {
  close_socket(fd);
  WSACleanup();
  exit(EXIT_FAILURE);
}

void panic(const char* msg) {
  fprintf(stderr, "%s: %d\n", msg, WSAGetLastError());
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
  SOCKET socket_fd,
  char* buffer,
  int max_buffer_size,
  char* msg
) {
  if (buffer == NULL) {
    return;
  }

  int bytes_received;

  while ((bytes_received = recv(socket_fd, buffer, max_buffer_size, 0)) > 0) {
    if (bytes_received >= max_buffer_size) {
      printf("[] Buffer overflow\n");
      printf("[] Received %d bytes, but max is %d bytes\n", bytes_received, max_buffer_size);
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

void try_send(SOCKET socket_fd, char* buffer) {
  if (buffer == NULL) {
    return;
  }

  int buffer_len = strlen(buffer);
  int bytes_sent = 0;
  int total_bytes_sent = 0;

  while (total_bytes_sent < buffer_len) {
    int bytes_sent = send(socket_fd, buffer + total_bytes_sent, buffer_len - total_bytes_sent, 0);

    if (bytes_sent < 0) {
        break;
    }

    printf("[] Sent %d bytes\n", bytes_sent);
    printf("[] Sent %.*s\n", bytes_sent, buffer + total_bytes_sent);

    total_bytes_sent += bytes_sent;
  }

  if (!handle_error(bytes_sent, "send() failed")) {
    close_and_die(socket_fd);
  }
}

void print_current_time() {
  time_t currentTime;
  time(&currentTime);

  struct tm localTime;
  localtime_s(&localTime, &currentTime);

  int year = localTime.tm_year + 1900;  // Years since 1900
  int month = localTime.tm_mon + 1;     // Months (0-11, so add 1)
  int day = localTime.tm_mday;          // Day of the month
  int hour = localTime.tm_hour;         // Hours
  int minute = localTime.tm_min;        // Minutes
  int second = localTime.tm_sec;        // Seconds

  printf("%04d-%02d-%02d %02d:%02d:%02d", year, month, day, hour, minute, second);
}