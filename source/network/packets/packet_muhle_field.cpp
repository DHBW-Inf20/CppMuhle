#include "packet.hpp"
#include <string>
#include "../../helperTypes.hpp"

class packet_muhle_field: public packet
{
public:
    int24 black;
    int24 white;

    packet_data_t serialize() 
    {
        char* data = (char*) std::malloc(2 * sizeof(int24));
        std::memcpy(data, &black, sizeof(int24));
        std::memcpy(data + sizeof(int24), &white, sizeof(int24));
        packet_data_t packet_data;
        packet_data.data = data;
        packet_data.size = 2 * sizeof(int24);
        return packet_data;
    }

    void deserialize(packet_data_t &packet_data)
    {
        std::memcpy(&black, packet_data.data, sizeof(int24));
        std::memcpy(&white, packet_data.data + sizeof(int24), sizeof(int24));
    }

    packet* clone()
    {
        return new packet_muhle_field();
    }

    char get_id()
    {
        return 0;
    }
};