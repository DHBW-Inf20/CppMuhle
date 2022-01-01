#ifndef NET_CLIENT_HPP
#define NET_CLIENT_HPP

#include <functional>
#include <vector>
#include <map>
#include <boost/asio.hpp>
#include "packet_factory.hpp"

using tcp = boost::asio::ip::tcp;
using error_code_t = boost::system::error_code;

typedef struct connection
{
    tcp::socket socket;
    char buf[1 + sizeof(int32_t)];
    connection(boost::asio::io_service &io_service);
    ~connection();
} connection_t;

class net_client
{
private:
    ::packet_factory packet_factory;

    std::map<int, std::vector<std::function<void(packet* packet)>>> listeners;

    boost::asio::io_service io_service;
    tcp::endpoint endpoint;

    void handle_read();
    void receive_packet(char packet_id, int32_t size);
    void call_listeners(packet*);

    bool write_data(tcp::socket&, boost::asio::mutable_buffers_1 &buf, boost::asio::mutable_buffers_1 &data_buf);

    std::shared_ptr<connection_t> server_con;
    std::unique_ptr<std::thread> client_thread;

public:
    net_client(std::string address, int port);
    ~net_client();
    std::string get_address();
    int get_port();
    void start();
    void join_thread();

    bool send_packet(packet*);
    template <typename P>
    void register_packet_listener(std::function<void(P *packet)>);
};

#endif
