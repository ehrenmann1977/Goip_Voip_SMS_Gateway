#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "10.147.20.3"
#define PORT 44444

void sendUDPData(const char* data, const char* ipAddress, int port) {
    int clientSocket;
    struct sockaddr_in serverAddress;

    // Create a socket
    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up server address
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(ipAddress);
    serverAddress.sin_port = htons(port);

    // Send data to the server
    ssize_t bytesSent = sendto(clientSocket, data, strlen(data), 0, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    if (bytesSent == -1) {
        perror("Send failed");
        exit(EXIT_FAILURE);
    }

    // Close the socket
    close(clientSocket);
}

int main() {
    // Example: Send an acknowledgment SMS
    printf("Sending SMS Acknowledgment...\n");

    // Prepare the acknowledgment response
    char response[512];
    snprintf(response, sizeof(response), "MSG id 5 hello\n");

    // Send the acknowledgment response
    sendUDPData(response, SERVER_IP, PORT);

    return 0;
}
