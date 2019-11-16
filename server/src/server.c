#include "../include/server.h"

/* for printf, NULL, fprintf */
#include <stdio.h>

/* for exit, EXIT_SUCCESS, EXIT_FAILURE, strtol */
#include <stdlib.h>

/* for bool variables */
#include <stdbool.h>

/* for errno, perror */
#include <errno.h>

/* for strerror, memset */
#include <string.h>

/* for getopt */
#include <getopt.h>

/* for pthread_t, pthread_create, pthread_join, pthread_mutex_t */
#include <pthread.h>

/* for msgget, msgctl, msgsnd, msgrcv */
#include <sys/msg.h>

/* for unlink */
#include <unistd.h>

/* for mkfifo */
#include <sys/types.h>

/* for mkfifo, open */
#include <sys/stat.h>

/* for open */
#include <fcntl.h>

/* for epoll_create1, struct epoll_event, epoll_ctl */
#include <sys/epoll.h>

/* for some helpful tips and tricks */
#include "../../include/lib.h"

/* for control thread */
#include "../include/controller.h"

/* for communication threads */
#include "../include/communicator.h"

int main(int argc, char *argv[], char *envp[]) {
  flags_t flags = { /* program flags */
    false  /* verbose */
  };
  int nthreads = NTHREAD_DEF; /* number of threads */

  /* Parse options */ {
    int retval = 0; /* returned value */
    while ((retval = getopt(argc, argv, "vn:")) != -1) {
      switch (retval) {
        case 'v':
          flags.v = true;
          break;
        case 'n': {
          char *end;
          nthreads = (int)strtol(optarg, &end, 10);
          if (*end != '\0') {
            wuprintf();
          }
          break;
        }
        default: /* '?' */
          wuprintf();
      }
    }
  }

  /* Create a control FIFO and open it for reading */
  if (unlink(CONTROL_FIFO_PATH) && errno != ENOENT) {
    eprintf("unlink");
  }
  if (mkfifo(CONTROL_FIFO_PATH, 0666)) {
    eprintf("mkfifo");
  }
  int control_fifo_fd_read = open(CONTROL_FIFO_PATH, O_RDONLY | O_NONBLOCK);
  if (control_fifo_fd_read < 0) {
    eprintf("open");
  }

  /* Create an epoll;
   * create and initialise an epoll control event;
   * add the control fifo descriptor into the epoll;
   * create an array for epoll events;
   * create a structure with epoll parameters for threads */
  int epoll_fd = epoll_create1(0);
  if (epoll_fd < 0) {
    eprintf("epoll_create1");
  }
  struct epoll_event control_epoll_event;
  memset(&control_epoll_event, 0, sizeof(struct epoll_event)); /* initialise the structure by zeroes */
  control_epoll_event.events = EPOLLIN;
  control_epoll_event.data.fd = control_fifo_fd_read;
  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, control_fifo_fd_read, &control_epoll_event)) {
    eprintf("epoll_ctl");
  }
  struct epoll_event epoll_events[NEPOLL_EVENTS_MAX];
  epollpar_t epoll = {epoll_fd, epoll_events, -1}; /* fd, events, timeout */

  /* Create a message queue and a buffer */
  int msgid = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
  if (msgid < 0) {
    eprintf("msgget");
  }
  msgbuf_t msgbuf = {0};

  /* Run the contol process */
  pthread_t controller;
  conargs_t control_args = {&flags, msgid};
  if (pthread_create(&controller, NULL, control, &control_args)) {
    eprintf("pthread_create");
  }

  /* Wait until the control process starts */
  if (msgrcv(msgid, &msgbuf, 0, 1, 0)) {
    eprintf("msgrcv");
  }

  /* Inform the first communication process that it can run */
  if (msgsnd(msgid, &msgbuf, 0, IPC_NOWAIT)) {
    eprintf("msgsnd");
  }

  /* Run communication processes */
  pthread_t communicators[nthreads];
  pthread_mutex_t mutex;
  pthread_mutex_init(&mutex, NULL);
  commargs_t communicators_args[nthreads];
  sharval_t sharval = {true, &mutex}; /* running, mutex */
  for (int i = 0; i < nthreads; ++i) {
    communicators_args[i] = (commargs_t){&flags, msgid, i + 1, &epoll, control_fifo_fd_read, &sharval};
    if (pthread_create(&communicators[i], NULL, communicate, &communicators_args[i])) {
      eprintf("pthread_create");
    }
  }

  /* Wait until all communication processes start */
  if (msgrcv(msgid, &msgbuf, 0, nthreads + 1, 0)) {
    eprintf("msgrcv");
  }

  if (flags.v) {
    printf("Hello, World! I'm the Server!\n");
  }

  if (pthread_join(controller, NULL)) {
    eprintf("pthread_join");
  }
  for (int i = 0; i < nthreads; ++i) {
    if (pthread_join(communicators[i], NULL)) {
      eprintf("pthread_join");
    }
  }
  if (msgctl(msgid, IPC_RMID, NULL)) {
    eprintf("msgctl");
  }
  if (close(epoll_fd)) {
    eprintf("close");
  }
  if (close(control_fifo_fd_read)) {
    eprintf("close");
  }
  if (unlink(CONTROL_FIFO_PATH)) {
    eprintf("unlink");
  }

  if (flags.v) {
    printf("Goodbye, World! Best Wishes, the Server!\n");
  }

  exit(EXIT_SUCCESS);
}
