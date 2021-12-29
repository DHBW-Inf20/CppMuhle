#include "client.hpp"
#include <iostream>
#include <string>

client::client(std::string address, int port)
{
    endpoint = tcp::endpoint(boost::asio::ip::address::from_string(address), port);
}

client::~client()
{
    std::cout << "Client stopped" << std::endl;
}

std::string client::get_address()
{
    return endpoint.address().to_string();
}

int client::get_port()
{
    return endpoint.port();
}

void client::start()
{
    auto con = std::make_shared<connection_t>(io_service);
    con->socket.connect(endpoint);

    handle_read(con);

    auto t = std::thread([this]() { io_service.run(); });
    t.join();
}

void client::handle_read(std::shared_ptr<connection_t> con)
{
    boost::asio::async_read_until(con->socket, con->buf, "\n", [this, con](error_code_t ec, size_t len) {
        if (!ec)
        {
            std::cout << std::istream(&con->buf).rdbuf();
            con->buf.consume(len);
            this->handle_read(con);
        }
    });
}

int main()
{    
    client client("127.0.0.1", 1337);
    client.start();
}
