# glfwq
Instead of callbacks, poll GLFW for events! As opposed to sticky mode, this generates one event for each callback.
# install
  cmake . && make && sudo make install
# usage
    #include <glfwq.h>
    ...
    glfwqSetupCallbacks(window);
    ...
    glfwPollEvents(window);
    while (!glfwqQueueEmpty()) {
      GLFWQEvent *event = glfwqTakeEvent();
      if (event.type == GLFWQ_KEY) {
        GLFWQKeyEvent *key = (GLFWQKeyEvent*) event;
        ...
      }
    }
