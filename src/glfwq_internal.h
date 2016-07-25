#include <GLFW/glfw3.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "glfwq.h"

typedef struct {
  uint8_t *data_ptr;
  // ring buffer: current read at readpos, current write at writepos
  // readpos, then writepos
  // write_end is the point where we looped around. (If we looped around.)
  size_t readpos, writepos, write_end, capacity;
} EventQueue;

extern __thread EventQueue queue;
