#include "../include/client.h"

/* for printf */
#include <stdio.h>

/* for exit, EXIT_SUCCESS, EXIT_FAILURE */
#include <stdlib.h>

/* for getopt */
#include <getopt.h>

/* for strcpy */
#include <string.h>

int main(int argc, char *argv[], char *envp[]) {
  flags_t flags = {
    false  /* verbose */
  }; /* program flags */
  char filename[FILENAME_MAX_LENGTH] = {};

  /* Parse options */ {
    bool topt = false; /* flag for t option */
    int retval = 0; /* returned value */
    while ((retval = getopt(argc, argv, "vf:")) != -1) {
      switch (retval) {
        case 'v':
          flags.v = true;
          break;
        case 'f':
          strcpy(filename, optarg);
          topt = true;
          break;
        default: /* '?' */
          wuprintf();
      }
    }
    if (!topt) {
      wuprintf();
    }
  }

  if (flags.v) {
    printf("Hello, World! I'm the Client!\n");
  }

  if (flags.v) {
    printf("Goodbye, World! Best wishes, the Client!\n");
  }

  exit(EXIT_SUCCESS);
}
