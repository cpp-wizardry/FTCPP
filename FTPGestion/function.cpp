#include "stdafx.h"
#include "function.h"
//function.cpp
std::string ftp_command(SOCKET sock, const std::string& command) {
    send(sock, command.c_str(), static_cast<int>(command.length()), 0);

    std::string response;
    char buffer[BUFFER_SIZE] = { 0 };

    while (true) {
        int bytesReceived = recv(sock, buffer, BUFFER_SIZE - 1, 0);
        if (bytesReceived <= 0) break;
        buffer[bytesReceived] = '\0';
        response += buffer;
        if (bytesReceived < BUFFER_SIZE - 1) break;
    }

    return response;
}

bool parse_pasv_response(const std::string& response, std::string& ip, int& port) {
    std::regex pasv_regex(R"(227 Entering Passive Mode \((\d+),(\d+),(\d+),(\d+),(\d+),(\d+)\))");
    std::smatch matches;

    if (std::regex_search(response, matches, pasv_regex)) {
        ip = matches[1].str() + "." + matches[2].str() + "." + matches[3].str() + "." + matches[4].str();
        port = (std::stoi(matches[5].str()) * 256 + std::stoi(matches[6].str()));
        return true;
    }
    return false;
}

bool create_data_connection(const std::string& ip, int port, SOCKET& data_sock) {
    data_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (data_sock == INVALID_SOCKET) {
        std::cerr << "Data socket creation failed: " << WSAGetLastError() << "\n";
        return false;
    }

    sockaddr_in data_addr = {};
    data_addr.sin_family = AF_INET;
    data_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &data_addr.sin_addr);

    if (connect(data_sock, (sockaddr*)&data_addr, sizeof(data_addr)) == SOCKET_ERROR) {
        std::cerr << "Data connection failed: " << WSAGetLastError() << "\n";
        closesocket(data_sock);
        return false;
    }
    return true;
}

bool list_files(SOCKET control_sock, SOCKET data_sock) {
    std::string response = ftp_command(control_sock, "LIST\r\n");
    if (response.find("150") == std::string::npos) {
        std::cerr << "LIST command failed: " << response << "\n";
        return false;
    }

    char buffer[BUFFER_SIZE];
    int bytes_received;
    while ((bytes_received = recv(data_sock, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        buffer[bytes_received] = '\0';
        std::cout << buffer;
    }
    std::cout << "\nFile listing complete.\n";
    return true;
}

bool download_file(SOCKET control_sock, SOCKET data_sock, const std::string& filename) {
    std::string response = ftp_command(control_sock, "RETR " + filename + "\r\n");
    if (response.find("150") == std::string::npos) {
        std::cerr << "RETR command failed: " << response << "\n";
        return false;
    }

    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing\n";
        return false;
    }

    char buffer[BUFFER_SIZE];
    int bytes_received;
    while ((bytes_received = recv(data_sock, buffer, BUFFER_SIZE, 0)) > 0) {
        file.write(buffer, bytes_received);
    }

    file.close();
    return true;
}