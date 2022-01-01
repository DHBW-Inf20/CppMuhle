#ifndef PACKET
#define PACKET

typedef struct packet_data
{
    int32_t size;
    char* data;
} packet_data_t;

class packet
{
public:
    virtual packet_data_t serialize() = 0;
    virtual void deserialize(packet_data_t) = 0;
    virtual char get_id() = 0;
    virtual packet* clone() = 0;
};

#endif