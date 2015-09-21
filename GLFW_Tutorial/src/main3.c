#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>


struct Winbuf
{
  GLFWwindow* Window;
  GLuint VBO;
};


void *Worker (void *arg)
{
  struct Winbuf* Winbuf0;
  float Buffer [512];

  Winbuf0 = (struct Winbuf*) arg;
  glfwMakeContextCurrent (Winbuf0->Window);
  //glewInit();
  while (1)
  {
    for (size_t i = 0; i < 512; i+=2)
    {
      Buffer [i+0] = rand() % 600;
      Buffer [i+1] = rand() % 600;
    }
    glBufferSubData (GL_ARRAY_BUFFER, 0, sizeof(float) * 512, Buffer);
    glFlush ();
    usleep (1000000);
  }
  return NULL;
}




int main( int argc, char** argv )
{
  GLFWwindow* window;
  struct Winbuf Winbuf0;
  pthread_t Worker_Type;

  srand (time (NULL));

  if( !glfwInit() )
      return -1;

  glfwWindowHint( GLFW_VISIBLE, GL_FALSE );
  Winbuf0.Window = glfwCreateWindow (1, 1, "Thread Window", NULL, NULL);
  glfwWindowHint( GLFW_VISIBLE, GL_TRUE );
  window = glfwCreateWindow (600, 600, "Hello World", NULL, Winbuf0.Window);
  glfwMakeContextCurrent (window);
  glewInit ();

  glGenBuffers (1, &Winbuf0.VBO);
  glBindBuffer (GL_ARRAY_BUFFER, Winbuf0.VBO);
  glBufferData (GL_ARRAY_BUFFER, sizeof(float) * 512, NULL, GL_DYNAMIC_DRAW);



  pthread_create (&Worker_Type, NULL, Worker, &Winbuf0);
  //pthread_join (Worker_Thread_Type, NULL);


while (!glfwWindowShouldClose (window))
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
  glfwSwapBuffers (window);
  glfwPollEvents ();
}

  return 0;
}
