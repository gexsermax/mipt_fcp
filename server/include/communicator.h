#ifndef _FCP_SERVER_COMMUNICATOR_H
#define _FCP_SERVER_COMMUNICATOR_H

#include "../include/lib.h"

struct communication_arguments {
  flags_t *flags;
  int msgid;
  int thread_id;
};
typedef struct communication_arguments commargs_t;

void *communicate (void *args_void);

#endif /* _FCP_SERVER_COMMUNICATOR_H */
