#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include <math.h>
#include <unistd.h>

#define Agl_Matrix4_Count 16
#define Agl_Matrix3_Count 9
typedef GLfloat Agl_Float_Matrix4[Agl_Matrix4_Count];
typedef GLfloat Agl_float_matrix3[Agl_Matrix3_Count];
#define PI 3.14159265359
#define Agl_Matrix_Map(N, R, C) R + (N * C)
#define Agl_Matrix4_Position_11 Agl_Matrix_Map(4, 0, 0)
#define Agl_Matrix4_Position_21 Agl_Matrix_Map(4, 1, 0)
#define Agl_Matrix4_Position_31 Agl_Matrix_Map(4, 2, 0)
#define Agl_Matrix4_Position_41 Agl_Matrix_Map(4, 3, 0)
#define Agl_Matrix4_Position_12 Agl_Matrix_Map(4, 0, 1)
#define Agl_Matrix4_Position_22 Agl_Matrix_Map(4, 1, 1)
#define Agl_Matrix4_Position_32 Agl_Matrix_Map(4, 2, 1)
#define Agl_Matrix4_Position_42 Agl_Matrix_Map(4, 3, 1)
#define Agl_Matrix4_Position_13 Agl_Matrix_Map(4, 0, 2)
#define Agl_Matrix4_Position_23 Agl_Matrix_Map(4, 1, 2)
#define Agl_Matrix4_Position_33 Agl_Matrix_Map(4, 2, 2)
#define Agl_Matrix4_Position_43 Agl_Matrix_Map(4, 3, 2)
#define Agl_Matrix4_Position_14 Agl_Matrix_Map(4, 0, 3)
#define Agl_Matrix4_Position_24 Agl_Matrix_Map(4, 1, 3)
#define Agl_Matrix4_Position_34 Agl_Matrix_Map(4, 2, 3)
#define Agl_Matrix4_Position_44 Agl_Matrix_Map(4, 3, 3)



int Agl_Read_File (char * Name, char * Buffer, int Buffer_Count)
{
  FILE *File;
  size_t File_Count;
  assert (Name != NULL);
  assert (Buffer != NULL);
  assert (Buffer_Count > 0);
  File = fopen (Name, "r");
  assert (File != NULL);
  File_Count = fread (Buffer, sizeof(char), Buffer_Count, File);
  assert (File_Count != 0);
  fclose (File);
  return File_Count;
}

void Agl_Read_File_Null_Terminated (char * Name, char * Buffer, int Buffer_Count)
{
  int File_Count;
  File_Count = Agl_Read_File (Name, Buffer, Buffer_Count);
  Buffer[File_Count] = '\0';
}

GLuint Agl_Create_Shader (GLenum Type, char * Name, char * Buffer, int Buffer_Count)
{
  GLuint Shader;
  Shader = glCreateShader (Type);
  assert (Shader > 0);
  Agl_Read_File_Null_Terminated (Name, Buffer, Buffer_Count);
  glShaderSource (Shader, 1, (const GLchar**)&Buffer, 0);
  glCompileShader (Shader);
  return Shader;
}

void Agl_Create_Shaders (const GLenum * Types, char ** Names, int Count, char * Buffer, int Buffer_Count, GLuint * Shader_Objects)
{
  for (int i = 0; i < Count; i++)
  {
    Shader_Objects[i] = Agl_Create_Shader (Types[i], Names[i], Buffer, Buffer_Count);
  }
}


GLuint Agl_Create_Program (GLuint * shader_objects, int n)
{
  GLuint program;
  GLint status;
  GLint log_length;
  assert (n > 0);
  program = glCreateProgram ();
  assert (program > 0);
  for (int i = 0; i < n; i++)
  {
    assert (shader_objects[i] > 0);
    glAttachShader (program, shader_objects[i]);
  }
  glLinkProgram (program);
  return program;
}


void Assert_Shader (GLuint shader, char * buffer, int buffer_length)
{
  GLint status = GL_FALSE;
  GLsizei log_length;
  printf ("Shader: %i\n", shader);
  glGetShaderiv (shader, GL_COMPILE_STATUS, &status);
  printf ("GL_COMPILE_STATUS: %i\n", status);
  glGetShaderiv (shader, GL_INFO_LOG_LENGTH, &log_length);
  printf ("GL_INFO_LOG_LENGTH: %i\n", log_length);
  if (status == GL_FALSE)
  {
    glGetShaderInfoLog (shader, buffer_length, &log_length, buffer);
    printf ("glGetShaderInfoLog:\n%s\n\n", buffer);
  }
  assert (status == GL_TRUE);
}

void Assert_Shaders (GLuint * shader_objects, int shader_Count, char * buffer, int buffer_length)
{
  for (int i = 0; i < shader_Count; i++)
  {
    Assert_Shader (shader_objects[i], buffer, buffer_length);
  }
}

void Assert_Program (GLuint program, char * buffer, int buffer_length)
{
  GLint status = GL_FALSE;
  GLsizei log_length;
  printf ("Program: %i\n", program);
  glGetProgramiv (program, GL_LINK_STATUS, &status);
  printf ("GL_LINK_STATUS: %i\n", status);
  glGetProgramiv (program, GL_INFO_LOG_LENGTH, &log_length);
  printf ("GL_INFO_LOG_LENGTH: %i\n", log_length);
  if (status == GL_FALSE)
  {
    glGetProgramInfoLog (program, buffer_length, &log_length, buffer);
    printf ("glGetProgramInfoLog:\n%s\n\n", buffer);
  }
  assert (status == GL_TRUE);
}


void Agl_Float_Matrix_Multiply_Accumulate (int n, GLfloat * A, GLfloat * B, GLfloat * R)
{
  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < n; j++)
    {
      for (int k = 0; k < n; k++)
      {
        R[Agl_Matrix_Map(n, i, j)] = R[Agl_Matrix_Map(n, i, j)] + A[Agl_Matrix_Map(n, i, k)] * B[Agl_Matrix_Map(n, k, j)];
      }
    }
  }
}

void Agl_Float_Matrix_Product (int n, GLfloat * A, GLfloat * B, GLfloat * R)
{
  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < n; j++)
    {
      R[Agl_Matrix_Map (n, i, j)] = 0.0f;
      for (int k = 0; k < n; k++)
      {
        R[Agl_Matrix_Map (n, i, j)] = R[Agl_Matrix_Map (n, i, j)] + A[Agl_Matrix_Map (n, i, k)] * B[Agl_Matrix_Map (n, k, j)];
      }
    }
  }
}


void Agl_Float_Matrix_Make_Identity (int n, GLfloat * R)
{
  memset (R, 0, n * n);
  for (int i = 0; i < n; i++)
  {
    R[(i) + (n * i)] = 1.0;
  }
}



void Agl_Float_Matrix_Make_Frustum (Agl_Float_Matrix4 Matrix, float l, float r, float b, float t, float n, float f)
{
  float n2;
  float rl;
  float tb;
  float nf;

  n2 = 2.0 * n;
  rl = r - l;
  tb = t - b;
  nf = n - f;

  Matrix[Agl_Matrix4_Position_11] = n2 / rl;
  Matrix[Agl_Matrix4_Position_21] = 0.0;
  Matrix[Agl_Matrix4_Position_31] = 0.0;
  Matrix[Agl_Matrix4_Position_41] = 0.0;

  Matrix[Agl_Matrix4_Position_12] = 0.0;
  Matrix[Agl_Matrix4_Position_22] = n2 / tb;
  Matrix[Agl_Matrix4_Position_32] = 0.0;
  Matrix[Agl_Matrix4_Position_42] = 0.0;

  Matrix[Agl_Matrix4_Position_13] = (r + l) / rl;
  Matrix[Agl_Matrix4_Position_23] = (t + b) / tb;
  Matrix[Agl_Matrix4_Position_33] = (f + n) / nf;
  Matrix[Agl_Matrix4_Position_43] = -1.0;

  Matrix[Agl_Matrix4_Position_14] = 0.0;
  Matrix[Agl_Matrix4_Position_24] = 0.0;
  Matrix[Agl_Matrix4_Position_34] = (f * n2) / nf;
  Matrix[Agl_Matrix4_Position_44] = 0.0;
}

void Agl_Float_Matrix_Make_Perspective (float *matrix, float fov, float aspect, float n, float f)
{
    float w;
    float l;
    l = n * tanf(fov * PI / 360.0);
    w = l * aspect;
    Agl_Float_Matrix_Make_Frustum (matrix, -w, w, -l, l, n, f);
}

void Agl_Float_Matrix_Make_Translation (float *matrix, float t0, float t1, float t2)
{
  matrix[Agl_Matrix4_Position_11] = 1.0f;
  matrix[Agl_Matrix4_Position_21] = 0.0f;
  matrix[Agl_Matrix4_Position_31] = 0.0f;
  matrix[Agl_Matrix4_Position_41] = 0.0f;

  matrix[Agl_Matrix4_Position_12] = 0.0f;
  matrix[Agl_Matrix4_Position_22] = 1.0f;
  matrix[Agl_Matrix4_Position_32] = 0.0f;
  matrix[Agl_Matrix4_Position_42] = 0.0f;

  matrix[Agl_Matrix4_Position_13] = 0.0f;
  matrix[Agl_Matrix4_Position_23] = 0.0f;
  matrix[Agl_Matrix4_Position_33] = 1.0f;
  matrix[Agl_Matrix4_Position_43] = 0.0f;

  matrix[Agl_Matrix4_Position_14] = t0;
  matrix[Agl_Matrix4_Position_24] = t1;
  matrix[Agl_Matrix4_Position_34] = t2;
  matrix[Agl_Matrix4_Position_44] = 1.0f;
}



void Agl_Print_Float_Matrix (float *matrix, int n)
{
  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < n; j++)
    {
      printf ("%f ", matrix [(i) + (n * j)]);
    }
    printf ("\n");
  }
}
















void Setup_GLEW_And_GLFW ()
{
  GLFWwindow* w;
  assert (glfwInit() == GL_TRUE);
  glfwWindowHint (GLFW_VISIBLE, GL_FALSE);
  w = glfwCreateWindow (1, 1, "NULL", NULL, NULL);
  assert (w != NULL);
  glfwMakeContextCurrent (w);
  assert (glewInit () == GLEW_OK);
  glfwDestroyWindow (w);
}


GLuint Setup_Shader_Program (char * Char_Buffer, int Char_Buffer_Length)
{
  #define Shader_Count 2
  GLuint Shader_Objects[Shader_Count];
  char * Shader_Names[Shader_Count] = {"shader.glvs", "shader.glfs"};
  GLenum Shader_Types[Shader_Count] = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};
  GLuint Program_Object;
  Agl_Create_Shaders (Shader_Types, Shader_Names, Shader_Count, Char_Buffer, Char_Buffer_Length, Shader_Objects);
  Assert_Shaders (Shader_Objects, Shader_Count, Char_Buffer, Char_Buffer_Length);
  Program_Object = Agl_Create_Program (Shader_Objects, Shader_Count);
  Assert_Program (Program_Object, Char_Buffer, Char_Buffer_Length);
  return Program_Object;
  #undef Shader_Count
}





#define Axis_X 0
#define Axis_Y 1
#define Axis_Z 2

#define Action_Go_Forward GLFW_KEY_W
#define Action_Go_Backward GLFW_KEY_S


void Register_Input (GLFWwindow * Window, int * States)
{
  States[GLFW_KEY_W] = glfwGetKey (Window, GLFW_KEY_W);
  States[GLFW_KEY_S] = glfwGetKey (Window, GLFW_KEY_S);
}

void Handle_Input (int * States, GLfloat * Position)
{
  if (States[Action_Go_Forward] == GLFW_PRESS)
  {
    Position[Axis_Z] += 0.1f;
  }
  if (States[Action_Go_Backward] == GLFW_PRESS)
  {
    Position[Axis_Z] += -0.1f;
  }
}







#define Key_States_Count 100
int Key_States[Key_States_Count];

#define General_Purpuse_Char_Buffer_Count 1000
char General_Purpuse_Char_Buffer[General_Purpuse_Char_Buffer_Count];



struct Vertex
{
  GLfloat Position[3];
  GLubyte Color[3];
};

#define Vertex_Specification_Position_Offset (0)
#define Vertex_Specification_Color_Offset (3 * sizeof(GLfloat))
#define Vertex_Specification_Size (sizeof(struct Vertex))

struct Vertex vertices[4] =
{
  {.Position = {1.0f, 1.0f, 0.0f},    .Color = {255, 111, 023}},
  {.Position = {-1.0f, 1.0f, 0.0f},   .Color = {052, 255, 100}},
  {.Position = {1.0f,-1.0f, 0.0f},    .Color = {241, 123, 055}},
  {.Position = {-1.0f,-1.0f, 0.0f},   .Color = {032, 060, 025}}
};

GLuint vertex_indices[] =
{
  0,1,2, // first triangle
  2,1,3, // second triangle
};



Agl_Float_Matrix4 transformation = {0.0f};
Agl_Float_Matrix4 mat_perspective;
Agl_Float_Matrix4 mat_view;

GLFWwindow* main_window;
GLuint shader_program;
GLuint vao, vbo, ibo;
GLint transformation_location;
GLfloat Position[3] = {0};



int main (int argc, char** argv)
{
  Setup_GLEW_And_GLFW ();
  glfwWindowHint (GLFW_VISIBLE, GL_TRUE);
  main_window = glfwCreateWindow (600, 600, "Hello World", NULL, NULL);
  assert (main_window != NULL);
  glfwMakeContextCurrent (main_window);

  printf ("GL_VERSION %s\n", glGetString (GL_VERSION));


  glGenVertexArrays (1, &vao);
  glGenBuffers (1, &vbo);
  glGenBuffers (1, &ibo);

  glBindBuffer (GL_ARRAY_BUFFER, vbo);
  glBufferData (GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindVertexArray (vao);
  glEnableVertexAttribArray (0);
  glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, Vertex_Specification_Size, (const GLvoid *)Vertex_Specification_Position_Offset);
  glEnableVertexAttribArray (1);
  glVertexAttribPointer (1, 3, GL_UNSIGNED_BYTE, GL_TRUE, Vertex_Specification_Size, (const GLvoid *)Vertex_Specification_Color_Offset);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex_indices), vertex_indices, GL_STATIC_DRAW);


  shader_program = Setup_Shader_Program (General_Purpuse_Char_Buffer, General_Purpuse_Char_Buffer_Count);
  glUseProgram (shader_program);

  GLint transformation_location = glGetUniformLocation (shader_program, "transformation");



  while (!glfwWindowShouldClose (main_window))
  {
    glfwPollEvents ();
    glClear (GL_COLOR_BUFFER_BIT);

    Register_Input (main_window, Key_States);
    Handle_Input (Key_States, Position);

    Agl_Float_Matrix_Make_Perspective (mat_perspective, 90.0f, 1.0f, 0.1f, 100.0f);
    //Agl_Float_Matrix_Make_Identity (4, mat_perspective);
    Agl_Float_Matrix_Make_Translation (mat_view, Position[0], Position[1], Position[2]);
    Agl_Float_Matrix_Product (4, mat_perspective, mat_view, transformation);
    Agl_Print_Float_Matrix (transformation, 4);
    printf ("\n");

    glUniformMatrix4fv (transformation_location, 1, GL_FALSE, transformation);

    glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    GLenum error = glGetError ();
    if (error != GL_NO_ERROR)
    {
      printf ("glGetError: %i\n", error);
      break;
    }
    glfwSwapBuffers (main_window);

    struct timespec req = {0, 1000*1000*100}; //0s + 100ms
    struct timespec rem = {0, 0};
    nanosleep (&req, &rem);

  }



  return 0;
}
