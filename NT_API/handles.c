#include <Windows.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	
	if (argc < 2) {
		puts("usage: handle.exe <PID>");
		return -1;
	}

	puts("======================[DISSECT ME v1.0================");
	DWORD PID = atoi(argv[1]);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);

	if (hProcess == NULL) {
		printf("[OpenProcess] faild, error 0x%1x", GetLastError());
		return -1;
	}

	printf("\n[0x%p] got a handle to the process!\n", hProcess);
	CloseHandle(hProcess);
	puts("[>] press <enter> to exit");
	(void)getchar();
	return 0;
}