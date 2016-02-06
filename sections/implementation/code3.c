//Finds PID inside the tree
ULONG FindPidInTree(ULONG InPid)
{
	KLOCK_QUEUE_HANDLE SpinLockHandle;
	KeAcquireInStackQueuedSpinLock(PTreeSpinLock, &SpinLockHandle);

	//First, quickly check parents
	PLIST_ENTRY entry = PListHead->Flink;
	while (PListHead != entry->Flink)
	{
		if (CONTAINING_RECORD(entry, PROCESS_LIST_ENTRY, ListEntry)->Pid == InPid)
		{
			KeReleaseInStackQueuedSpinLock(&SpinLockHandle);
			return InPid;
		}
		entry = entry->Flink;
	}

	//If not found, check children of all parents
	entry = PListHead->Flink;
	while (PListHead != entry->Flink)
	{
		PLIST_ENTRY childHead = CONTAINING_RECORD(entry, PROCESS_LIST_ENTRY, ListEntry)->ChildHead;
		PLIST_ENTRY child = childHead;
		while (childHead != child->Flink)
		{
			child = child->Flink;
			if (CONTAINING_RECORD(child, PROCESS_LIST_ENTRY_CHILD, ListEntry)->Pid == InPid)
			{
				DEBUG("Exit Find Pid PTree\n");
				KeReleaseInStackQueuedSpinLock(&SpinLockHandle);
				return CONTAINING_RECORD(child, PROCESS_LIST_ENTRY_CHILD, ListEntry)->ParentPid;
			}
		}
		entry = entry->Flink;
	}

	KeReleaseInStackQueuedSpinLock(&SpinLockHandle);
	return 0;
}