#ifndef __UTILS__H__
#define __UTILS__H__

#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <winsock2.h>

#define SERVER_PORT 8080
#define BUFFER_SIZE 6
#define MAX_PENDING_CONNECTIONS 2
#define MAX_DATE_TIME_LENGTH 20

void close_socket(SOCKET fd);
void close_and_die(SOCKET fd)
bool handle_error(int condition, const char* msg);
void try_respond(SOCKET socket_fd, char* buffer, int max_buffer_size, char* msg);
void try_send(SOCKET socket_fd, char* buffer);

void panic(const char* msg);
void hexdump(const char* data);

void print_current_time();

#endif
