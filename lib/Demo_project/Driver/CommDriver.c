

#include <ntddk.h>
#include "CommDriver.h"
#include "..\include\IoControl.h"


PVOID	gpEventObject = NULL;

/*
Response to CreateFile
*/
NTSTATUS CommDriver_Create(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    )
{
	NTSTATUS status = STATUS_SUCCESS;

	DbgPrint("CommDriver_Create\n");

	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return status;
}

/*
Response to CloseHandle
*/
NTSTATUS CommDriver_Close(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    )
{
	NTSTATUS status = STATUS_SUCCESS;

	DbgPrint("CommDriver_Close\n");

	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return status;
}

/*
Resonse to DeviceIoControl
*/
NTSTATUS CommDriver_IoControl(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    )
{
	NTSTATUS					status = STATUS_SUCCESS;
	ULONG						controlCode;
	PIO_STACK_LOCATION			irpStack;
	HANDLE						hEvent;
	OBJECT_HANDLE_INFORMATION	objHandleInfo;
	LONG*						outBuf;

	irpStack = IoGetCurrentIrpStackLocation(Irp);
	controlCode = irpStack->Parameters.DeviceIoControl.IoControlCode;
	
	switch(controlCode)
	{
	case IO_REFERENCE_EVENT:
		hEvent = (HANDLE) irpStack->Parameters.DeviceIoControl.Type3InputBuffer;
		status = ObReferenceObjectByHandle(
				hEvent,
				GENERIC_ALL,
				NULL,
				KernelMode,
				&gpEventObject,
				&objHandleInfo);
		if(status != STATUS_SUCCESS)
		{
			DbgPrint("ObReferenceObjectByHandle failed! status = %x\n", status);
			break;
		}
		DbgPrint("Referenct object sussfully!\n");
		break;
		
	case IO_DEREFERENCE_EVENT:
		if(gpEventObject)
			ObDereferenceObject(gpEventObject);
		DbgPrint("Dereferenct object sussfully!\n");
		break;

	case IO_SET_EVENT:
		KeSetEvent(gpEventObject,
			0,
			FALSE);
		DbgPrint("KeSetEvent sussfully!\n");
		break;
		
	case IO_CLEAR_EVENT:
		KeClearEvent(gpEventObject);
		DbgPrint("KeClearEvent sussfully!\n");
		break;
	
	case IO_QUERY_EVENT_STATE:
		DbgPrint("in KeReadStateEvent !\n");
		outBuf = (LONG*) Irp->UserBuffer;
		*outBuf = KeReadStateEvent(gpEventObject);
		DbgPrint("KeReadStateEvent sussfully!\n");
		
		Irp->IoStatus.Status = STATUS_SUCCESS;
		Irp->IoStatus.Information = sizeof(LONG);
		IoCompleteRequest(Irp, IO_NO_INCREMENT);
		return status;

	default:
		break;
	}

	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return status;
}


void CommDriver_Unload(
    IN PDRIVER_OBJECT DriverObject
    )
{
	UNICODE_STRING	SymbolicName;

	RtlInitUnicodeString(&SymbolicName, COMM_DRIVER_WIN32_DEV_NAME);
	IoDeleteSymbolicLink(&SymbolicName);

	IoDeleteDevice(DriverObject->DeviceObject);
}

NTSTATUS DriverEntry( 
    IN PDRIVER_OBJECT DriverObject, 
    IN PUNICODE_STRING RegistryPath 
    )
{
	NTSTATUS		status = STATUS_SUCCESS;
	UNICODE_STRING	DeviceName;
	UNICODE_STRING	SymbolicName;

	/* Driver unload routine */
	DriverObject->DriverUnload = CommDriver_Unload;

	/* Initialize major functions */
	DriverObject->MajorFunction[IRP_MJ_CREATE] = CommDriver_Create;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = CommDriver_Close;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = CommDriver_IoControl;

	/* Initizlize device name and symbolic name */
	RtlInitUnicodeString(&DeviceName, COMM_DRIVER_DEV_NAME);
	RtlInitUnicodeString(&SymbolicName, COMM_DRIVER_WIN32_DEV_NAME);

	/*
	Create a communication object, 
	GUI application can open the device and communicate with this kernel module.
	*/
	status = IoCreateDevice(
			DriverObject,
			sizeof(COMM_DRIVER_EXT),
			&DeviceName,
			FILE_DEVICE_UNKNOWN,
			0,
			TRUE,
			&DriverObject->DeviceObject
			);
	if(status != STATUS_SUCCESS)
	{
		DbgPrint("IoCreateDevice failed\n");
		return status;
	}
	
	/* Create symbilic link */
	status = IoCreateSymbolicLink(&SymbolicName, &DeviceName);
	if(status != STATUS_SUCCESS)
	{
		DbgPrint("IoCreateSymbolicLink failed\n");
		return status;
	}
	
	DbgPrint("Exit DriverEntry\n");

	return  status;
}