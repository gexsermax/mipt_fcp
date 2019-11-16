#include "../include/copyist.h"

/* for sprintf */
#include <stdio.h>

/* for exit, malloc, free */
#include <stdlib.h>

/* for strcpy, strerror, strlen */
#include <string.h>

/* for errno */
#include <errno.h>

/* for getpid, write */
#include <unistd.h>

/* for mkfifo */
#include <sys/types.h>

/* for mkfifo, open */
#include <sys/stat.h>

/* for mkfifo modes, open */
#include <fcntl.h>

/* for some helpful tips and tricks, defines */
#include "../../include/lib.h"

/* for defines, flags_t */
#include "../include/lib.h"

void copy(flags_t *flags, char *filename) {
  /* Create a FIFO for getting information from a server and open it for reading */
  pid_t pid = getpid();
  char *client_fifo_pathname = (char *)malloc(PATHNAME_MAX_LENGTH * sizeof(*client_fifo_pathname));
  sprintf(client_fifo_pathname, CLIENT_FIFO_DIR "%d.fifo", pid);
  if (unlink(client_fifo_pathname) && errno != ENOENT) {
    eprintf("unlink");
  }
  if (mkfifo(client_fifo_pathname, 0666)) {
    eprintf("mkfifo");
  }
  int client_fifo_fd_read = open(client_fifo_pathname, O_RDONLY | O_NONBLOCK);
  if (client_fifo_pathname < 0) {
    eprintf("open");
  }

  /* create a message to server */
  char *message = (char *)malloc(FIFO_ATOMIC_BLOCK_SIZE * sizeof(*message));
  sprintf(message, "%d %s", pid, filename);
  memset(message + strlen(message), '\0', FIFO_ATOMIC_BLOCK_SIZE - strlen(message));

  /* open the server communication fifo and write there the message */
  int server_communication_fifo_fd_write = open(SERVER_COMMUNICATION_FIFO_PATHNAME, O_WRONLY | O_NONBLOCK);
  if (server_communication_fifo_fd_write < 0) {
    eprintf("open");
  }
  int write_size = write(server_communication_fifo_fd_write, message, FIFO_ATOMIC_BLOCK_SIZE);
  if (write_size != FIFO_ATOMIC_BLOCK_SIZE) {
    eprintf("write");
  }

  if (close(server_communication_fifo_fd_write)) {
    eprintf("close");
  }
  free(message);
  if (close(client_fifo_fd_read)) {
    eprintf("close");
  }
  if (unlink(client_fifo_pathname)) {
    eprintf("unlink");
  }
  free(client_fifo_pathname);
}
