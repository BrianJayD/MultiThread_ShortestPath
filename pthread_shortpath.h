#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int paths[10];
  int costs[10];
} node;
