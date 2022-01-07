#include "net_server.hpp"
#include <iostream>
#include <string>

int new_id = 1;
connection::connection(boost::asio::io_service &io_service) : socket(io_service)
{
    id = new_id++;
}

connection::~connection()
{
    socket.close();
}

net_server::net_server(int port)
{
    endpoint = tcp::endpoint(tcp::v4(), port);
}

net_server::~net_server()
{
    std::cout << "Server stopped" << std::endl;
}

int net_server::get_port()
{
    return endpoint.port();
}

void net_server::start()
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

void net_server::stop()
{
    packet_socket_disconnect psd;
    for (auto &map : clients)
    {
        this->call_listeners(map.first, &psd);
        map.second->socket.close();
    }
    clients.clear();
    io_service.stop();
    server_thread->detach();
}

void net_server::join_thread()
{
    server_thread->join();
}

void net_server::handle_accept()
{  
    auto con = std::make_shared<connection_t>(io_service);
    acceptor.async_accept(con->socket, [this, con](const error_code_t &ec) {
        if (!ec)
        {
            clients[con->id] = con;
            this->handle_read(con);

            packet_socket_connect psc;
            this->call_listeners(con->id, &psc);
        }
        this->handle_accept();
    });
}

void net_server::handle_read(std::shared_ptr<connection_t> con)
{
    auto buf = boost::asio::buffer(con->buf, 1 + sizeof(int32_t));
    boost::asio::async_read(con->socket, buf, [this, con](error_code_t ec, size_t len) {
        if (!ec)
        {
            char packet_id = con->buf[0];
            int32_t size;
            memcpy(&size, &con->buf[1], sizeof(int32_t));
            // std::cout << "Recieved packet " << (int) packet_id << " from " << con->id << std::endl;
            // std::cout << "size: " << size << std::endl;
            this->receive_packet(con, packet_id, size);
            this->handle_read(con);
        } 
        else
        {
            // Client disconnected
            packet_socket_disconnect psd;
            this->call_listeners(con->id, &psd);

            clients.erase(con->id);
            con->socket.close();
        }
    });
}

void net_server::receive_packet(std::shared_ptr<connection_t> con, char packet_id, int32_t size)
{
    if (size > 0)
    {
        char* data_buf = (char*) malloc(size);
        auto buf = boost::asio::buffer(data_buf, size);
        boost::asio::async_read(con->socket, buf, [this, data_buf, con, packet_id](error_code_t ec, size_t len) {
            if (!ec)
            {
                packet* packet = packet_factory.get_packet_from_id(packet_id);
                if (packet)
                {
                    packet_data_t packet_data;
                    packet_data.size = len;
                    packet_data.data = data_buf;
                    packet->deserialize(packet_data);
                    this->call_listeners(con->id, packet);
                    delete packet;
                }
                else
                {
                    std::cout << "Packet with id " << packet_id << "not found" << std::endl;
                }
            }
            free(data_buf);
        });
    }
    else
    {
        packet* packet = packet_factory.get_packet_from_id(packet_id);
        if (packet)
        {
            call_listeners(con->id, packet);
            delete packet;
        }
        else
        {
            std::cout << "Packet with id " << packet_id << "not found" << std::endl;
        }
    }
}

void net_server::call_listeners(int from_id, packet* packet)
{
    if (listeners.find(packet->get_id()) != listeners.end())
    {
        for (long long unsigned int  i = 0; i < listeners[packet->get_id()].size(); i++)
        {
            listeners[packet->get_id()][i](from_id, packet);
        }
    }
}

packet_buf_t net_server::get_packet_buf(packet *packet, packet_data_t &packet_data)
{
    packet_buf_t packet_buf;

    packet_buf.cbuf[0] = packet->get_id();
    memcpy(&packet_buf.cbuf[1], &packet_data.size, sizeof(int32_t));
    
    packet_buf.buffers.push_back(boost::asio::buffer(packet_buf.cbuf, 1 + sizeof(int32_t)));
    packet_buf.buffers.push_back(boost::asio::buffer(packet_data.data, packet_data.size));

    return packet_buf;
}

void net_server::write_data(tcp::socket &socket, packet_buf_t &packet_buf)
{
    boost::asio::async_write(socket, packet_buf.buffers, [](error_code_t ec, size_t len) {});
}

bool net_server::is_connected(int client_id)
{
    if (clients.find(client_id) != clients.end()) {
        return clients[client_id]->socket.is_open();
    }
    return false;
}

void net_server::send_packet(packet *packet)
{
    packet_data_t packet_data = packet->serialize();
    packet_buf_t packet_buf = get_packet_buf(packet, packet_data);

    for (auto &map : clients) {
        std::shared_ptr<connection_t> con = map.second;
        if (con->socket.is_open())
        {
            write_data(con->socket, packet_buf);
        }
    }
    
    if (packet_data.size > 0 && packet_data.data != nullptr)
    {
        free(packet_data.data);
    }
}

void net_server::send_packet(packet *packet, int client_id)
{
    if (is_connected(client_id)) {
        packet_data_t packet_data = packet->serialize();

        std::shared_ptr<connection_t> con = clients[client_id];
        packet_buf_t packet_buf = get_packet_buf(packet, packet_data);

        write_data(con->socket, packet_buf);

        if (packet_data.size > 0 && packet_data.data != nullptr)
        {
            free(packet_data.data);
        }
    }
}

void net_server::send_packet(packet *packet, std::vector<int> &client_ids)
{
    send_packet(packet, client_ids.data(), client_ids.size());
}

void net_server::send_packet(packet *packet, int *client_ids, int size)
{
    packet_data_t packet_data = packet->serialize();
    packet_buf_t packet_buf = get_packet_buf(packet, packet_data);

    for (int* client_id = client_ids; client_id < client_ids + size; client_id++)
    {
        if (is_connected(*client_id)) {
            std::shared_ptr<connection_t> con = clients[*client_id];
            write_data(con->socket, packet_buf);
        }
    }

    if (packet_data.size > 0 && packet_data.data != nullptr)
    {
        free(packet_data.data);
    }
}
