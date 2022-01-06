#ifndef NET_SERVER_HPP
#define NET_SERVER_HPP

#include <functional>
#include <vector>
#include <thread>
#include <map>
#include <boost/asio.hpp>
#include "packet_factory.hpp"

using tcp = boost::asio::ip::tcp;
using error_code_t = boost::system::error_code;

typedef struct connection
{
    int id;
    tcp::socket socket;
    char buf[1 + sizeof(int32_t)];
    connection(boost::asio::io_service &io_service);
    ~connection();
} connection_t;

typedef struct packet_buf
{
    char cbuf[1 + sizeof(int32_t)];
    std::vector<boost::asio::const_buffer> buffers;
} packet_buf_t;

class net_server
{
private:
    ::packet_factory packet_factory;

    std::map<int, std::vector<std::function<void(int id, packet* packet)>>> listeners;
    std::map<int, std::shared_ptr<connection_t>> clients;

    boost::asio::io_service io_service;
    tcp::endpoint endpoint;
    tcp::acceptor acceptor{io_service};

    void handle_accept();
    void handle_read(std::shared_ptr<connection_t> con);
    void receive_packet(std::shared_ptr<connection_t> con, char packet_id, int32_t size);
    void call_listeners(int from_id, packet*);

    packet_buf_t get_packet_buf(packet*, packet_data_t &packet_data);
    void write_data(tcp::socket&, packet_buf_t &packet_buf);

    std::unique_ptr<std::thread> server_thread;

public:
    net_server(int);
    ~net_server();
    int get_port();
    void start();
    void join_thread();

    bool is_connected(int client_id);
    void send_packet(packet*);
    void send_packet(packet*, int client_id);
    void send_packet(packet*, std::vector<int> &client_ids);
    void send_packet(packet*, int *client_ids, int size);

    template <class P>
    void register_packet_listener(std::function<void(int id, P *packet)> method) {
        char packet_id = P().get_id();
        if (listeners.find(packet_id) == listeners.end())
        {
            listeners[packet_id] = std::vector<std::function<void(int id, packet *packet)>>();
        }
        listeners[packet_id].push_back([method](int id, packet *packet) {
            method(id, (P*) packet);
        });
    }
};

#endif
