#ifndef SERVER
#define SERVER

#include <functional>
#include <vector>
#include <map>
#include <boost/asio.hpp>
#include "packet/packet_factory.hpp"

using tcp = boost::asio::ip::tcp;
using error_code_t = boost::system::error_code;

int new_id = 0;

typedef struct connection
{
    int id;
    tcp::socket socket;
    char buf[1 + sizeof(int32_t)];
    connection(boost::asio::io_service &io_service) : socket(io_service)
    {
        id = new_id++;
    }
    ~connection()
    {
        socket.close();
    }

} connection_t;

class server
{
private:
    ::packet_factory packet_factory;

    // std::map<int, std::vector<std::function<void(int id, packet* packet)>>> listeners;

    boost::asio::io_service io_service;
    tcp::endpoint endpoint;
    tcp::acceptor acceptor{io_service};

    void handle_accept();
    void handle_read(std::shared_ptr<connection_t> con);
    void handle_read_data(std::shared_ptr<connection_t> con, char packet_id, int32_t size);

    std::unique_ptr<std::thread> server_thread;

public:
    std::map<int, std::vector<std::function<void(int id, packet* packet)>>> listeners;
    server(int);
    ~server();
    int get_port();
    void start();
    void join_thread();

    void send_packet(packet*);
    void send_packet(int client_id, packet*);
    template <typename P>
    void register_packet_listener(std::function<void(int id, P *packet)>);


};

#endif
