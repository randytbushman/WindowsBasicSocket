#include <winsock2.h>
#include <stdio.h>
#include <windows.h>

int main() {
    WSADATA WinSockData;

    // Declare buffers
    char RecvBuffer[1024];
    char SenderBuffer[512] = "HTTP/1.1 200 OK\r\n"
                         "Content-Type: text/html\r\n"
                         "\r\n"
                         "Hello World! This is the simplest webserver. It is created by Randy.\n";

    // Step 1: Startup
    if (WSAStartup(MAKEWORD(2, 2), &WinSockData) != 0) printf("Problem with startup!\n");

    // Step 2: Fill struct
    struct sockaddr_in TCPServerAdd;
    TCPServerAdd.sin_family = AF_INET;
    TCPServerAdd.sin_addr.s_addr = inet_addr("127.0.0.1");
    TCPServerAdd.sin_port = htons(8000);

    // Step 3: Create socket
    SOCKET TCPServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (TCPServerSocket == INVALID_SOCKET)
        printf("Invalid socket at socket creation\n");

    // Step 4: Bind socket
    if (bind(TCPServerSocket, (SOCKADDR*)&TCPServerAdd, sizeof(TCPServerAdd)) != 0)
        printf("Cannot bind!\n");

    // Step 5: Begin listening
    if (listen(TCPServerSocket, 500) != 0) printf("Cannot listen\n");

    // Step 6: Begin accepting
    struct sockaddr_in TCPClientAdd;
    int iTCPClientAdd = sizeof(TCPClientAdd);
    SOCKET sAcceptSocket = accept(TCPServerSocket, (SOCKADDR*)&TCPClientAdd, &iTCPClientAdd);
    if (sAcceptSocket == INVALID_SOCKET) printf("Accepted an invalid socket\n");

    // Step 7: Receive request and do work on it
    if (recv(sAcceptSocket, RecvBuffer, 512, 0) == SOCKET_ERROR)
        printf("Error receiving from socket\n");
    printf(RecvBuffer);

    // Step 8: Send Response
    if (send(sAcceptSocket, SenderBuffer, sizeof(SenderBuffer) / sizeof(SenderBuffer[0]), 0) == SOCKET_ERROR)
        printf("Error sending response from socket\n");
    Sleep(2000);     // Sleep so response can transmit before server closes socket

    // Step 9: Close Socket
    if (closesocket(TCPServerSocket) == SOCKET_ERROR) printf("Error closing socket!\n");

    // Step 10: Cleanup
    if (WSACleanup() == SOCKET_ERROR) printf("Error cleaning up!\n");

    return 0;
}
