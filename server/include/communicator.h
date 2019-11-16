#ifndef _FCP_SERVER_COMMUNICATOR_H
#define _FCP_SERVER_COMMUNICATOR_H

#include "../include/lib.h"

/* for bool variables */
#include <stdbool.h>

/* for pthread_mutex_t */
#include <pthread.h>

struct shared_variables {
  bool running;
  pthread_mutex_t *mutex;
};
typedef struct shared_variables sharval_t;

struct communication_arguments {
  flags_t *flags;
  int msgid;
  int thread_id;
  epollpar_t *epoll;
  int control_fifo_fd_read;
  int communication_fd_read;
  sharval_t *sharval;
};
typedef struct communication_arguments commargs_t;

void *communicate (void *args_void);

#endif /* _FCP_SERVER_COMMUNICATOR_H */
