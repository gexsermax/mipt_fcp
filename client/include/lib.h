#ifndef _FCP_CLIENT_LIB_H
#define _FCP_CLIENT_LIB_H

/* for bool variables */
#include <stdbool.h>

struct program_flags {
  bool v; /* verbose */
};
typedef struct program_flags flags_t;

#define PATHNAME_MAX_LENGTH 128

#endif /* _FCP_CLIENT_LIB_H */
