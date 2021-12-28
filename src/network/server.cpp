#include "Server.hpp"
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
            this->handle_client_answer(con);
            std::cout << "Client connected" << std::endl;
        }
        this->handle_accept();
    });
}

void server::handle_client_answer(std::shared_ptr<connection_t> con)
{
    boost::asio::async_read_until(con->socket, con->buf, "\n", [this, con](error_code_t ec, size_t len) {
        std::cout << std::istream(&con->buf).rdbuf();
        con->buf.consume(len);
        this->handle_client_answer(con);
    });
}

int main()
{
    server server(1337);
    server.start();
}
