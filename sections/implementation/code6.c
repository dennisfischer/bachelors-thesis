//This is the patch routine which will break the DLL file
void PatchRoutine(PVOID Parameter)
{
	//...
	//To access the dll in memory, we have to reach into the process virtual memory
	PRKAPC_STATE apcState = AllocMemory(TRUE, sizeof(KAPC_STATE));
	PEPROCESS pEProcess;
	if (!NT_SUCCESS(Status = PsLookupProcessByProcessId(WorkItem->ProcessId, &pEProcess)))
	{
		DEBUG("Process Lookup failed: %d\n", Status);
		goto Exit;
	}
	//We attach to it here
	KeStackAttachProcess(pEProcess, apcState);

	//First of all Write protection is disabled
#ifdef X64_DRIVER
	KIRQL CurrentIRQL = RtlWPOff();
#endif
	//The IRQL needs to be lowered to <=APC_LEVEL, so that PAGE faults don't lead to a BugCheck
	KeLowerIrql(APC_LEVEL);
	//Get the entryPoint of the dll
	uint8* entryPointAbs = ReadPE(WorkItem->ImageBase);

	//Patch the entry point function by writing a ret instruction
	uint8 patch[] = { 0xC3 }; //ret
	CopyMemory(entryPointAbs, patch, sizeof(patch));

	//Undo KeLowerIRQL to previous IRQL level from RtlWpOff
	KeRaiseIrqlToDpcLevel();
	//And enabled again
#ifdef X64_DRIVER
	RtlWPOn(CurrentIRQL);
#endif

	//And after patching is done, we detach from it
	KeUnstackDetachProcess(apcState);
	//...
}