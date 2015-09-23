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

#define LED_GLfloat_Size sizeof(GLfloat)

#define LED_Matrix4_Count 16
#define LED_Matrix3_Count 9
#define LED_GLfloat_Matrix4_Size (LED_Matrix4_Count * LED_GLfloat_Size)
#define LED_GLfloat_Matrix3_Size (LED_Matrix3_Count * LED_GLfloat_Size)
typedef GLfloat LED_GLfloat_Matrix4[LED_Matrix4_Count];
typedef GLfloat LED_GLfloat_matrix3[LED_Matrix3_Count];

#define PI 3.14159265359

#define LED_Matrix_Map(N, R, C) (R + (N * C))
#define LED_Matrix4_Position_11 LED_Matrix_Map(4, 0, 0)
#define LED_Matrix4_Position_21 LED_Matrix_Map(4, 1, 0)
#define LED_Matrix4_Position_31 LED_Matrix_Map(4, 2, 0)
#define LED_Matrix4_Position_41 LED_Matrix_Map(4, 3, 0)
#define LED_Matrix4_Position_12 LED_Matrix_Map(4, 0, 1)
#define LED_Matrix4_Position_22 LED_Matrix_Map(4, 1, 1)
#define LED_Matrix4_Position_32 LED_Matrix_Map(4, 2, 1)
#define LED_Matrix4_Position_42 LED_Matrix_Map(4, 3, 1)
#define LED_Matrix4_Position_13 LED_Matrix_Map(4, 0, 2)
#define LED_Matrix4_Position_23 LED_Matrix_Map(4, 1, 2)
#define LED_Matrix4_Position_33 LED_Matrix_Map(4, 2, 2)
#define LED_Matrix4_Position_43 LED_Matrix_Map(4, 3, 2)
#define LED_Matrix4_Position_14 LED_Matrix_Map(4, 0, 3)
#define LED_Matrix4_Position_24 LED_Matrix_Map(4, 1, 3)
#define LED_Matrix4_Position_34 LED_Matrix_Map(4, 2, 3)
#define LED_Matrix4_Position_44 LED_Matrix_Map(4, 3, 3)



int LED_Read_File (char * Name, char * Buffer, int Buffer_Count)
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

void LED_Read_File_Null_Terminated (char * Name, char * Buffer, int Buffer_Count)
{
  int File_Count;
  File_Count = LED_Read_File (Name, Buffer, Buffer_Count);
  Buffer[File_Count] = '\0';
}

GLuint LED_GL_Shader_File (GLenum Type, char * Name, char * Buffer, int Buffer_Count)
{
  GLuint Shader;
  Shader = glCreateShader (Type);
  assert (Shader > 0);
  LED_Read_File_Null_Terminated (Name, Buffer, Buffer_Count);
  glShaderSource (Shader, 1, (const GLchar**)&Buffer, 0);
  glCompileShader (Shader);
  return Shader;
}

void LED_GL_Shader_Files (const GLenum * Types, char ** Names, int Count, char * Buffer, int Buffer_Count, GLuint * Shader_Objects)
{
  for (int i = 0; i < Count; i++)
  {
    Shader_Objects[i] = LED_GL_Shader_File (Types[i], Names[i], Buffer, Buffer_Count);
  }
}


GLuint LED_GL_Create_Program (GLuint * Shader_Objects, int Count)
{
  GLuint Program;
  GLint status;
  assert (Count > 0);
  Program = glCreateProgram ();
  assert (Program > 0);
  for (int i = 0; i < Count; i++)
  {
    assert (Shader_Objects[i] > 0);
    glAttachShader (Program, Shader_Objects[i]);
  }
  glLinkProgram (Program);
  return Program;
}




void LED_GLfloat_Matrix_Multiply_Accumulate (int Count, GLfloat * A, GLfloat * B, GLfloat * R)
{
  for (int i = 0; i < Count; i++)
  {
    for (int j = 0; j < Count; j++)
    {
      for (int k = 0; k < Count; k++)
      {
        R[LED_Matrix_Map(Count, i, j)] = R[LED_Matrix_Map(Count, i, j)] + A[LED_Matrix_Map(Count, i, k)] * B[LED_Matrix_Map(Count, k, j)];
      }
    }
  }
}

void LED_GLfloat_Matrix_Product (int Count, GLfloat * A, GLfloat * B, GLfloat * R)
{
  for (int i = 0; i < Count; i++)
  {
    for (int j = 0; j < Count; j++)
    {
      R[LED_Matrix_Map (Count, i, j)] = 0.0f;
      for (int k = 0; k < Count; k++)
      {
        R[LED_Matrix_Map (Count, i, j)] = R[LED_Matrix_Map (Count, i, j)] + A[LED_Matrix_Map (Count, i, k)] * B[LED_Matrix_Map (Count, k, j)];
      }
    }
  }
}


void LED_GLfloat_Matrix_Make_Identity (int Count, GLfloat * Result)
{
  memset (Result, 0, (Count * Count) * LED_GLfloat_Size);
  for (int i = 0; i < Count; i++)
  {
    Result[LED_Matrix_Map(Count, i, i)] = 1.0;
  }
}



void LED_GLfloat_Matrix_Make_Frustum (LED_GLfloat_Matrix4 Matrix, float l, float r, float b, float t, float n, float f)
{
  float n2;
  float rl;
  float tb;
  float nf;

  n2 = 2.0 * n;
  rl = r - l;
  tb = t - b;
  nf = n - f;

  Matrix[LED_Matrix4_Position_11] = n2 / rl;
  Matrix[LED_Matrix4_Position_21] = 0.0;
  Matrix[LED_Matrix4_Position_31] = 0.0;
  Matrix[LED_Matrix4_Position_41] = 0.0;

  Matrix[LED_Matrix4_Position_12] = 0.0;
  Matrix[LED_Matrix4_Position_22] = n2 / tb;
  Matrix[LED_Matrix4_Position_32] = 0.0;
  Matrix[LED_Matrix4_Position_42] = 0.0;

  Matrix[LED_Matrix4_Position_13] = (r + l) / rl;
  Matrix[LED_Matrix4_Position_23] = (t + b) / tb;
  Matrix[LED_Matrix4_Position_33] = (f + n) / nf;
  Matrix[LED_Matrix4_Position_43] = -1.0;

  Matrix[LED_Matrix4_Position_14] = 0.0;
  Matrix[LED_Matrix4_Position_24] = 0.0;
  Matrix[LED_Matrix4_Position_34] = (f * n2) / nf;
  Matrix[LED_Matrix4_Position_44] = 0.0;
}

void LED_GLfloat_Matrix_Make_Perspective (float *matrix, float fov, float aspect, float n, float f)
{
    float w;
    float l;
    l = n * tanf(fov * PI / 360.0);
    w = l * aspect;
    LED_GLfloat_Matrix_Make_Frustum (matrix, -w, w, -l, l, n, f);
}

void LED_GLfloat_Matrix_Make_Translation (float *matrix, float t0, float t1, float t2)
{
  matrix[LED_Matrix4_Position_11] = 1.0f;
  matrix[LED_Matrix4_Position_21] = 0.0f;
  matrix[LED_Matrix4_Position_31] = 0.0f;
  matrix[LED_Matrix4_Position_41] = 0.0f;

  matrix[LED_Matrix4_Position_12] = 0.0f;
  matrix[LED_Matrix4_Position_22] = 1.0f;
  matrix[LED_Matrix4_Position_32] = 0.0f;
  matrix[LED_Matrix4_Position_42] = 0.0f;

  matrix[LED_Matrix4_Position_13] = 0.0f;
  matrix[LED_Matrix4_Position_23] = 0.0f;
  matrix[LED_Matrix4_Position_33] = 1.0f;
  matrix[LED_Matrix4_Position_43] = 0.0f;

  matrix[LED_Matrix4_Position_14] = t0;
  matrix[LED_Matrix4_Position_24] = t1;
  matrix[LED_Matrix4_Position_34] = t2;
  matrix[LED_Matrix4_Position_44] = 1.0f;
}



void LED_Print_Float_Matrix (float *matrix, int n)
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
  LED_GL_Shader_Files (Shader_Types, Shader_Names, Shader_Count, Char_Buffer, Char_Buffer_Length, Shader_Objects);
  Assert_Shaders (Shader_Objects, Shader_Count, Char_Buffer, Char_Buffer_Length);
  Program_Object = LED_GL_Create_Program (Shader_Objects, Shader_Count);
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


#define Vertex_Info_Position_Location 0
#define Vertex_Info_Color_Location 1
#define Vertex_Info_Position_Count 3
#define Vertex_Info_Color_Count 3
struct Vertex
{
  GLfloat Position[Vertex_Info_Position_Count];
  GLubyte Color[Vertex_Info_Color_Count];
};
#define Vertex_Info_Position_Offset (0)
#define Vertex_Info_Color_Offset (Vertex_Info_Position_Count * sizeof(GLfloat))
#define Vertex_Info_Position_Type GL_FLOAT
#define Vertex_Info_Color_Type GL_UNSIGNED_BYTE
#define Vertex_Info_Size (sizeof(struct Vertex))

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



LED_GLfloat_Matrix4 transformation = {0.0f};
LED_GLfloat_Matrix4 mat_perspective;
LED_GLfloat_Matrix4 mat_view;

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
  glEnableVertexAttribArray (Vertex_Info_Position_Location);
  glVertexAttribPointer (Vertex_Info_Position_Location, Vertex_Info_Position_Count, Vertex_Info_Position_Type, GL_FALSE, Vertex_Info_Size, (const GLvoid *)Vertex_Info_Position_Offset);
  glEnableVertexAttribArray (Vertex_Info_Color_Location);
  glVertexAttribPointer (Vertex_Info_Color_Location, Vertex_Info_Color_Count, Vertex_Info_Color_Type, GL_TRUE, Vertex_Info_Size, (const GLvoid *)Vertex_Info_Color_Offset);

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

    LED_GLfloat_Matrix_Make_Perspective (mat_perspective, 90.0f, 1.0f, 0.1f, 100.0f);
    //LED_GLfloat_Matrix_Make_Identity (4, mat_perspective);
    LED_GLfloat_Matrix_Make_Translation (mat_view, Position[0], Position[1], Position[2]);
    LED_GLfloat_Matrix_Product (4, mat_perspective, mat_view, transformation);
    LED_Print_Float_Matrix (transformation, 4);
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
