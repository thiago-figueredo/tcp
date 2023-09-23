#ifndef __SOCKET__H__
#define __SOCKET__H__

#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>

int create_socket(const uint16_t port);

#endif