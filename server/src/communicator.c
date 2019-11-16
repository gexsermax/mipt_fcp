#include "../include/communicator.h"

/* for printf, NULL */
#include <stdio.h>

/* for exit */
#include <stdlib.h>

/* for errno, perror */
#include <errno.h>

/* for strerror */
#include <string.h>

/* for msgsnd, msgrcv */
#include <sys/msg.h>

/* for some helpful tips and tricks */
#include "../../include/lib.h"

void *communicate (void *args_void) {
  /* Parse arguments */
  commargs_t *args = (commargs_t *)args_void;
  flags_t *flags = args->flags;
  int msgid = args->msgid;
  int thread_id = args->thread_id;

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

  return NULL;
}
