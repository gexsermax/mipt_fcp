#include "../include/communicator.h"

/* for printf, NULL */
#include <stdio.h>

/* for exit, malloc, free */
#include <stdlib.h>

/* for errno, perror */
#include <errno.h>

/* for strerror, strcmp */
#include <string.h>

/* for msgsnd, msgrcv */
#include <sys/msg.h>

/*for pthread_mutex_t, pthread_mutex_lock, pthread_mutex_unlock */
#include <pthread.h>

/* for epoll_wait, epoll_ctl */
#include <sys/epoll.h>

/* for read */
#include <unistd.h>

/* for some helpful tips and tricks */
#include "../../include/lib.h"

void *communicate (void *args_void) {
  /* Parse arguments */
  commargs_t *args = (commargs_t *)args_void;
  flags_t *flags = args->flags;
  int msgid = args->msgid;
  int thread_id = args->thread_id;
  epollpar_t *epoll = args->epoll;
  int control_fifo_fd_read = args->control_fifo_fd_read;
  pthread_mutex_t *mutex = args->sharval->mutex;
  bool *running = &args->sharval->running;

  /* Wait until get the message to start */
  msgbuf_t msgbuf;
  if (msgrcv(msgid, &msgbuf, 0, thread_id, 0)) {
    eprintf("msgrcv");
  }

  if (flags->v) {
    printf("Hello, World! I'm the %dth communicator!\n", thread_id);
  }

  /* Inform the next process that it can run
   * or inform the main process that all communicator processes have been started */
  ++msgbuf.mtype;
  if (msgsnd(msgid, &msgbuf, 0, IPC_NOWAIT)) {
    eprintf("msgsnd");
  }

  while(*running) {
    pthread_mutex_lock(mutex);

    /* Wait until someting happens with some descriptors */
    int cevents /* events counter */
      = epoll_wait(epoll->fd, epoll->events, NEPOLL_EVENTS_MAX, epoll->timeout);
    if (cevents == 0) {
      pthread_mutex_unlock(mutex);
      break;
    }

    /* Process incoming data */
    for (int i = 0; i < cevents; ++i) {
      /* Contol command */
      if (epoll->events[i].data.fd == control_fifo_fd_read) {
        char *command = (char *)malloc(COMMAND_MAX_LENGTH * sizeof(*command));
        if (command == NULL) {
          eprintf("malloc");
        }
        int read_size = read(control_fifo_fd_read, command, COMMAND_MAX_LENGTH);
        if(read_size < 0) {
          eprintf("read");
        }

        /* stop */
        if (!strcmp(command, commands[0])) {
          epoll_ctl(epoll->fd, EPOLL_CTL_DEL, control_fifo_fd_read, epoll->events);
          epoll->timeout = 0;
          *running = false;
        } else {
          errno = -1;
          eprintf("control");
        }

        free(command);

        pthread_mutex_unlock(mutex);
      } else {
        errno = -1;
        eprintf("epoll_wait");
      }
    }

    pthread_mutex_unlock(mutex);
  }

  if (flags->v) {
    printf("Goodbye, World! Best Wishes, the %dth communicator!\n", thread_id);
  }

  return NULL;
}
