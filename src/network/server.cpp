#include "server.hpp"
#include <iostream>
#include <thread>

#define DEBUG(x) std::cout << "[DEBUG] " << x << std::endl

server::server(int port)
{
    endpoint = tcp::endpoint(tcp::v4(), port);
}

server::~server()
{
    std::cout << "Server stopped" << std::endl;
}

int server::get_port()
{
    return endpoint.port();
}

void server::start()
{
    std::cout << "Starting server..." << std::endl;
    acceptor.open(endpoint.protocol());
    acceptor.bind(endpoint);
    acceptor.listen();
    std::cout << "Server listening to port " << get_port() << std::endl;

    handle_accept();

    auto thread_func = [this]() { io_service.run(); };
    std::thread t(thread_func);
    t.join();
}

void server::handle_accept()
{  
    auto con = std::make_shared<connection_t>(io_service);
    acceptor.async_accept(con->socket, [this, con](const error_code_t &ec) {
        if (!ec)
        {
            this->handle_read(con);
            std::cout << "Client connected: " << con->id << std::endl;
        }
        this->handle_accept();
    });
}

void server::handle_read(std::shared_ptr<connection_t> con)
{
    boost::asio::async_read_until(con->socket, con->buf, "\n", [this, con](error_code_t ec, size_t len) {
        if (!ec)
        {
            std::cout << std::istream(&con->buf).rdbuf();
            con->buf.consume(len);
            this->handle_read(con);
        } 
        else
        {
            // Client disconnected
            con->socket.close();
        }
    });
}

int main()
{
    server server(1337);
    server.start();
}
