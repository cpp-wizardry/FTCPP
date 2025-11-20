#include "stdafx.h"
#include "function.h"
//main.cpp
int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

    SOCKET control_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (control_sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << "\n";
        WSACleanup();
        return 1;
    }

    const char* server_name = "ftp.dlptest.com";
    sockaddr_in server_addr = {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(21);
    inet_pton(AF_INET, server_name, &server_addr.sin_addr);

    if (connect(control_sock, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed: " << WSAGetLastError() << "\n";
        closesocket(control_sock);
        WSACleanup();
        return 1;
    }


    std::string response = ftp_command(control_sock, "");
    std::cout << response << std::endl;

    // Login
    std::string username = "dlpuser";
    std::string password = "rNrKYTX9g7z3RgJRmxWuGHbeu";

    response = ftp_command(control_sock, "USER " + username + "\r\n");
    std::cout << response << "\n";

    response = ftp_command(control_sock, "PASS " + password + "\r\n");
    std::cout << response << "\n";

    // List files
    response = ftp_command(control_sock, "PASV\r\n");
    std::cout << response << "\n";

    std::string data_ip;
    int data_port;
    if (!parse_pasv_response(response, data_ip, data_port)) {
        std::cerr << "Failed to parse PASV response\n";
        closesocket(control_sock);
        WSACleanup();
        return 1;
    }

    SOCKET data_sock;
    if (!create_data_connection(data_ip, data_port, data_sock)) {
        closesocket(control_sock);
        WSACleanup();
        return 1;
    }

    if (!list_files(control_sock, data_sock)) {
        closesocket(data_sock);
        closesocket(control_sock);
        WSACleanup();
        return 1;
    }
    closesocket(data_sock);

    // Download file
    std::string filename;
    std::cout << "Enter file name to download: ";
    std::cin >> filename;

    response = ftp_command(control_sock, "PASV\r\n");
    std::cout << response << "\n";

    if (!parse_pasv_response(response, data_ip, data_port)) {
        std::cerr << "Failed to parse PASV response\n";
        closesocket(control_sock);
        WSACleanup();
        return 1;
    }

    if (!create_data_connection(data_ip, data_port, data_sock)) {
        closesocket(control_sock);
        WSACleanup();
        return 1;
    }

    if (!download_file(control_sock, data_sock, filename)) {
        closesocket(data_sock);
        closesocket(control_sock);
        WSACleanup();
        return 1;
    }
    closesocket(data_sock);


    response = ftp_command(control_sock, "QUIT\r\n");
    std::cout << response << "\n";

    closesocket(control_sock);
    WSACleanup();
    return 0;
}