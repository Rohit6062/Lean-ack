
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
// #include "raptor_header/base.h"
// Function to receive data with a timeout
ssize_t recv_with_timeout(int sockfd, void *buffer, size_t len, int timeout_secs) {
    fd_set read_fds;
    struct timeval timeout;

    // Set up the timeout
    timeout.tv_sec = timeout_secs;
    timeout.tv_usec = 0;

    // Initialize the file descriptor set
    FD_ZERO(&read_fds);
    FD_SET(sockfd, &read_fds);

    // Wait for the socket to become readable or timeout
    int retval = select(sockfd + 1, &read_fds, NULL, NULL, &timeout);
    if (retval == -1) {
        perror("select error");
        return -1;  // Error occurred
    } else if (retval == 0) {
        printf("Timeout occurred! No data received in %d seconds.\n", timeout_secs);
        return -1;  // Timeout occurredṇ
    }

    // If socket is readable, call recv
    ssize_t bytes_received = recv(sockfd, buffer, len, 0);
    if (bytes_received == -1) {
        perror("recv error");
        return -1;  // Error occurred
    }

    return bytes_received;  // Return number of bytes received
}

int main1() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[1024];

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);  // Server port
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Server IP

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect error");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server. Waiting for data...\n");

    // Call the recv_with_timeout function
    ssize_t bytes = recv_with_timeout(sockfd, buffer, sizeof(buffer), 10);
    if (bytes > 0) {
        buffer[bytes] = '\0';  // Null-terminate the received data
        printf("Received: %s\n", buffer);
    } else if (bytes == 0) {
        printf("Server closed the connection.\n");
    }

    // Close the socket
    close(sockfd);
    return 0;
}
int main()
{
    typedef struct{
        int a;
        int b;
        int c;
        int d;
    }o;
    o a;
    o b;
    a.a=100;
    a.b=101;
    a.c=1021232;
    a.d=103;
    memcpy(&b,&a,sizeof(o));
    printf("%d %d %d %d\n",b.a,b.b,b.c,b.d);
    return 0;
}