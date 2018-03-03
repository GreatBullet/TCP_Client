#pragma warning( disable : 4326 )
#include <iostream>
#include <string>
#include <WS2tcpip.h>
#pragma comment (lib,"ws2_32.lib")
void main() {
	std::string ipAddress = "127.0.0.1";
	int port = 54000;
	// Initialize WinSock
	WSADATA WinSockData;
	WORD Version = MAKEWORD(2, 2);
	int WinSockResult = WSAStartup(Version, &WinSockData);
	if (WinSockResult != 0){
		std::cerr << "Can't Initialize winsock! Quitting\n";
		return;
	}
	// Create Socket
	SOCKET Socket = socket(AF_INET, SOCK_STREAM, 0);
	if (Socket == INVALID_SOCKET){
		std::cerr << "Can't create a socket! Quitting\n";
		return;
	}
	// Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);
	// Connect to server
	int ConnectionResult = connect(Socket, (sockaddr*)&hint, sizeof(hint));
	if (ConnectionResult == SOCKET_ERROR){
		std::cerr << "Can't connect to server, Err #" << WSAGetLastError() << '\n';
		closesocket(Socket);
		WSACleanup();
		return;
	}
	// Do-While loop to send and receive data
	char buf[4096];
	std::string userInput;
	do 
	{
		// Promt the user for some text
		std::cout << "> ";
		std::getline(std::cin, userInput);
		if (userInput.size() > 0){ 
			// Send the text
			int sendResult = send(Socket, userInput.c_str(), userInput.size() + 1, 0);
			if (sendResult != SOCKET_ERROR) {
				// Wait for response
				ZeroMemory(buf, 4096);
				int bytesReceived = recv(Socket, buf, 4094, 0);
				if (bytesReceived > 0)
				{
				// Echo response to console
					std::cout << "Server> " << std::string(buf, 0, bytesReceived) << '\n';
				}
			}
		}
	} while (userInput.size() > 0);
	// Close down everything
	closesocket(Socket);
	WSACleanup();
}