#include "function.h"



std::string ftp_command(SOCKET sock, const std::string& command) {
    send(sock, command.c_str(), static_cast<int>(command.length()), 0);

    char buffer[BUFFER_SIZE] = { 0 };
    int bytesReceived = recv(sock, buffer, BUFFER_SIZE - 1, 0);
    if (bytesReceived <= 0) {
        return "Error receiving response";
    }

    return std::string(buffer, bytesReceived);
}

bool isValidFTP(const std::string& url) {
    std::regex ftpRegex(R"(^(ftp:\/\/)([a-zA-Z0-9._-]+(:[a-zA-Z0-9._-]+)?@)?([a-zA-Z0-9.-]+\.[a-zA-Z]{2,})(:[0-9]+)?(\/.*)?$)");
    return std::regex_match(url, ftpRegex);
}


bool parse_pasv_response(const std::string& response, std::string& ip, int& port) {
    std::regex pasv_regex(R"(227 Entering Passive Mode \((\d+),(\d+),(\d+),(\d+),(\d+),(\d+)\))");
    std::smatch matches;

    if (std::regex_search(response, matches, pasv_regex) && matches.size() == 7) {

        ip = matches[1].str() + "." + matches[2].str() + "." + matches[3].str() + "." + matches[4].str();


        int port_high = std::stoi(matches[5].str());
        int port_low = std::stoi(matches[6].str());
        port = (port_high * 256) + port_low;

        return true;
    }

    return false;
}