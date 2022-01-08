#ifndef NET_CLIENT_HPP
#define NET_CLIENT_HPP

#include <functional>
#include <vector>
#include <thread>
#include <future>
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

typedef struct packet_buf
{
    char cbuf[1 + sizeof(int32_t)];
    std::vector<boost::asio::const_buffer> buffers;
} packet_buf_t;

class net_client
{
private:
    ::packet_factory packet_factory;

    std::map<char, std::vector<std::function<void(packet* packet)>>> listeners;
    std::map<char, std::promise<packet*>*> promises;

    boost::asio::io_service io_service;
    std::string address;
    int port;


    void handle_read();
    void receive_packet(char packet_id, int32_t size);
    bool call_promises(packet*);
    void call_listeners(packet*);

    packet_buf_t get_packet_buf(packet*, packet_data_t &packet_data);
    void write_data(tcp::socket&, packet_buf_t &packet_buf);

    std::shared_ptr<connection_t> server_con;
    std::unique_ptr<std::thread> client_thread;

public:
    net_client(std::string address, int port);
    ~net_client();
    std::string get_address();
    int get_port();
    void start();
    void stop();
    void join_thread();

    void send_packet(packet*);
    
    template <class P>
    void register_packet_listener(std::function<void(P *packet)> method)
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

    template <class P>
    P* wait_for_packet()
    {
        char packet_id = P().get_id();

        std::promise<packet*> promise;
        std::future<packet*> future = promise.get_future();

        promises[packet_id] = &promise;

        P* packet = (P*) future.get();

        promises.erase(packet_id);

        return packet;
    }

    template <class P>
    P* send_and_receive_packet(packet* packet)
    {
        send_packet(packet);
        return wait_for_packet<P>();
    }
};

#endif
