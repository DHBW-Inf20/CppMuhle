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

    server.start();

    server.register_packet_listener<packet_hello_world>([](int id, packet_hello_world *packet) {
        std::cout << "Test1 " << packet->str << std::endl;
    });
    server.register_packet_listener<packet_hello_world>([](int id, packet_hello_world *packet) {
        std::cout << "Test2 " << packet->str << std::endl;
    });

    packet_hello_world *phw = new packet_hello_world();
    phw->str = "Hello world!";
    for (int i = 0; i < server.listeners[phw->get_id()].size(); i++)
    {
        server.listeners[phw->get_id()][i](0, phw);
    }
    
    server.join_thread();
}
