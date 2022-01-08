#include "packet.hpp"
#include <string>

class packet_exchange_names : public packet
{
public:
    std::string enemy_name;

    packet_data_t serialize()
    {
        char *data = (char *)malloc(enemy_name.length());
        memcpy(data, enemy_name.data(), enemy_name.length());
        packet_data_t packet_data;
        packet_data.data = data;
        packet_data.size = enemy_name.length();
        return packet_data;
    }

    void deserialize(packet_data_t &packet_data)
    {
        enemy_name = std::string(packet_data.data, packet_data.size);
    }

    packet *clone()
    {
        return new packet_exchange_names();
    }

    char get_id()
    {
        return PACKET_EXCHANGE_NAMES_ID;
    }
};
