#include <popl.hpp>
#include <iostream>

#include "uciws.hpp"
#include "board.hpp"
#include "engine.hpp"

#define BOT_NAME "cs1200869"

int main(int argc, char** argv) {

    popl::OptionParser op("Rollerball");
    int port;
    auto port_op = op.add<popl::Value<int>>("p", "port", "port number", -1, &port);
    op.parse(argc, argv);

    if (port == -1) {
        std::cout << "ERROR: port is a compulsory argument" << std::endl;
        return 0;
    }

    UCIWSServer server(BOT_NAME, port);

    server.start();

    return 0;
}
