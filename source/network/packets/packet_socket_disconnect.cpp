#include "packet.hpp"

class packet_socket_disconnect: public packet
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
        return new packet_socket_disconnect();
    }

    char get_id()
    {
        return PACKET_SOCKET_DISCONNECT_ID;
    }
};
