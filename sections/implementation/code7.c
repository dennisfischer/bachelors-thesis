//Get both processes
PEPROCESS OpenedProcess = (PEPROCESS)InPreInfo->Object;
PEPROCESS CurrentProcess = PsGetCurrentProcess();
//Do we have the same process?
if (OpenedProcess == CurrentProcess)
	goto Exit;
//Get the names of the processes
LPSTR OpenedProcName = GetProcessNameFromPid(PsGetProcessId(OpenedProcess));
LPSTR TargetProcName = GetProcessNameFromPid(PsGetCurrentProcessId());
//..
//Names match (chrome == chrome) or PIDs match
if (!_stricmp(TargetProcName, OpenedProcName) || PsGetProcessId(OpenedProcess) == PsGetCurrentProcessId()) {
	//Are PIDs equal?
	if (PsGetProcessId(OpenedProcess) == PsGetCurrentProcessId())
		goto Exit;
	//Locate both processed inside the process tree
	ULONG currentPid = FindPidInTree(HandleToLong(PsGetCurrentProcessId()));
	ULONG openedPid = FindPidInTree(HandleToLong(PsGetProcessId(OpenedProcess)));
	//Do they match?
	if (currentPid == openedPid)
		//Self access (possible cases: parent -> parent, parent -> child, child -> parent, child -> child)
		goto Exit;
}
//Otherwise we have an unallowed access!
//Remove permission to modify (write into) virtual memory
InPreInfo->Parameters->CreateHandleInformation.DesiredAccess &= ~(PROCESS_VM_WRITE);