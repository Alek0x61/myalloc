#include <stdio.h>

typedef enum {
    FASTBIN      = 0,
    SMALLBIN     = 1,
    LARGEBIN     = 2,
    UNSORTEDBIN  = 3
} Bin_type;