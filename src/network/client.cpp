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
    auto buf = boost::asio::buffer(server_con->buf, 1 + sizeof(int32_t));
    boost::asio::async_read(server_con->socket, buf, [this](error_code_t ec, size_t len) {
        if (!ec)
        {
            char packet_id = server_con->buf[0];
            int32_t size;
            std::memcpy(&size, &server_con->buf[1], sizeof(int32_t));
            this->receive_packet(packet_id, size);
            this->handle_read();
        } 
        else
        {
            // Server disconnected
            server_con->socket.close();
        }
    });
}

void client::receive_packet(char packet_id, int32_t size)
{
    char* data_buf = (char*) malloc(size);
    auto buf = boost::asio::buffer(data_buf, size);
    boost::asio::async_read(server_con->socket, buf, [this, data_buf, packet_id](error_code_t ec, size_t len) {
        if (!ec)
        {
            packet* packet = packet_factory.get_packet_from_id(packet_id);
            if (packet)
            {
                packet_data_t packet_data;
                packet_data.size = len;
                packet_data.data = data_buf;
                packet->deserialize(packet_data);
                this->call_listeners(packet);
                delete packet;
            }
            else
            {
                std::cout << "Packet with id " << packet_id << "not found" << std::endl;
            }
        }
        delete data_buf;
    });
}

void client::call_listeners(packet* packet)
{
    if (listeners.find(packet->get_id()) != listeners.end())
    {
        for (int i = 0; i < listeners[packet->get_id()].size(); i++)
        {
            listeners[packet->get_id()][i](packet);
        }
    }
}

bool client::write_data(tcp::socket &socket, boost::asio::mutable_buffers_1 &buf, boost::asio::mutable_buffers_1 &data_buf)
{
    error_code_t ec;
    boost::asio::write(socket, buf, ec);
    if (ec) return false;
    boost::asio::write(socket, data_buf, ec);
    if (ec) return false;
    return true;
}


bool client::send_packet(packet* packet)
{
    if (server_con->socket.is_open()) {
        packet_data_t packet_data = packet->serialize();

        char cbuf[1 + sizeof(int32_t)];
        cbuf[0] = packet->get_id();
        std::memcpy(&cbuf[1], &packet_data.size, sizeof(int32_t));
        auto buf = boost::asio::buffer(cbuf, 1 + sizeof(int32_t));
        auto data_buf = boost::asio::buffer(packet_data.data, packet_data.size);
        return write_data(server_con->socket, buf, data_buf);
    }
    return false;
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

    client.register_packet_listener<packet_hello_world>([](packet_hello_world* packet) {
        std::cout << packet->str << std::endl;
    });

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
