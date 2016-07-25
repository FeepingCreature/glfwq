cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(glfwq)

find_package(PkgConfig)

pkg_check_modules(GLFW3 REQUIRED glfw3)

include_directories("${CMAKE_SOURCE_DIR}/include")

add_library(glfwq SHARED src/glfwq.c src/callbacks.c)

target_compile_options(glfwq PRIVATE "-g" "-O2" "-Wall" "-Werror" "-pedantic" "-std=c99")

target_link_libraries(glfwq ${GLFW3_LIBRARIES})
target_include_directories(glfwq PUBLIC ${GLFW3_INCLUDE_DIRS})
target_compile_options(glfwq PUBLIC ${GLFW3_CFLAGS_OTHER})

set_target_properties(glfwq PROPERTIES SOVERSION "0.1")

install(TARGETS glfwq DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)
