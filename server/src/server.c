#include "../include/server.h"

/* for printf, NULL, fprintf */
#include <stdio.h>

/* for exit, EXIT_SUCCESS, EXIT_FAILURE */
#include <stdlib.h>

/* for errno, perror */
#include <errno.h>

/* for strerror */
#include <string.h>

/* for getopt */
#include <getopt.h>

/* for pthread_t, pthread_create, pthread_join */
#include <pthread.h>

/* for msgget, msgctl */
#include <sys/msg.h>

/* for some helpful tips and tricks */
#include "../../include/lib.h"

/* for control thread */
#include "../include/controller.h"

int main(int argc, char *argv[], char *envp[]) {
  flags_t flags = { /* program flags */
    false  /* verbose */
  };

  /* Parse arguments */ {
    int retval = 0; /* returned value */
    while ((retval = getopt(argc, argv, "v")) != -1) {
      switch (retval) {
        case 'v':
          flags.v = true;
          break;
        default: /* '?' */
          fprintf(stderr, "Usage: %s [-v]\n", argv[0]);
          exit(EXIT_FAILURE);
          break;
      }
    }
  }

  /* Create a message queue and a buffer*/
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

  if (flags.v) {
    printf("Hello, World! I'm the Server!\n");
  }

  if (msgctl(msgid, IPC_RMID, NULL)) {
    eprintf("msgctl");
  }
  if (pthread_join(controller, NULL)) {
    eprintf("pthread_join");
  }

  if (flags.v) {
    printf("Goodbye, World! Best Wishes, Server!\n");
  }

  exit(EXIT_SUCCESS);
}
