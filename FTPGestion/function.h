#pragma once
#include "stdafx.h"
#include <string>

std::string ftp_command(SOCKET sock, const std::string& command);

bool isValidFTP(const std::string& url);
bool parse_pasv_response(const std::string& response, std::string& ip, int& port);

