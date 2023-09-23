#ifndef __UTILS__H__
#define __UTILS__H__
 
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SERVER_PORT 8080
#define BUFFER_SIZE 6
#define MAX_PENDING_CONNECTIONS 2
#define MAX_DATE_TIME_LENGTH 20

void close_socket(int fd);
void close_and_die(int fd);
bool handle_error(int condition, const char* msg);
void try_respond(int socket_fd, char* buffer, long max_buffer_size, char* msg);
void try_send(int socket_fd, char* buffer);

void panic(const char* msg);
void hexdump(const char* data);

void print_current_time();

#endif
