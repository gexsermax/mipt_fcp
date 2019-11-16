#ifndef _FCP_SERVER_CONTROLLER_H
#define _FCP_SERVER_CONTROLLER_H

#include "../include/lib.h"

struct control_arguments {
  flags_t *flags;
  int msgid;
};
typedef struct control_arguments conargs_t;

void *control (void *args_void);

#endif /* _FCP_SERVER_CONTROLLER_H */
