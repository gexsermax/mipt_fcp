#ifndef _FCP_SERVER_LIB_H
#define _FCP_SERVER_LIB_H

/* for bool variables */
#include <stdbool.h>

struct program_flags {
  bool v; /* verbose */
};
typedef struct program_flags flags_t;

struct message_buffer {
  long mtype;
};
typedef struct message_buffer msgbuf_t;

#endif /* _FCP_SERVER_LIB_H */
