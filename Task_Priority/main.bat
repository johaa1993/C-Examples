cd bin
del main.exe
cd ..

gcc -o bin\main src\main.c

cd bin
start main.exe
cd ..

pause
