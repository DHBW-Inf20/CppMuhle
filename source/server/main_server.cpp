#include <iostream>
#include "../network/net_server.hpp"
#include "../logic/muhle_logik.hpp"
#include "../logic/iview.hpp"

int main()
{

    net_server server(42069);



    server.start();
    server.join_thread();

}


