//Should the dll get loaded into a chrome process?
if (FindPidInTree(HandleToULong(InProcessId)) == 0) {
	//Maybe we're just coming early into this routine - check if this is a chrome.exe process
	LPSTR OpenedProcName = GetProcessNameFromPid(InProcessId);
	//Is this is a chrome process? -> check if the dll is okay.
	if (strcmp(OpenedProcName, "chrome.exe") == 0)
		goto Check;
	//No - then do nothing
	goto Allow;
}