#include "packet.hpp"
#include <string>
#include "../../logic/helper_types.hpp"

#include <bitset>
#include <iostream>


/*
* packet_muhle_field
* Used to send the updated muhle data to the client
*/
class packet_muhle_field: public packet
{
public:
    int24 black;
    int24 white;

    char black_pieces_left;
    char white_pieces_left;

    char game_state;

    char message;

    bool own_turn;

    packet_data_t serialize() 
    {
        char* data = (char*) malloc(2 * sizeof(int24) + 4 * sizeof(char) + 1 * sizeof(bool));

        memcpy(data, &black, sizeof(int24));
        memcpy(data + sizeof(int24), &white, sizeof(int24));

        memcpy(data + 2 * sizeof(int24), &black_pieces_left, sizeof(char));
        memcpy(data + 2 * sizeof(int24) + sizeof(char), &white_pieces_left, sizeof(char));

        memcpy(data + 2 * sizeof(int24) + 2 * sizeof(char), &game_state, sizeof(char));
        memcpy(data + 2 * sizeof(int24) + 3 * sizeof(char), &message, sizeof(char));

        memcpy(data + 2 * sizeof(int24) + 4 * sizeof(char), &own_turn, sizeof(bool));

        packet_data_t packet_data;
        packet_data.data = data;
        packet_data.size = 2 * sizeof(int24) + 4 * sizeof(char) + 1 * sizeof(bool);
        // std::cout << std::bitset<64>(data) << std::endl;
        return packet_data;
    }

    void deserialize(packet_data_t &packet_data)
    {
        memcpy(&black, packet_data.data, sizeof(int24));
        memcpy(&white, packet_data.data + sizeof(int24), sizeof(int24));
        
        memcpy(&black_pieces_left, packet_data.data + 2 * sizeof(int24), sizeof(char));
        memcpy(&white_pieces_left, packet_data.data + 2 * sizeof(int24) + sizeof(char), sizeof(char));
        
        memcpy(&game_state, packet_data.data + 2 * sizeof(int24) + 2 * sizeof(char), sizeof(char));
        
        memcpy(&message, packet_data.data + 2 * sizeof(int24) +  3 * sizeof(char), sizeof(char));
        
        memcpy(&own_turn, packet_data.data + 2 * sizeof(int24) + 4 * sizeof(char), sizeof(bool));

        // std::cout << std::bitset<64>(&packet_data.data) << std::endl;

    }

    packet* clone()
    {
        return new packet_muhle_field();
    }

    char get_id()
    {
        return PACKET_MUHLE_FIELD_ID;
    }
};