#include <iostream>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    const char *hello = "Hello from client";
    char buffer[1024] = {0};

    // 1. Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Socket creation error\n";
        return -1;
    }

    // 2. Setup server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // 3. Convert IP address
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address / Address not supported\n";
        return -1;
    }

    // 4. Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection Failed\n";
        return -1;
    }

    // 5. Send message
    send(sock, hello, strlen(hello), 0);
    std::cout << "Message sent to server\n";

    // 6. Read server reply
    read(sock, buffer, 1024);
    std::cout << "Received from server: " << buffer << std::endl;

    // 7. Close socket
    close(sock);

    return 0;
}
