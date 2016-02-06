//Should the dll get loaded into a chrome process?
if (FindPidInTree(HandleToULong(InProcessId)) == 0)
{
	//Maybe we're just coming early into this routine - check if this is a chrome.exe process
	LPSTR OpenedProcName = GetProcessNameFromPid(InProcessId);

	if (strcmp(OpenedProcName, "chrome.exe") == 0)
	{
		//This is a chrome process, check if the dlls are okay.
		goto Check;
	}

	//No - then do nothing
	DEBUG("Not tracked: PID: %d ImageName: %wZ\n", HandleToLong(InProcessId), InFullImageName);
	goto Allow;
}