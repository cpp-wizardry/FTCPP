#pragma once
#include "stdafx.h"

//function.h
std::string ftp_command(SOCKET sock, const std::string& command);
bool parse_pasv_response(const std::string& response, std::string& ip, int& port);
bool create_data_connection(const std::string& ip, int port, SOCKET& data_sock);
bool download_file(SOCKET control_sock, SOCKET data_sock, const std::string& filename);
bool list_files(SOCKET control_sock, SOCKET data_sock);