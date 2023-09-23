#include <assert.h>
#include "./utils.h"
#include "./chat.h"

struct chat_client chat_client_init(const char *name) {
  assert(name != NULL);

  return (struct chat_client) {
    .client = (struct client) {
      // .name = name,
      // .socket_fd = socket(AF_INET, SOCK_STREAM, 0),
    }
  }
}

bool chat_client_connect_to(struct room target) {

}