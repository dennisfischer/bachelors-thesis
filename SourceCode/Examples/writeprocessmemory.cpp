#include <stdio.h>
#include <iostream>
#include <tchar.h>

#include "Windows.h"
#pragma comment(lib, "advapi32.lib")

#include <SDKDDKVer.h>

HANDLE CreateProcessHandle(unsigned long dwProcessId);
HANDLE CallWithRemoteThread(HANDLE hProcess, LPVOID hModule, 
	LPVOID lp_base_address);

int main()
{
	//Path to the DLL that should get loaded into the remote process
	std::string DllPath = "...";
	//The current pid of the target process
	DWORD dwProcessId = 0;

	//First setup the required process handle
	auto hProcess = CreateProcessHandle(dwProcessId);

	//Allocate memory for the DLL path inside the target process
	//This address is the parameter for LoadLibrary later in the code
	auto lpBaseAdress = VirtualAllocEx(hProcess, nullptr, DllPath.size(),
		MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (lpBaseAdress == nullptr)
	{
		printf("Failed allocating memory in target process: %d\n", 
			GetLastError());
		return -1;
	}

	//Write the DLL path into the target process
	if (!WriteProcessMemory(hProcess, lpBaseAdress, DllPath.c_str(),
		DllPath.size(), nullptr))
	{
		printf("Failed writing into process memory: %d\n", GetLastError());
		return -2;
	}

	//Get the address of the LoadLibraryA function of kernel32.dll
	auto hModule = PTHREAD_START_ROUTINE(GetProcAddress(
		GetModuleHandleA("kernel32.dll"), "LoadLibraryA"));

	//Call LoadLibraryA with a remote process to complete the DLL injection
	auto hRemoteThread = CallWithRemoteThread(hProcess, hModule, lpBaseAdress);

	//The application can wait now until the just started thread has finished.
	//This allows to check the return value and call other functions of the DLL
	if (WaitForSingleObject(hRemoteThread, INFINITE) == WAIT_FAILED)
	{
		printf("Failed waiting for remote thread to end: %d\n", GetLastError());
		return -3;
	}

	//In this case, the return value of the remote thread is the DLL base 
	//address inside the virtual memory of the target process
	DWORD dllBaseAddress = 0;
	GetExitCodeThread(hRemoteThread, &dllBaseAddress);

	//Cleanup, free memory in target process
	if (hRemoteThread != nullptr)
	{
		CloseHandle(hRemoteThread);
	}
	VirtualFreeEx(hProcess, lpBaseAdress, 0, MEM_RELEASE);
    return 0;
}

//This function creates a process handle given a process id
HANDLE CreateProcessHandle(unsigned long dwProcessId)
{
	//OpenProcess is specified with PROCESS_ALL_ACCESS
	//This could be lowered to just the required rights
	//In this case PROCESS_VM_OPERATION | PROCESS_VM_WRITE are enough.
	auto hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, dwProcessId);
	if (!hProcess)
	{
		printf("Couldn't receive process handle: %d\n", GetLastError());
		return nullptr;
	}
	return hProcess;
}

//This function starts a remote thread inside hProcess.
//The function given by hModule is called with the given arguments 
//located in lp_base_address
HANDLE CallWithRemoteThread(HANDLE hProcess, LPVOID hModule, 
	LPVOID lp_base_address)
{
	auto hRemoteHandle = CreateRemoteThread(hProcess, nullptr, 0,
		LPTHREAD_START_ROUTINE(hModule), lp_base_address, 0, nullptr);
	auto error = GetLastError();
	if (error == ERROR_ACCESS_DENIED)
	{
		printf("Access was denied by process: %d\n", error);
	}
	else if (error != 0)
	{
		printf("Unknown error during remote thread creation: %d\n", error);
	}
	return hRemoteHandle;
}