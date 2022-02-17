#include <WinSock2.h>
#include <stdio.h>

int main() {

    WSADATA WinSockData;

    char SenderBuffer[512] = "Hello from server";
    int iSenderBuffer = strlen(SenderBuffer)+1;

    char RecvBuffer[512];

    // Step 1: Startup
    int iWsaStartup = WSAStartup(MAKEWORD(2, 2), &WinSockData);
    if (iWsaStartup != 0) {
        printf("Problem with startup!");
        return 0;
    }

    // Step 2: Define the client socket
    SOCKET TCPClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (TCPClientSocket == INVALID_SOCKET) {
        printf("Step 2: Invalid socket");
        return 0;
    }

    // Step 3: Fill server struct
    struct sockaddr_in TCPServerAdd;
    TCPServerAdd.sin_family = AF_INET;
    TCPServerAdd.sin_addr.s_addr = inet_addr("127.0.0.1");
    TCPServerAdd.sin_port = htons(8000);

    // Step 4: Connect
    int iConnect = connect(TCPClientSocket, (SOCKADDR*)&TCPServerAdd, sizeof(TCPServerAdd));
    if (iConnect == INVALID_SOCKET) {
        printf("Problem with connecting to server socket\n");
        return 0;
    }

    // Step 5: Receive data
    if (recv(TCPClientSocket, RecvBuffer, sizeof(RecvBuffer) / sizeof(RecvBuffer[0]), 0) == SOCKET_ERROR) {
        printf("Problem receiving message");
        return 0;
    }
    printf(RecvBuffer);

    // Step 5b: Send data
    if (send(TCPClientSocket, SenderBuffer, sizeof(SenderBuffer) / sizeof(SenderBuffer[0]), 0) == SOCKET_ERROR) {
        printf("Problem Sending message");
        return 0;
    }

    // Step 6: Close socket
    if (closesocket(TCPClientSocket) == SOCKET_ERROR) {
        printf("Problem closing socket");
        return 0;
    }

    // Step 7: Cleanup
    if (WSACleanup() == SOCKET_ERROR) {
        printf("Problem cleaning!");
    }
    return 0;
}
