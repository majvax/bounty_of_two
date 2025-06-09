#define ASIO_STANDALONE
#include "test.hpp"
#include <asio.hpp>
#include <iostream>

int ping() {
    try {
        asio::io_context io_context;

        // Resolve the host name and service to endpoints
        asio::ip::tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve("google.com", "80");

        // Create and connect the socket
        asio::ip::tcp::socket socket(io_context);
        asio::connect(socket, endpoints);

        // Form the HTTP GET request
        std::string request = 
            "GET / HTTP/1.1\r\n"
            "Host: google.com\r\n"
            "Connection: close\r\n\r\n";

        // Send the request
        asio::write(socket, asio::buffer(request));

        // Read and print the response
        for (;;) {
            char buffer[1024];
            asio::error_code error;
            size_t len = socket.read_some(asio::buffer(buffer), error);

            if (error == asio::error::eof) {
                // Connection closed cleanly by peer
                break;
            } else if (error) {
                throw asio::system_error(error);
            }

            std::cout.write(buffer, len);
        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
