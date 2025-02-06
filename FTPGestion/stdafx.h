#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <WinSock2.h>
#include <sys/types.h>
#include <fstream>
#include <WS2tcpip.h>
#include <regex>
#include <iostream>
#include <string>
#define PORT 3006 /* Port client*/
#define FTP_SOCKET "21" /* Socket dedier à la transmission de commande entre client et serveur ftp*/
#define FTP_SOCKET_DATA "20" /* Socket dedier à la transmission de fichier entre client et serveur ftp*/
#define BUFFER_SIZE 1024 /*Taille du buffer*/