#ifndef __CHAT__H__
#define __CHAT__H__

#include <stddef.h>

struct client {
  int socket;
  const char *name;
};

struct room {
  const char *name;
  struct client* clients;
  size_t max_clients;
};

struct chat_client {
  struct client client;
  struct room {
    const char *name;
    struct chat_client* guests;
    size_t max_guests;
  };
};

struct chat_client chat_client_init(const char *name);
bool chat_client_connect_to(struct room target);

#endif