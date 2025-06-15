﻿#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

#include<Windows.h>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<iphlpapi.h>
#include<stdio.h>
#include<iostream>
#include<FormatLastError.h>
using namespace std;

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "FormatLastError.lib")

#define DEFAULT_PORT	"27015"


void main()
{
	setlocale(LC_ALL, "Russian");
	cout << "WinSock Client" << endl;
	//1) Инициализация WinSock:
	WSADATA wsaData;
	INT iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult)
	{
		cout << "WSAStartup() failed with code " << iResult << endl;
		return;
	}

	//2) Создаем ClientSocket:
	addrinfo* result = NULL;
	addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;	//INET - TCP/IPv4
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	//3) Определяем IP-адрес Сервера:
	iResult = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result);
	if (iResult)
	{
		cout << "getaddressinfo() failed with code " << iResult << endl;
		WSACleanup();
		return;
	}
	//cout << "hints:" << endl;
	//cout << "ai_addr:" << hints.ai_addr->sa_data << endl;

	SOCKET connect_socket = socket(hints.ai_family, hints.ai_socktype, hints.ai_protocol);
	if (connect_socket == INVALID_SOCKET)
	{
		DWORD dwMessageID = WSAGetLastError();
		//cout << "Error: soket creatin failed with code: " << dwMessageID << ":\t";
		LPSTR szBuffer = FormatLastError(dwMessageID);
		
		freeaddrinfo(result);
		WSACleanup();
		return;
	}

	//4) Подключаемся к Серверу:
	iResult = connect(connect_socket, result->ai_addr, result->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		/*DWORD dwMessageID = WSAGetLastError();
		cout << "Error: Connect to Server failed with code: " << WSAGetLastError() << endl;
		LPSTR szBuffer = NULL;
		FormatMessage
		(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dwMessageID,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_RUSSIAN_RUSSIA),
			(LPSTR)&szBuffer,
			0,
			NULL
		);
		cout << szBuffer << endl;
		LocalFree(szBuffer);*/
		/*DWORD dwMessageID = WSAGetLastError();
		LPSTR szMessage = FormatLastError(dwMessageID);
		printf("Error %i:%s", dwMessageID, szMessage);
		LocalFree(szMessage);*/
		PrintLastError(WSAGetLastError());
		closesocket(connect_socket);
		freeaddrinfo(result);
		WSACleanup();
	}

	//?) Освобождаем ресурсы WinSock:
	WSACleanup();
}