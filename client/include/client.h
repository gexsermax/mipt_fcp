#ifndef _FCP_CLIENT_CLIENT_H
#define _FCP_CLIENT_CLIENTR_H

/* for bool variables */
#include <stdbool.h>

struct program_flags {
  bool v; /* verbose */
};
typedef struct program_flags flags_t;

#define FILENAME_MAX_LENGTH 64

/* wrong usage error message */
#define wuprintf()                                                             \
  fprintf(stderr, "Usage: %s [-v] -f filename \n", argv[0]);                 \
  exit(EXIT_FAILURE);

#endif /* _FCP_CLIENT_CLIENT_H */
