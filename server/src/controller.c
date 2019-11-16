#include "../include/controller.h"

/* for printf, NULL, fgets */
#include <stdio.h>

/* for malloc, free */
#include <stdlib.h>

/* for errno, perror */
#include <errno.h>

/* for strcmp, strerror */
#include <string.h>

/* for msgsnd */
#include <sys/msg.h>

/* for some helpful tips and tricks */
#include "../../include/lib.h"

/* Commands for server control */
#define COMMAND_MAX_LENGTH 16 /* Maximum length of available command */
#define COMMANDS_NUM 2 /* Number of available commands */
/* Using '\n' because 'fgets' stores it into the buffer */
static const char * const commands[] = {
  "stop\n",
  "help\n"
};

void *control (void *args_void) {
  /* Parse arguments */
  conargs_t *args = (conargs_t *)args_void;
  flags_t *flags = args->flags;
  int msgid = args->msgid;

  if (flags->v) {
    printf("Hello, World! I'm the Server Contoller!\n");
  }

  /* Inform the main procces that the contol process has been started */
  msgbuf_t msgbuf = {1};
  if (msgsnd(msgid, &msgbuf, 0, IPC_NOWAIT)) {
    eprintf("msgsnd");
  }

  /* Scan user commands */ {
    bool scanning = true;
    char *command = (char *)malloc(COMMAND_MAX_LENGTH * sizeof(*command));
    if (command == NULL) {
      eprintf("malloc");
    }
    while (scanning) {
      fgets(command, COMMAND_MAX_LENGTH, stdin);

      /* stop */
      if (!strcmp(command, commands[0])) {
        scanning = false;
      }
      /* help */
      else if (!strcmp(command, commands[1])) {
        printf("Available commands:\n");
        for (int i = 0; i < COMMANDS_NUM; ++i) {
          printf("\t%s", commands[i]);
        }
      }
      /* unknown command */
      else {
        printf("Unknown command. Type 'help' to get a list of the available commands.\n");
      }
    }
    free(command);
  }

  if (flags->v) {
    printf("Goodbye, World! Best Wishes, Controller!\n");
  }

  return NULL;
}
