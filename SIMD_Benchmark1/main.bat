del "bin\main.exe"
::gcc -o bin/main src\main.c src\Timer.c
::gcc -S src\main.c src\Timer.c
gcc -o bin/main src\main5.c src\Timer.c src/SIMD.c -msse4.2
cd bin
start main
pause
