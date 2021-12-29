#include "packet_factory.hpp"

packet_factory::packet_factory()
{
    this->initialize();
}

void packet_factory::registerPacket(packet* packet)
{
    this->packet_map[packet->get_id()] = packet;
}

void packet_factory::initialize()
{
    this->registerPacket(new packet_hello_world());
}

packet* packet_factory::get_packet_from_id(char id)
{
    if (packet_map.find(id) == packet_map.end()) return nullptr;
    return this->packet_map[id]->clone(); 
}

// int main()
// {
//     packet_data_t pdi;
//     pdi.data = (char*) "Hallo";
//     pdi.length = 5;

//     packet_factory* pf = new packet_factory();
    
//     packet* packet = pf->get_packet_from_id(0);
//     packet->deserialize(pdi);
//     packet_data_t pdo = packet->serialize();
//     std::cout << pdo.data << std::endl;
// }
