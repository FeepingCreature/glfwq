#include "glfwq_internal.h"

#define ASSIGN5(T1,N1,...) new_event->N1 = N1; ASSIGN4(__VA_ARGS__)
#define ASSIGN4(T1,N1,...) new_event->N1 = N1; ASSIGN3(__VA_ARGS__)
#define ASSIGN3(T1,N1,...) new_event->N1 = N1; ASSIGN2(__VA_ARGS__)
#define ASSIGN2(T1,N1,...) new_event->N1 = N1; ASSIGN1(__VA_ARGS__)
#define ASSIGN1(T1,N1) new_event->N1 = N1;
#define ARGS5(T1,N1,...) T1 N1, ARGS4(__VA_ARGS__)
#define ARGS4(T1,N1,...) T1 N1, ARGS3(__VA_ARGS__)
#define ARGS3(T1,N1,...) T1 N1, ARGS2(__VA_ARGS__)
#define ARGS2(T1,N1,...) T1 N1, ARGS1(__VA_ARGS__)
#define ARGS1(T1,N1) T1 N1
#define GET_MACRO(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,NAME,...) NAME
#define ARGS_EXPAND(...) GET_MACRO(__VA_ARGS__, ARGS5,, ARGS4,, ARGS3,, ARGS2,, ARGS1, ) (__VA_ARGS__)
#define ASSIGN_EXPAND(...) GET_MACRO(__VA_ARGS__, ASSIGN5,, ASSIGN4,, ASSIGN3,, ASSIGN2,, ASSIGN1, ) (__VA_ARGS__)

static size_t next_pot(size_t i) {
  size_t limit = SIZE_MAX >> 1; // there is no next power of two after 2^31 / 2^63
  if (i >= limit) { fprintf(stderr, "internal error: queue has exploded.\n"); abort(); }
  size_t v = 1;
  while (v <= i) v *= 2;
  return v;
}

static void *queue_alloc(size_t size) {
  size_t space_free;
  // [       R|----->|W      ]
  if (queue.writepos >= queue.readpos) {
    // write cursor is still ahead
    space_free = queue.capacity - queue.writepos;
    if (space_free < size && queue.readpos >= size) {
      // ran out of space before the end, but we can just loop around
      // [->|W    R|----------E| ]
      queue.write_end = queue.writepos;
      queue.writepos = 0;
    }
  }
  // [--->|W            R|---]
  if (queue.writepos < queue.readpos) {
    // write cursor has looped around
    space_free = queue.readpos - queue.writepos;
  }
  // write pos must not reach read pos (ambiguous!)
  if (size >= space_free) {
    // next pot to fit the data we need to store (+1 is implied in definition of *next*_pot)
    size_t newcap = next_pot(queue.capacity - space_free + size);
    uint8_t *new_ptr = malloc(newcap);
    if (new_ptr == NULL) { fprintf(stderr, "internal error: could not resize queue, out of memory.\n"); abort(); }
    size_t data_copied;
    if (queue.writepos >= queue.readpos) {
      size_t space_between_pos = queue.writepos - queue.readpos;
      if (space_between_pos > 0) {
        memcpy(new_ptr, queue.data_ptr + queue.readpos, space_between_pos);
      }
      data_copied = space_between_pos;
    } else { // looped around
      size_t space_before_end = queue.write_end - queue.readpos;
      // copy back half
      memcpy(new_ptr, queue.data_ptr + queue.readpos, space_before_end);
      // copy front half
      memcpy(new_ptr + space_before_end, queue.data_ptr, queue.writepos);
      data_copied = space_before_end + queue.writepos;
    }
    // clean up and reinit
    if (queue.data_ptr) free(queue.data_ptr);
    queue = (EventQueue) {
      .data_ptr = new_ptr,
      .readpos = 0,
      .writepos = data_copied,
      .write_end = 0, // not relevant until we wrap around again
      .capacity = newcap
    };
    space_free = queue.capacity - queue.writepos;
  }
  assert(space_free >= size);
  void *res = queue.data_ptr + queue.writepos;
  queue.writepos += size;
  return res;
}

#define DEF_CALLBACK(NAME, TYPE, ENUM, ...)\
static void NAME ## _callback(ARGS_EXPAND(__VA_ARGS__)) {\
  GLFWQ ## TYPE ## Event *new_event = queue_alloc(sizeof(GLFWQ ## TYPE ## Event));\
  new_event->base = (GLFWQEvent) { .type = GLFWQ_ ## ENUM, .size = sizeof(GLFWQ ## TYPE ## Event) };\
  ASSIGN_EXPAND(__VA_ARGS__)\
}
#define DEF_CALLBACK_W(NAME, TYPE, ENUM, ...) DEF_CALLBACK(NAME, TYPE, ENUM, __VA_ARGS__)

#include "defs.txt"

#undef DEF_CALLBACK
#undef DEF_CALLBACK_W

void glfwqSetupCallbacks(GLFWwindow *window) {
#define DEF_CALLBACK(NAME, TYPE, ENUM, ...) glfwSet ## TYPE ## Callback(NAME ## _callback);
#define DEF_CALLBACK_W(NAME, TYPE, ENUM, ...) glfwSet ## TYPE ## Callback(window, NAME ## _callback);
#include "defs.txt"
#undef DEF_CALLBACK
#undef DEF_CALLBACK_W
}
