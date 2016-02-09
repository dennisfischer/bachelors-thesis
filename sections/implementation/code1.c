struct _PROCESS_LIST_ENTRY {
	LIST_ENTRY ListEntry;
	ULONG Pid;
	PLIST_ENTRY ChildHead;
};

struct _PROCESS_LIST_ENTRY_CHILD {
	LIST_ENTRY ListEntry;
	ULONG Pid;
	ULONG ParentPid;
};