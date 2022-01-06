#include "packet.hpp"
#include <string>

class packet_game_code: public packet
{
public:
    std::string code;

    packet_data_t serialize() 
    {
        char* data = (char*) malloc(code.length());
        memcpy(data, code.data(), code.length());
        packet_data_t packet_data;
        packet_data.data = data;
        packet_data.size = code.length();
        return packet_data;
    }

    void deserialize(packet_data_t &packet_data)
    {
        code = std::string(packet_data.data, packet_data.size);
    }

    packet* clone()
    {
        return new packet_game_code();
    }

    char get_id()
    {
        return PACKET_GAME_CODE_ID;
    }
};
