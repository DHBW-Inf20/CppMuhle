#ifndef PACKET_FACTORY
#define PACKET_FACTORY

#include <map>
#include "packet.hpp"

class packet_factory
{
private:
    std::map<char, packet*> packet_map;

    void initialize();
    void registerPacket(packet*);
public:
    packet_factory();
    packet* get_packet_from_id(char);
};

#endif
