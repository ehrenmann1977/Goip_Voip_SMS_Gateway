#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define SERVER_IP "10.147.20.3"
#define PORT 44444
#define BUFFER_SIZE 1024

void sendKeepAliveAck(int serverSocket, const char* data, struct sockaddr_in* serverAddress) {
    printf("Sending Keepalive Acknowledgment: %s\n", data);
    // Print the data being sent (debug)
    printf("Sending data: %s\n", data);

    ssize_t bytesSent = sendto(serverSocket, data, strlen(data), 0, (struct sockaddr*)serverAddress, sizeof(*serverAddress));
    if (bytesSent == -1) {
        perror("Send failed");
        exit(EXIT_FAILURE);
    }
}



void analyzeReceivedData(const char* data, int serverSocket, struct sockaddr_in* serverAddress) {
    if (strncmp(data, "RECEIVE", 7) == 0) {
        // SMS received data
        printf("SMS Received: %s\n", data);
        
        // Extract the relevant fields
        const char* delimiter = ";";
        const char* keyValDelimiter = ":";
        char* token;
        char* context;
        
        char receive[20] = "";
        char id[20] = "";
        char password[20] = "";
        char srcnum[20] = "";
        char msg[256] = "";
        
        // Parse the received data
        token = strtok_r((char*)data, delimiter, &context);
        while (token != NULL) {
            char* key = strtok(token, keyValDelimiter);
            char* value = strtok(NULL, keyValDelimiter);
            
            if (key != NULL && value != NULL) {
                if (strcmp(key, "RECEIVE") == 0) {
                    strncpy(receive, value, sizeof(receive) - 1);
                } else if (strcmp(key, "id") == 0) {
                    strncpy(id, value, sizeof(id) - 1);
                } else if (strcmp(key, "password") == 0) {
                    strncpy(password, value, sizeof(password) - 1);
                } else if (strcmp(key, "srcnum") == 0) {
                    strncpy(srcnum, value, sizeof(srcnum) - 1);
                } else if (strcmp(key, "msg") == 0) {
                    strncpy(msg, value, sizeof(msg) - 1);
                }
            }
            
            token = strtok_r(NULL, delimiter, &context);
        }
        
        // Print the extracted field values
        printf("RECEIVE: %s\n", receive);
        printf("id: %s\n", id);
        printf("password: %s\n", password);
        printf("srcnum: %s\n", srcnum);
        printf("msg: %s\n\n\n\n\n", msg);

        if (strcmp(id, "2002") == 0 && strcmp(password, "2002") == 0) {
            // Authentication success
            char ackResponse[512];
            snprintf(ackResponse, sizeof(ackResponse), "RECEIVE %s OK\n", receive);
            sendKeepAliveAck(serverSocket, ackResponse, serverAddress);
            printf("Receive Acknowledge = %s\n",ackResponse);
        }




    } else {
        // Keepalive packet received
        char req[20] = "";
        char id[20] = "";
        char password[20] = "";
        char imei[20] = "";
        char imsi[20] = "";
        char iccid[20] = "";
        char pro[20] = "";
        char cellInfo[50] = "";
        
        // Parse the received data
        const char* delimiter = ";";
        const char* keyValDelimiter = ":";
        char* token;
        char* context;
        
        token = strtok_r((char*)data, delimiter, &context);
        while (token != NULL) {
            char* key = strtok(token, keyValDelimiter);
            char* value = strtok(NULL, keyValDelimiter);
            
            if (key != NULL && value != NULL) {
                if (strcmp(key, "req") == 0) {
                    strncpy(req, value, sizeof(req) - 1);
                } else if (strcmp(key, "id") == 0) {
                    strncpy(id, value, sizeof(id) - 1);
                } else if (strcmp(key, "pass") == 0) {
                    strncpy(password, value, sizeof(password) - 1);
                } else if (strcmp(key, "imei") == 0) {
                    strncpy(imei, value, sizeof(imei) - 1);
                } else if (strcmp(key, "imsi") == 0) {
                    strncpy(imsi, value, sizeof(imsi) - 1);
                } else if (strcmp(key, "iccid") == 0) {
                    strncpy(iccid, value, sizeof(iccid) - 1);
                } else if (strcmp(key, "pro") == 0) {
                    strncpy(pro, value, sizeof(pro) - 1);
                } else if (strcmp(key, "CELLINFO") == 0) {
                    strncpy(cellInfo, value, sizeof(cellInfo) - 1);
                }
            }
            
            token = strtok_r(NULL, delimiter, &context);
        }
        
        // Print the extracted field values
        printf("req: %s\n", req);
        printf("id: %s\n", id);
        printf("password: %s\n", password);
        printf("imei: %s\n", imei);
        printf("imsi: %s\n", imsi);
        printf("iccid: %s\n", iccid);
        printf("pro: %s\n", pro);
        printf("CELLINFO: %s\n\n\n\n\n", cellInfo);
        
        // Perform authentication check
        if (strcmp(id, "2002") == 0 && strcmp(password, "2002") == 0) {
            printf("password correct\n");
            // Authentication success
            int status = 200;
            
            // Prepare the keepalive acknowledgment
            char keepAliveAck[100];
            snprintf(keepAliveAck, sizeof(keepAliveAck), "reg:%s;status:%d;", req, status);
            
            // Send the keepalive acknowledgment
            sendKeepAliveAck(serverSocket, keepAliveAck, serverAddress);
        }
    }
}


int main() {
    int serverSocket;
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t clientLength;
    char buffer[BUFFER_SIZE];

    // Create a socket
    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up server address
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    // Bind the socket to the specified port
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Socket bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Listening on port %d...\n", PORT);

    // Receive and print data
    ssize_t bytesRead;
    clientLength = sizeof(clientAddress);
    while ((bytesRead = recvfrom(serverSocket, buffer, BUFFER_SIZE - 1, 0,
                                 (struct sockaddr *)&clientAddress, &clientLength)) > 0) {
        buffer[bytesRead] = '\0';
        printf("Received: %s\n", buffer);
        analyzeReceivedData(buffer, serverSocket, &clientAddress);
    }

    if (bytesRead == -1) {
        perror("Receive failed");
    }

    // Close the socket
    close(serverSocket);

    return 0;
}

