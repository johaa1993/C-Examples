del "bin\main.exe"
gcc -std=gnu99 src\main4.c -lpthread -Ilib\glew-1.13.0\include -Ilib\glfw-3.1.1.bin.WIN32\include -Llib\glew-1.13.0\lib\Release\x64 -Llib/glfw-3.1.1.bin.WIN64/lib-mingw -lglfw3 -lglew32s -lgdi32 -lopengl32 -obin/main
cd bin
main


pause
