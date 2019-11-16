#include "../include/controller.h"

/* for printf, NULL, fgets */
#include <stdio.h>

/* for malloc, free, exit */
#include <stdlib.h>

/* for errno, perror */
#include <errno.h>

/* for strcmp, strerror */
#include <string.h>

/* for msgsnd */
#include <sys/msg.h>

/* for open */
#include <sys/stat.h>

/* for open */
#include <fcntl.h>

/* for write */
#include <unistd.h>

/* for some helpful tips and tricks */
#include "../../include/lib.h"

void *control (void *args_void) {
  /* Parse arguments */
  conargs_t *args = (conargs_t *)args_void;
  flags_t *flags = args->flags;
  int msgid = args->msgid;
  int control_fifo_fd_write = open(CONTROL_FIFO_PATH, O_WRONLY);
  if (control_fifo_fd_write < 0) {
    eprintf("open");
  }

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
        int write_size = write(control_fifo_fd_write, commands[0], COMMAND_MAX_LENGTH);
        if (write_size < 0) {
          eprintf("write");
        }
        scanning = false;
      }
      /* help */
      else if (!strcmp(command, commands[1])) {
        printf("Available commands:\n");
        for (int i = 0; i < NCOMMANDS; ++i) {
          printf("\t%s", commands[i]);
        }
      }
      /* unknown command */
      else {
        printf(ANSI_COLOR_YELLOW "Unknown command" ANSI_COLOR_RESET ": %s"
               "Type 'help' to get a list of the available commands.\n", command);
      }
    }
    free(command);
  }

  if (flags->v) {
    printf("Goodbye, World! Best Wishes, the Controller!\n");
  }

  return NULL;
}
