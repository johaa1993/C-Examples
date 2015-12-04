cd bin
del Test_BRS_CDLL_Insert_Sorted.exe
del Test_BRS_CDLL_Count.exe
del Test_BRS_Semaphore.exe
del Test_BRS_Schedule_P_Promote.exe
del Test_BRS_Schedule_T_Insert.exe
cd ..

gcc -o bin\Test_BRS_CDLL_Count src\Test_BRS_CDLL_Count.c
gcc -o bin\Test_BRS_CDLL_Insert_Sorted src\Test_BRS_CDLL_Insert_Sorted.c
gcc -o bin\Test_BRS_Semaphore src\Test_BRS_Semaphore.c
gcc -o bin\Test_BRS_Schedule_P_Promote src\Test_BRS_Schedule_P_Promote.c
gcc -o bin\Test_BRS_Schedule_T_Insert src\Test_BRS_Schedule_T_Insert.c

cd bin
::Test_BRS_CDLL_Count.exe
Test_BRS_CDLL_Insert_Sorted.exe
::Test_BRS_Semaphore.exe
::Test_BRS_Schedule_P_Promote.exe
::Test_BRS_Schedule_T_Insert.exe
cd ..


pause
