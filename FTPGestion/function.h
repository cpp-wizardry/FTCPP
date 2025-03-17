#pragma once
#include "stdafx.h"
#include <string>

std::string ftp_command(SOCKET sock, const std::string& command);

//bool isValidFTP(const std::string& url);
bool parse_pasv_response(const std::string& response, std::string& ip, int& port);

bool setup_data_connection(SOCKET control_sock, SOCKET& data_sock, std::string& data_ip, int& data_port);