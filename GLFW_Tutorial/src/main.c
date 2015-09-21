#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>

#define BUF_SIZE 512

GLFWwindow* worker_win = NULL;
GLFWwindow* renderer_win = NULL;
GLuint vbo = 0;

GLuint egl_arraybuffer_get_binding ()
{
  GLuint x;
  glGetIntegerv (GL_ARRAY_BUFFER_BINDING, &x);
  return x;
}

GLuint egl_arraybuffer_get_size ()
{
  GLuint x;
  glGetBufferParameteriv (GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &x);
  return x;
}

GLuint egl_buffer_generate ()
{
  GLuint x;
  glGenBuffers (1, &x);
  assert (x > 0);
  return x;
}

GLuint egl_arraybuffer_allocate (GLsizeiptr size, GLenum usage)
{
  assert (egl_arraybuffer_get_binding() > 0);
  glBufferData (GL_ARRAY_BUFFER, size, NULL, usage);
  assert (egl_arraybuffer_get_size() == size);
}

void egl_arraybuffer_bind (GLuint buffer)
{
  glBindBuffer (GL_ARRAY_BUFFER, buffer);
  assert (egl_arraybuffer_get_binding () == buffer);
}

void egl_arraybuffer_load (GLintptr offset, GLsizeiptr size, const GLvoid * data)
{
  assert (egl_arraybuffer_get_binding() > 0);
  assert (egl_arraybuffer_get_size() >= (offset + size));
  glBufferSubData (GL_ARRAY_BUFFER, offset, size, data);
}

void egl_opengl_init ()
{
  GLFWwindow* w;
  assert (glfwInit() == GL_TRUE);
  glfwWindowHint( GLFW_VISIBLE, GL_FALSE );
  w = glfwCreateWindow (1, 1, "NULL", NULL, NULL);
  assert (w != NULL);
  glfwMakeContextCurrent (w);
  assert (glewInit() == GLEW_OK);
  glfwDestroyWindow (w);
}



void *worker (void *arg)
{
  float buf [BUF_SIZE];
  struct timespec req = {0, 1000*1000*100}; //0s + 100ms
  struct timespec rem = {0, 0};

  glfwMakeContextCurrent (worker_win);
  while (1)
  {
    for (size_t i = 0; i < BUF_SIZE; i+=2)
    {
      buf [i+0] = rand() % 600;
      buf [i+1] = rand() % 600;
    }
    egl_arraybuffer_bind (vbo);
    egl_arraybuffer_load (0, sizeof(float) * BUF_SIZE, buf);
    glFlush ();
    nanosleep (&req, &rem);
  }
  return NULL;
}

void *renderer (void *arg)
{
  struct timespec req = {0, 1000*1000}; //0s + 1ms
  struct timespec rem = {0, 0};
  glfwMakeContextCurrent (renderer_win);

  vbo = egl_buffer_generate ();
  egl_arraybuffer_bind (vbo);
  egl_arraybuffer_allocate (sizeof (float) * BUF_SIZE, GL_DYNAMIC_DRAW);

  while (!glfwWindowShouldClose (renderer_win))
  {
    glClear (GL_COLOR_BUFFER_BIT);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glOrtho (0, 600, 0, 600, -1, 1);
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();
    glEnableClientState (GL_VERTEX_ARRAY);
    glVertexPointer (2, GL_FLOAT, 0, 0);
    glColor3ub (255, 0, 0);
    glDrawArrays (GL_LINES, 0, 256);
    glDisableClientState (GL_VERTEX_ARRAY);
    glfwSwapBuffers (renderer_win);
    glfwPollEvents ();
    nanosleep (&req, &rem);
  }

  return NULL;
}


int main( int argc, char** argv )
{
  pthread_t worker_pthread;
  pthread_t renderer_pthread;
  egl_opengl_init ();

  glfwWindowHint( GLFW_VISIBLE, GL_FALSE );
  worker_win = glfwCreateWindow (1, 1, "Thread Window", NULL, NULL);
  assert (worker_win != NULL);

  glfwWindowHint( GLFW_VISIBLE, GL_TRUE );
  renderer_win = glfwCreateWindow (600, 600, "Hello World", NULL, worker_win);
  assert (renderer_win != NULL);

  pthread_create (&worker_pthread, NULL, worker, NULL);
  //pthread_create (&renderer_pthread, NULL, renderer, NULL);
  //pthread_join (renderer_pthread, NULL);
  renderer (NULL);

  return 0;
}
