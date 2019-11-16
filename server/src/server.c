#include "../include/server.h"

/* for printf, NULL, fprintf */
#include <stdio.h>

/* for exit, EXIT_SUCCESS, EXIT_FAILURE, strtol */
#include <stdlib.h>

/* for errno, perror */
#include <errno.h>

/* for strerror */
#include <string.h>

/* for getopt */
#include <getopt.h>

/* for pthread_t, pthread_create, pthread_join */
#include <pthread.h>

/* for msgget, msgctl, msgsnd, msgrcv */
#include <sys/msg.h>

/* for some helpful tips and tricks */
#include "../../include/lib.h"

/* for control thread */
#include "../include/controller.h"

/* for communication threads */
#include "../include/communicator.h"

#define NTHREAD_DEF 10 /* default number of communication threads */

int main(int argc, char *argv[], char *envp[]) {
  flags_t flags = { /* program flags */
    false  /* verbose */
  };
  int nthreads = NTHREAD_DEF;

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
            fprintf(stderr, "Usage: %s [-v] [-n nthreads] \n", argv[0]);
            exit(EXIT_FAILURE);
          }
          break;
        }
        default: /* '?' */
          fprintf(stderr, "Usage: %s [-v] [-n nthreads] \n", argv[0]);
          exit(EXIT_FAILURE);
      }
    }
  }

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
  commargs_t communicators_args[nthreads];
  for (int i = 0; i < nthreads; ++i) {
    communicators_args[i] = (commargs_t){&flags, msgid, i + 1};
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

  if (flags.v) {
    printf("Goodbye, World! Best Wishes, Server!\n");
  }

  exit(EXIT_SUCCESS);
}
