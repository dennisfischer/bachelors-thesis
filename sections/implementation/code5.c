//This is the hash routine which generates the sha256 hash
void HashRoutine(PVOID Parameter)
{
	//We will need to get a fileHandle
	HANDLE fileHandle = NULL;
	uint8* fileData = NULL;
	NTSTATUS Status;

	//...
	//A file handle is obtained, sometimes additional path building is done
	//...

	Checksum:
		//Get the file size
		LARGE_INTEGER file_size;
		if (!NT_SUCCESS(Status = GetFileSize(WorkItem->FileObject, &file_size)))
		{
			goto Fail;
		}

		//Allocate data buffer and read file into buffer
		fileData = AllocMemory(TRUE, file_size.LowPart);
		if (!NT_SUCCESS(Status = ReadFile(fileHandle, file_size.LowPart, fileData)))
		{
			goto Fail;
		}

		//Finally hash the data contained inside fileData
		WorkItem->Result = calc_sha256(fileData, file_size.LowPart);
	//...
}