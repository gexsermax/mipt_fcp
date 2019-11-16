#ifndef _FCP_SERVER_LIB_H
#define _FCP_SERVER_LIB_H

/* for bool variables */
#include <stdbool.h>

struct program_flags {
  bool v; /* verbose */
};
typedef struct program_flags flags_t;

#define CONTROL_FIFO_PATHNAME "server/tmp/controller.fifo"

struct message_buffer {
  long mtype;
};
typedef struct message_buffer msgbuf_t;

/* Commands for server control */
#define COMMAND_MAX_LENGTH 16 /* Maximum length of available command */
#define NCOMMANDS 2 /* Number of available commands */
/* Using '\n' because 'fgets' stores it into the buffer */
static const char * const commands[] = {
  "stop\n",
  "help\n"
};

#define NEPOLL_EVENTS_MAX 2 /* max number of epoll events per time */

/* the structure with epoll parameters for threads */
struct epoll_parameters {
  int fd;
  struct epoll_event *events;
  int timeout;
};
typedef struct epoll_parameters epollpar_t;

#endif /* _FCP_SERVER_LIB_H */
