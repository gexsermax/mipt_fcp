#ifndef _FCP_CLIENT_CLIENT_H
#define _FCP_CLIENT_CLIENTR_H

#define FILENAME_MAX_LENGTH 64

/* wrong usage error message */
#define wuprintf()                                                             \
  fprintf(stderr, "Usage: %s [-v] (-f filename) (-l pathname) \n", argv[0]);                 \
  exit(EXIT_FAILURE);

#endif /* _FCP_CLIENT_CLIENT_H */
