#ifndef _FCP_SERVER_SERVER_H
#define _FCP_SERVER_SERVER_H

/* wrong usage error message */
#define wuprintf()                                                             \
  fprintf(stderr, "Usage: %s [-v] [-n nthreads] \n", argv[0]);                 \
  exit(EXIT_FAILURE);

#define NTHREAD_DEF 10 /* default number of communication threads */

#endif /* _FCP_SERVER_SERVER_H */
