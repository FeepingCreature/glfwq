#ifndef GLFWQ_H
#define GLFWQ_H

#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdbool.h>

typedef enum {
  GLFWQ_ERROR,
  GLFWQ_MONITOR,
  GLFWQ_WINDOW_POS,
  GLFWQ_WINDOW_SIZE,
  GLFWQ_WINDOW_CLOSE,
  GLFWQ_WINDOW_REFRESH,
  GLFWQ_WINDOW_FOCUS,
  GLFWQ_WINDOW_ICONIFY,
  GLFWQ_FRAMEBUFFER_SIZE,
  GLFWQ_KEY,
  GLFWQ_CHAR,
  GLFWQ_CHAR_MODS,
  GLFWQ_MOUSE_BUTTON,
  GLFWQ_CURSOR_POS,
  GLFWQ_CURSOR_ENTER,
  GLFWQ_SCROLL,
  GLFWQ_DROP,
} GLFWQEventType;

typedef struct {
  GLFWQEventType type;
  size_t size;
} GLFWQEvent;

typedef struct {
  GLFWQEvent base;
  int error;
  const char *description;
} GLFWQErrorEvent;

typedef struct {
  GLFWQEvent base;
  GLFWmonitor *monitor;
  int event;
} GLFWQMonitorEvent;

typedef struct {
  GLFWQEvent base;
  GLFWwindow *window;
  int xpos, ypos;
} GLFWQWindowPosEvent;

typedef struct {
  GLFWQEvent base;
  GLFWwindow *window;
  int width, height;
} GLFWQWindowSizeEvent;

typedef struct {
  GLFWQEvent base;
  GLFWwindow *window;
} GLFWQWindowCloseEvent;

typedef struct {
  GLFWQEvent base;
  GLFWwindow *window;
} GLFWQWindowRefreshEvent;

typedef struct {
  GLFWQEvent base;
  GLFWwindow *window;
  int focused;
} GLFWQWindowFocusEvent;

typedef struct {
  GLFWQEvent base;
  GLFWwindow *window;
  int iconified;
} GLFWQWindowIconifyEvent;

typedef struct {
  GLFWQEvent base;
  GLFWwindow *window;
  int width, height;
} GLFWQFramebufferSizeEvent;

typedef struct {
  GLFWQEvent base;
  GLFWwindow *window;
  int button, action, mods;
} GLFWQMouseButtonEvent;

typedef struct {
  GLFWQEvent base;
  GLFWwindow *window;
  double xpos, ypos;
} GLFWQCursorPosEvent;

typedef struct {
  GLFWQEvent base;
  GLFWwindow *window;
  int entered;
} GLFWQCursorEnterEvent;

typedef struct {
  GLFWQEvent base;
  GLFWwindow *window;
  double xoffset, yoffset;
} GLFWQScrollEvent;

typedef struct {
  GLFWQEvent base;
  GLFWwindow *window;
  int key, scancode, action, mods;
} GLFWQKeyEvent;

typedef struct {
  GLFWQEvent base;
  GLFWwindow *window;
  unsigned int codepoint;
} GLFWQCharEvent;

typedef struct {
  GLFWQEvent base;
  GLFWwindow *window;
  unsigned int codepoint;
  int mods;
} GLFWQCharModsEvent;

typedef struct {
  GLFWQEvent base;
  GLFWwindow *window;
  int count;
  const char **paths;
} GLFWQDropEvent;

void glfwqSetupCallbacks(GLFWwindow *window);

bool glfwqQueueEmpty();

GLFWQEvent *glfwqPeekEvent();

GLFWQEvent *glfwqTakeEvent();

#endif
