#include <iostream>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    const char *message = "Hello from server";

    // 1. Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // 2. Set socket options
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    // 3. Define the address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;  // Accept any IP address
    address.sin_port = htons(PORT);        // Host to Network Short (8080 -> network byte order)

    // 4. Bind the socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // 5. Start listening
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << PORT << "...\n";

    // 6. Accept connection
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (new_socket < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    // 7. Read from client
    read(new_socket, buffer, 1024);
    std::cout << "Received from client: " << buffer << std::endl;

    // 8. Send response
    send(new_socket, message, strlen(message), 0);
    std::cout << "Message sent to client\n";

    // 9. Close sockets
    close(new_socket);
    close(server_fd);
    return 0;
}
