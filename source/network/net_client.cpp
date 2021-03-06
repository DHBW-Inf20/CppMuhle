#include "net_client.hpp"
#include <iostream>
#include <string>

connection::connection(boost::asio::io_service &io_service) : socket(io_service) {}

connection::~connection()
{
    socket.close();
}

net_client::net_client(std::string address, int port)
{
    this->address = address;
    this->port = port;
}

net_client::~net_client()
{
}

std::string net_client::get_address()
{
    return address;
}

int net_client::get_port()
{
    return port;
}

void net_client::start()
{
    server_con = std::make_shared<connection_t>(io_service);

    tcp::resolver resolver{io_service};
    tcp::resolver::query query(address.data(), std::to_string(port).data());
    auto it = resolver.resolve(query);

    boost::asio::connect(server_con->socket, it);

    packet_socket_connect psc;
    this->call_listeners(&psc);

    handle_read();

    // client_thread = std::make_unique<std::thread>([this]() { io_service.run(); }); // C++14
    client_thread = std::unique_ptr<std::thread>(new std::thread([this]() { io_service.run(); }));
}

void net_client::stop()
{
    server_con->socket.close();
    io_service.stop();
    client_thread->detach();
}

void net_client::join_thread()
{
    client_thread->join();
}


void net_client::handle_read()
{
    auto buf = boost::asio::buffer(server_con->buf, 1 + sizeof(int32_t));
    boost::asio::async_read(server_con->socket, buf, [this](error_code_t ec, size_t len) {
        if (!ec)
        {
            char packet_id = server_con->buf[0];
            int32_t size;
            memcpy(&size, &server_con->buf[1], sizeof(int32_t));
            this->receive_packet(packet_id, size);
            this->handle_read();
        } 
        else
        {
            // Server disconnected
            packet_socket_disconnect psd;
            this->call_listeners(&psd);

            this->stop();
        }
    });
}

void net_client::receive_packet(char packet_id, int32_t size)
{
    if (size > 0)
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
                    if (!this->call_promises(packet))
                    {
                        delete packet;
                    } // else: SELBER DELETEN!
                }
                else
                {
                    std::cout << "Packet with id " << packet_id << " not found" << std::endl;
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
            call_listeners(packet);
            delete packet;
        }
        else
        {
            std::cout << "Packet with id " << packet_id << " not found" << std::endl;
        }
    }
}

bool net_client::call_promises(packet* packet)
{
    if (promises.find(packet->get_id()) != promises.end())
    {
        promises[packet->get_id()]->set_value(packet);
        return true;
    }
    return false;
}

void net_client::call_listeners(packet* packet)
{
    if (listeners.find(packet->get_id()) != listeners.end())
    {
        for (long long unsigned int i = 0; i < listeners[packet->get_id()].size(); i++)
        {
            listeners[packet->get_id()][i](packet);
        }
    }
}

packet_buf_t net_client::get_packet_buf(packet *packet, packet_data_t &packet_data)
{
    packet_buf_t packet_buf;

    packet_buf.cbuf[0] = packet->get_id();
    memcpy(&packet_buf.cbuf[1], &packet_data.size, sizeof(int32_t));
    
    packet_buf.buffers.push_back(boost::asio::buffer(packet_buf.cbuf, 1 + sizeof(int32_t)));
    packet_buf.buffers.push_back(boost::asio::buffer(packet_data.data, packet_data.size));

    return packet_buf;
}

void net_client::write_data(tcp::socket &socket, packet_buf_t &packet_buf)
{
    boost::asio::async_write(socket, packet_buf.buffers, [](error_code_t ec, size_t len) {});
}


void net_client::send_packet(packet* packet)
{
    if (server_con->socket.is_open()) {
        packet_data_t packet_data = packet->serialize();
        packet_buf_t packet_buf = get_packet_buf(packet, packet_data);

        write_data(server_con->socket, packet_buf);

        if (packet_data.size > 0 && packet_data.data != nullptr)
        {
            free(packet_data.data);
        }
    }
}
