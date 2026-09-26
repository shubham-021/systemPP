#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>


#define PORT 8000
#define BUFFER_SIZE 1024

int main() {
    int sock_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("invalid address");
        exit(EXIT_FAILURE);
    }

    if(connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connection failed");
        exit(EXIT_FAILURE);
    }

    char input[1024];
    const char *message = "Hello from client!";
    send(sock_fd, message, strlen(message), 0);

    while(1) {
        printf("Enter your message for the server: ");
        fgets(input, sizeof(input), stdin);

        send(sock_fd, input, strlen(input), 0);
    }

    // int bytes_received = recv(sock_fd, buffer, BUFFER_SIZE - 1, 0);
    // buffer[bytes_received] = '\0';
    // printf("Server says: %s", buffer);

    close(sock_fd);

    return 0;
}
