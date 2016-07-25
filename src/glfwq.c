#include "glfwq_internal.h"

__thread EventQueue queue = {
  .data_ptr = NULL,
  .readpos = 0,
  .writepos = 0,
  .write_end = 0xDEADBEEF,
  .capacity = 0
};

bool glfwqQueueEmpty() {
  return queue.readpos == queue.writepos;
}

GLFWQEvent *glfwqPeekEvent() {
  if (queue.readpos == queue.writepos) return NULL;
  // [--->|W           R|-E| ]
  if (queue.writepos < queue.readpos) { // wrapped around
    assert(queue.readpos <= queue.write_end);
    if (queue.readpos == queue.write_end) {
      // wrap read cursor as well
      return (GLFWQEvent*) queue.data_ptr;
    }
  }
  // otherwise, return current readpos as normal
  return (GLFWQEvent*) (queue.data_ptr + queue.readpos);
}

GLFWQEvent *glfwqTakeEvent() {
  if (queue.readpos == queue.writepos) return NULL;
  size_t size_limit;
  // [--->|W           R|-|E ]
  if (queue.writepos < queue.readpos) {
    assert(queue.readpos <= queue.write_end);
    size_limit = queue.write_end - queue.readpos;
    if (queue.readpos == queue.write_end) {
      queue.readpos = 0; // wrap!
      queue.write_end = 0xDEADBEEF; // and reset.
      // now, proceed with
      // [       R|----->|W      ]
    }
  }
  if (queue.readpos <= queue.writepos) {
    size_limit = queue.writepos - queue.readpos;
  }
  GLFWQEvent *event = (GLFWQEvent*)(queue.data_ptr + queue.readpos);
  size_t size = event->size;
  assert(size <= size_limit);
  queue.readpos += size;
  return event;
}
