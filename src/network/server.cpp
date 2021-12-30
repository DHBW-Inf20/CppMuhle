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

    // server_thread = std::make_unique<std::thread>([this]() { io_service.run(); }); // C++14
    server_thread = std::unique_ptr<std::thread>(new std::thread([this]() { io_service.run(); }));
}

void server::join_thread()
{
    server_thread->join();
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
    auto buf = boost::asio::buffer(con->buf, 1 + sizeof(int32_t));
    boost::asio::async_read(con->socket, buf, [this, con](error_code_t ec, size_t len) {
        if (!ec)
        {
            char packet_id = con->buf[0];
            int32_t size;
            std::memcpy(&size, &con->buf[1], sizeof(int32_t));
            std::cout << "Recieved packet " << (int) packet_id << " from " << con->id << std::endl;
            std::cout << "size: " << size << std::endl;
            this->handle_read_data(con, packet_id, size);
            this->handle_read(con);
        } 
        else
        {
            // Client disconnected
            con->socket.close();
        }
    });
}

void server::handle_read_data(std::shared_ptr<connection_t> con, char packet_id, int32_t size)
{
    char* data_buf = (char*) malloc(size);
    auto buf = boost::asio::buffer(data_buf, size);
    boost::asio::async_read(con->socket, buf, [this, data_buf, con, packet_id](error_code_t ec, size_t len) {
        if (!ec)
        {
            packet_data_t packet_data;
            packet_data.size = len;
            packet_data.data = data_buf;
            packet* packet = packet_factory.get_packet_from_id(packet_id);
            if (packet)
            {
                packet->deserialize(packet_data);
                if (listeners.find(packet_id) != listeners.end())
                {
                    for (int i = 0; i < listeners[packet_id].size(); i++)
                    {
                        listeners[packet_id][i](con->id, packet);
                    }
                }
            }
            else
            {
                std::cout << "Packet with id " << packet_id << "not found" << std::endl;
            }
        }
        delete data_buf;
    });
}

void server::send_packet(int client_id, packet*)
{

}

void server::send_packet(packet*)
{
    
}

template <typename P>
void server::register_packet_listener(std::function<void(int id, P *packet)> method)
{
    char packet_id = P().get_id();
    if (listeners.find(packet_id) == listeners.end())
    {
        listeners[packet_id] = std::vector<std::function<void(int id, packet *packet)>>();
    }
    listeners[packet_id].push_back([method](int id, packet *packet) {
        method(id, (P*) packet);
    });
}


int main()
{
    server server(1337);

    server.register_packet_listener<packet_hello_world>([](int id, packet_hello_world *packet) {
        std::cout << "Message from " << id << ": " << packet->str << std::endl;
    });

    server.start();
    server.join_thread();
}
