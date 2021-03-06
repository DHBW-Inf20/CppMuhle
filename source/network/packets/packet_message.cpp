#include "packet.hpp"
#include <string>

class packet_message: public packet
{
public:
    std::string str;

    packet_data_t serialize() 
    {
        char* data = (char*) malloc(str.length());
        memcpy(data, str.data(), str.length());
        packet_data_t packet_data;
        packet_data.data = data;
        packet_data.size = str.length();
        return packet_data;
    }

    void deserialize(packet_data_t &packet_data)
    {
        str = std::string(packet_data.data, packet_data.size);
    }

    packet* clone()
    {
        return new packet_message();
    }

    char get_id()
    {
        return PACKET_MESSAGE_ID;
    }
};
