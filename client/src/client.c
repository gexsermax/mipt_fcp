#include "../include/client.h"

/* for printf */
#include <stdio.h>

/* for exit, EXIT_FAILURE */
#include <stdlib.h>

/* for getopt */
#include <getopt.h>

/* for strerror */
#include <string.h>

/* for errno */
#include <errno.h>

/* for bool variables */
#include <stdbool.h>

/* for some helpful tips and tricks, defines */
#include "../../include/lib.h"

/* for defines, flags_t */
#include "../include/lib.h"

/* for ls */
#include "../include/ls.h"

/* for copy */
#include "../include/copyist.h"

int main(int argc, char *argv[], char *envp[]) {
  flags_t flags = {
    false  /* verbose */
  }; /* program flags */
  bool fflag = false; /* flag for f option */
  bool lflag = false; /* flag for l option */
  char filename[FILENAME_MAX_LENGTH] = {};
  char pathname[PATHNAME_MAX_LENGTH] = {};

  /* Parse options */ {
    int retval = 0; /* returned value */
    while ((retval = getopt(argc, argv, "vf:l:")) != -1) {
      switch (retval) {
        case 'v':
          flags.v = true;
          break;
        case 'f':
          strcpy(filename, optarg);
          fflag = true;
          break;
        case 'l':
          strcpy(pathname, optarg);
          lflag = true;
          break;
        default: /* '?' */
          wuprintf();
      }
    }
    if (!fflag && !lflag) {
      wuprintf();
    }
  }

  if (flags.v) {
    printf("Hello, World! I'm the Client!\n");
  }

  if (lflag) {
    ls(&flags, pathname);
  }
  if (fflag) {
    copy(&flags, filename);
  }

  if (flags.v) {
    printf("Goodbye, World! Best wishes, the Client!\n");
  }

  exit(EXIT_SUCCESS);
}
