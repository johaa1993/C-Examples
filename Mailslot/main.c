#include <Windows.h>


HANDLE Mailslot_create(const char* name)
{
	HANDLE handle;
	char slotname[100];
	strcpy_s(slotname, 100, "\\\\.\\mailslot\\");
	strcat_s(slotname, 100, name);
	handle = CreateMailslot(
		(LPCTSTR) slotname,
		(DWORD) 0,
		(DWORD) MAILSLOT_WAIT_FOREVER,
		(LPSECURITY_ATTRIBUTES) NULL
	);
	return handle;
}



int Mailslot_read(const HANDLE mailslot, const char* buffer, const int bufferSize)
{
	int bytes;
	ReadFile(
		(HANDLE) mailslot,
		(LPVOID) buffer,
		(DWORD) bufferSize,
		(LPDWORD) &bytes,
		(LPOVERLAPPED) 0
	);
	return bytes;
}

HANDLE Mailslot_connect(const char* name)
{
	HANDLE handle;
	char slotname[100];
	strcpy_s(slotname, 100, "\\\\.\\mailslot\\");
	strcat_s(slotname, 100, name);
	handle = CreateFile(
		(LPCTSTR) slotname,
		(DWORD) GENERIC_WRITE,
		(DWORD) FILE_SHARE_READ,
		(LPSECURITY_ATTRIBUTES) 0,
		(DWORD) OPEN_EXISTING,
		(DWORD) FILE_ATTRIBUTE_NORMAL,
		(HANDLE) 0
	);
	return handle;
}

int Mailslot_write(const HANDLE mailconnection, const char* buffer, const int bufferSize)
{
	int bytes;
	WriteFile(
		(HANDLE) mailconnection,
		(LPCVOID) buffer,
		(DWORD) bufferSize,
		(LPDWORD) &bytes,
		(LPOVERLAPPED) 0
	);
	return bytes;
}




int main(const int argc, const char** argv)
{
	HANDLE server;
	HANDLE serverConnection;

	server = Mailslot_create("server");

	serverConnection = Mailslot_connect("server");
	Mailslot_write(serverConnection, "The first line", 100);
	Mailslot_write(serverConnection, "The second line", 100);

	{
		char buffer[100] = {0};
		while (Mailslot_read(server, buffer, 100)){
			printf("%s\n", buffer);
		}
	}
	while (1);
}