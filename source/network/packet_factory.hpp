#ifndef PACKET_FACTORY_HPP
#define PACKET_FACTORY_HPP

#include <map>
#include "packets/packet.hpp"

#include "packets/packet_message.cpp"
#include "packets/packet_login.cpp"
#include "packets/packet_muhle_field.cpp"

class packet_factory
{
private:
    std::map<char, packet*> packet_map;

    void initialize();
    void register_packet(packet*);
public:
    packet_factory();
    packet* get_packet_from_id(char);
};

#endif
