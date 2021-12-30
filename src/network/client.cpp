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
    server_con = std::make_shared<connection_t>(io_service);
    server_con->socket.connect(endpoint);

    handle_read();

    // server_thread = std::make_unique<std::thread>([this]() { io_service.run(); }); // C++14
    client_thread = std::unique_ptr<std::thread>(new std::thread([this]() { io_service.run(); }));
}

void client::join_thread()
{
    client_thread->join();
}


void client::handle_read()
{
    boost::asio::async_read_until(server_con->socket, server_con->buf, "\n", [this](error_code_t ec, size_t len) {
        if (!ec)
        {
            std::cout << std::istream(&server_con->buf).rdbuf();
            server_con->buf.consume(len);
            this->handle_read();
        }
    });
}


void client::send_packet(packet* packet)
{
    packet_data_t packet_data = packet->serialize();
    char cbuf[1 + sizeof(int32_t)];
    cbuf[0] = packet->get_id();
    std::memcpy(&cbuf[1], &packet_data.size, sizeof(int32_t));
    auto buf = boost::asio::buffer(cbuf, 1 + sizeof(int32_t));
    // boost::asio::async_write(server_con->socket, buf, [this, packet_data](error_code_t e, size_t r) {
    //     auto buf = boost::asio::buffer(packet_data.data, packet_data.length);
    //     boost::asio::async_write(server_con->socket, buf, [](error_code_t e, size_t r) {
    //     });
    // });

    std::size_t size = boost::asio::write(server_con->socket, buf);
    if (size != 0) {
        auto data_buf = boost::asio::buffer(packet_data.data, packet_data.size);
        boost::asio::write(server_con->socket, data_buf);
    }
}

template <typename P>
void client::register_packet_listener(std::function<void(P *packet)> method)
{
    char packet_id = P().get_id();
    if (listeners.find(packet_id) == listeners.end())
    {
        listeners[packet_id] = std::vector<std::function<void(packet *packet)>>();
    }
    listeners[packet_id].push_back([method](packet *packet) {
        method((P*) packet);
    });
}


int main()
{    

    client client("127.0.0.1", 1337);
    client.start();

    packet_hello_world* phw = new packet_hello_world();
    phw->str = "Hello world!";
    client.send_packet(phw);
    phw->str = "C++ Test /+#-.;,äöüß";
    client.send_packet(phw);

    client.join_thread();
}
