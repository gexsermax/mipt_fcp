#include "../include/client.h"

/* for printf */
#include <stdio.h>

/* for exit, EXIT_SUCCESS, EXIT_FAILURE */
#include <stdlib.h>

#include <getopt.h>

int main(int argc, char *argv[], char *envp[]) {
  flags_t flags = {
    false  /* verbose */
  }; /* program flags */

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

  if (flags.v) {
    printf("Hello, World! I'm the Client!\n");
  }

  exit(EXIT_SUCCESS);
}
