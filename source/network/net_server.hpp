#ifndef NET_SERVER_HPP
#define NET_SERVER_HPP

#include <functional>
#include <vector>
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
    boost::asio::mutable_buffer buf;
    boost::asio::mutable_buffer data_buf;
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

    packet_buf_t get_packet_buf(packet*);
    bool write_data(tcp::socket&, packet_buf_t &packet_buf);

    std::unique_ptr<std::thread> server_thread;

public:
    net_server(int);
    ~net_server();
    int get_port();
    void start();
    void join_thread();

    bool is_connected(int client_id);
    void send_packet(packet*);
    bool send_packet(packet*, int client_id);
    void send_packet(packet*, std::vector<int> &client_ids);
    void send_packet(packet*, int *client_ids, int size);
    template <typename P>
    void register_packet_listener(std::function<void(int id, P *packet)>);


};

#endif