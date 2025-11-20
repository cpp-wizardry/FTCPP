#pragma once
//stdafx.h
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <thread>
#include <chrono>

#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 1024
#define FTP_CONTROL_PORT "21"