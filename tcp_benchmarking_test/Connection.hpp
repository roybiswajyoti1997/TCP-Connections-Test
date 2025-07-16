#ifndef MEM_CONNECT_CONNECTION_HPP_
#define MEM_CONNECT_CONNECTION_HPP_

#include <string>
#include <cstddef>   // for size_t
#include <sys/types.h>

class Connection {
public:
    Connection();
    ~Connection();

    // Client side
    bool connectToServer(const std::string& ip, int port);

    // Server side
    bool bindAndListen(int port);
    bool acceptConnection();

    // Shared (client or server)
    ssize_t sendData(const void* buffer, size_t size);
    ssize_t receiveData(void* buffer, size_t size);
    void closeConnection();

private:
    int socket_fd;     // For client or server socket
    int client_fd = -1; // Only used on the server side after accept()
};

#endif  // MEM_CONNECT_CONNECTION_HPP_
