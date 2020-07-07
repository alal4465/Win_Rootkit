#pragma once
#include <iostream>
#include <vector>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")


/*
the definition of "WIN32_LEAN_AND_MEAN" excludes some driver macros.
this is a copy of them.
*/
#include "d4drvif.h"
std::string cmd_exec(const char* cmd);
bool logError(const char* message);