//
// Created by Administrator on 2023/12/13.
//

#include <core.hpp>

void init(TinyWebsocketServer &server);

int main(int argc, char **argv)
{
    TinyWebsocketServer server;
    server.setBlock(true);
    if (argc > 1)
    {
        server.setHost(argv[1]);
    }
    if (argc > 2)
    {
        server.setPort(std::stoi(argv[2]));
    }
    init(server);
    server.run();
    return 0;
}