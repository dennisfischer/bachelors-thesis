//Inserts a new parent pid
VOID InsertPidToTree(ULONG InPid) {
	//Check if tree already contains parent and return
	PLIST_ENTRY entry = PListHead->Flink;
	while (PListHead != entry->Flink) {
		if (CONTAINING_RECORD(entry, PROCESS_LIST_ENTRY, ListEntry)->Pid == InPid)
			return;
		entry = entry->Flink;
	}
	//Otherwise add new parent list entry
	PPROCESS_LIST_ENTRY newEntry = AllocMemory(TRUE, sizeof(PROCESS_LIST_ENTRY));
	newEntry->Pid = InPid;
	newEntry->ChildHead = AllocMemory(TRUE, sizeof(LIST_ENTRY));
	InitializeListHead(newEntry->ChildHead);
	InsertHeadList(PListHead, &(newEntry->ListEntry));
}