#pragma once

constexpr int ROOTKIT_DEVICE = 0x8000;

enum class RookitIoctls {
	HideProcces = CTL_CODE(ROOTKIT_DEVICE, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS),
	TestConnection = CTL_CODE(ROOTKIT_DEVICE, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)
};
