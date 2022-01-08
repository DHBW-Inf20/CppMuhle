#include "packet.hpp"

class packet_socket_connect: public packet
{
public:

    packet_data_t serialize() 
    {
        packet_data_t packet_data;
        packet_data.size = 0;
        return packet_data;
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
        return PACKET_SOCKET_CONNECT_ID;
    }
};
