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


bool client::send_packet(packet* packet)
{
    char* data = packet->serialize();
    int32_t data_size = sizeof(data);
    char cbuf[1 + sizeof(int32_t)];
    cbuf[0] = packet->get_id();
    std::memcpy(&cbuf[1], &data_size, sizeof(int32_t));
    auto buf = boost::asio::buffer(cbuf, 1 + sizeof(int32_t));
    error_code_t ec;
    boost::asio::write(server_con->socket, buf, ec);
    if (ec) return false;
    auto data_buf = boost::asio::buffer(data, data_size);
    boost::asio::write(server_con->socket, data_buf, ec);
    if (ec) return false;
    return true;
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

    // packet_hello_world* phw = new packet_hello_world();
    // phw->str = "Hello world!";
    // bool test = client.send_packet(phw);
    // std::cout << test << std::endl;
    // phw->str = "C++ Test /+#-.;,äöüß";
    // client.send_packet(phw);

    packet_hello_world* phw = new packet_hello_world();
    while (std::cin.good())
    {
        std::string msg;
        std::getline(std::cin, msg);
        phw->str = msg;
        client.send_packet(phw);
    }

    client.join_thread();
}
