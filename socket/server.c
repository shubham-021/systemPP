#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>


#define PORT 8000
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));


    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(PORT);

    if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd, 5) < 0) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd < 0) {
        perror("accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Client connected: %s\n", inet_ntoa(client_addr.sin_addr));

    while(1) {
        // const char* response = "Hello from server";
        // send(client_fd, response, strlen(response), 0); // can also use write here: write(client_fd, response, strlen(response))

        int bytes_received = recv(client_fd, buffer, BUFFER_SIZE - 1, 0); // can also use read here: read(client_fd, buffer, BUFFER_SIZE - 1)

        if (bytes_received == 0) {
            printf("Client disconnected");
            break;
        }

        buffer[bytes_received] = '\0';
        printf("Client says: %s\n", buffer);
    }

    close(server_fd);
    close(client_fd);

    return 0;
}
