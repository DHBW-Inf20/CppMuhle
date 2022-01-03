#include "packet.hpp"

class packet_socket_connect: public packet
{
public:

    packet_data_t serialize() 
    {
        return packet_data_t();
    }

    void deserialize(packet_data_t &packet_data)
    {
    }

    packet* clone()
    {
        return new packet_socket_connect();
    }

    char get_id()
    {
        return -1;
    }
};
