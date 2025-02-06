#include "stdafx.h"
#include "function.h"
#include <thread>
#include <chrono>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

    SOCKET control_sock = INVALID_SOCKET;
    char buffer[BUFFER_SIZE] = { 0 };
    std::string username, password, fileName;
    const char* constServerName = "ftp.dlptest.com";

    struct addrinfo hints = {}, * res;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if (getaddrinfo(constServerName, FTP_SOCKET, &hints, &res) != 0) {
        std::cerr << "Failed to resolve FTP server address\n";
        WSACleanup();
        return 1;
    }

    control_sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (control_sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << "\n";
        freeaddrinfo(res);
        WSACleanup();
        return 1;
    }

    if (connect(control_sock, res->ai_addr, static_cast<int>(res->ai_addrlen)) == SOCKET_ERROR) {
        std::cerr << "Connection failed: " << WSAGetLastError() << "\n";
        closesocket(control_sock);
        freeaddrinfo(res);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(res);

    // Receive welcome message
    std::string response = ftp_command(control_sock, "");
    std::cout << response << std::endl;

    // Log in
    std::cout << "Enter username: ";
    std::cin >> username;
    response = ftp_command(control_sock, "USER " + username + "\r\n");
    std::cout << response;

    std::cout << "Enter password: ";
    std::cin >> password;
    response = ftp_command(control_sock, "PASS " + password + "\r\n");
    std::cout << response;

    // Enter Passive Mode
    response = ftp_command(control_sock, "PASV\r\n");
    std::cout << response;

    // Parse PASV response
    std::string data_ip;
    int data_port;
    if (!parse_pasv_response(response, data_ip, data_port)) {
        std::cerr << "Failed to parse PASV response\n";
        closesocket(control_sock);
        WSACleanup();
        return 1;
    }

    std::cout << "Data connection: " << data_ip << ":" << data_port << std::endl;

    // Establish data connection
    SOCKET data_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (data_sock == INVALID_SOCKET) {
        std::cerr << "Data socket creation failed: " << WSAGetLastError() << "\n";
        closesocket(control_sock);
        WSACleanup();
        return 1;
    }

    sockaddr_in data_addr = {};
    data_addr.sin_family = AF_INET;
    data_addr.sin_port = htons(data_port);
    inet_pton(AF_INET, data_ip.c_str(), &data_addr.sin_addr);

    if (connect(data_sock, (sockaddr*)&data_addr, sizeof(data_addr)) == SOCKET_ERROR) {
        std::cerr << "Data connection failed: " << WSAGetLastError() << "\n";
        closesocket(data_sock);
        closesocket(control_sock);
        WSACleanup();
        return 1;
    }

    // Download file
    std::cout << "Enter file name to download: ";
    std::cin >> fileName;
    response = ftp_command(control_sock, "RETR " + fileName + "\r\n");
    std::cout << response;

    // Receive file data
    char file_buffer[BUFFER_SIZE];
    std::ofstream file(fileName, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing\n";
        closesocket(data_sock);
        closesocket(control_sock);
        WSACleanup();
        return 1;
    }

    int bytes_received;
    while ((bytes_received = recv(data_sock, file_buffer, BUFFER_SIZE, 0)) > 0) {
        file.write(file_buffer, bytes_received);
    }

    file.close();
    closesocket(data_sock);

    // Quit
    response = ftp_command(control_sock, "QUIT\r\n");
    std::cout << response;

    closesocket(control_sock);
    WSACleanup();

    return 0;
}