#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <boost/thread.hpp>

using namespace std;
using namespace boost;

// reload shared VBO with random data every second
void MyThread( GLFWwindow* win, GLuint vbo )
{
    glfwMakeContextCurrent( win );
    glewInit();
    while( true )
    {
        float temp[ 512 ];
        for( size_t i = 0; i < 512; i+=2 )
        {
            temp[ i+0 ] = rand() % 600;
            temp[ i+1 ] = rand() % 600;
        }

        glBindBuffer( GL_ARRAY_BUFFER, vbo );
        glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * 512, temp, GL_DYNAMIC_DRAW );
        glBindBuffer( GL_ARRAY_BUFFER, 0 );

        // oddly important, might need to be glFinish()
        glFlush();

        this_thread::sleep( posix_time::milliseconds( 1000 ) );
    }
}

int main( int argc, char** argv )
{
    if( !glfwInit() )
        return -1;

    glfwWindowHint( GLFW_VISIBLE, GL_FALSE );
    GLFWwindow* threadWin = glfwCreateWindow( 1, 1, "Thread Window", NULL, NULL );

    glfwWindowHint( GLFW_VISIBLE, GL_TRUE );
    GLFWwindow* window = glfwCreateWindow( 600, 600, "Hello World", NULL, threadWin );
    glfwMakeContextCurrent( window );
    glewInit();

    // load shared VBO with dummy data
    float temp[ 512 ];
    GLuint vbo;
    glGenBuffers( 1, &vbo );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * 512, temp, GL_DYNAMIC_DRAW );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    thread aThread( MyThread, threadWin, vbo );

    while( !glfwWindowShouldClose( window ) )
    {
        glClear( GL_COLOR_BUFFER_BIT );

        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        glOrtho( 0, 600, 0, 600, -1, 1 );
        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();

        glEnableClientState( GL_VERTEX_ARRAY );
        glBindBuffer( GL_ARRAY_BUFFER, vbo );
        glVertexPointer( 2, GL_FLOAT, 0, 0 );
        glColor3ub( 255, 0, 0 );
        glDrawArrays( GL_LINES, 0, 256 );
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
        glDisableClientState( GL_VERTEX_ARRAY );

        glfwSwapBuffers( window );
        glfwPollEvents();
        this_thread::sleep( posix_time::milliseconds( 16 ) );
    }

    aThread.interrupt();
    aThread.join();

    glfwTerminate();
    return 0;
}
