#pragma once
#include <Ntifs.h>

namespace IoctlHandlers {

	NTSTATUS HandleHideProcess(PIRP Irp);

	NTSTATUS HandleTestConnection(PIRP Irp,ULONG bufferSize);

	NTSTATUS HandleElevate(PIRP Irp);
}