
#ifndef BF_COMMON_H_INCLUDED
#define BF_COMMON_H_INCLUDED

#define VERSION_MAJOR 1
#define VERSION_PATCH 3
#define VERSION_TEST 3

typedef enum {
    bf_end,
    bf_add,
    bf_subtract,
    bf_right,
    bf_left,
    bf_output,
    bf_input,
    bf_open,
    bf_close,
    bf_debug
} bf_type;

typedef struct {
    bf_type type;
    int val; /* Will represent the magnitude of increments, or the pointer to jump to... */
} bf_command;

#endif
