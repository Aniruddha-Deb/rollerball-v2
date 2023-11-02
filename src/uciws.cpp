#include <iostream>
#include "uciws.hpp"
#include "board.hpp"
#include "butils.hpp"

#include <string>
#include <sstream>
#include <unistd.h>
#include <thread>
#include <vector>
#include <iterator>

template <typename Out>
void split(const std::string &s, char delim, Out result) {
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim)) {
        *result++ = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

UCIWSServer::UCIWSServer(std::string name, uint32_t port) {
    this->name = name;
    this->port = port;
}

void UCIWSServer::handle_message(ClientConnection conn, const std::string& message) {

    auto toks = split(message, ' ');

    if (toks[0] == "uci") {
        on_uci();
    }
    else if (toks[0] == "ucinewgame") {
        on_ucinewgame(toks);
    }
    else if (toks[0] == "position") {
        on_position(toks);
    }
    else if (toks[0] == "go") {
        on_go(toks);
    }
    else if (toks[0] == "quit") {
        on_quit();
    }
    else {
        std::cout << "Unsupported message\n";
    }
}

void UCIWSServer::start() {

    // Register our network callbacks, ensuring the logic is run on the main thread's event loop
    server.connect([this](ClientConnection conn)
    {
        this->main_evt_loop.post([conn, this]()
        {
            std::clog << "Connection opened." << std::endl;
            std::clog << "There are now " << server.numConnections() << " open connections." << std::endl;
        });
    });

    server.disconnect([this](ClientConnection conn)
    {
        main_evt_loop.post([conn, this]()
        {
            std::clog << "Connection closed." << std::endl;
            std::clog << "There are now " << server.numConnections() << " open connections." << std::endl;
        });
    });

    server.message([this](ClientConnection conn, const string& message) {
        this->handle_message(conn, message);
    });
    
    //Start the networking thread
    this->server_thread = std::thread([this]() {
        server.run(port);
    });
    
    //Start the event loop for the main thread
    asio::io_service::work work(main_evt_loop);
    main_evt_loop.run();
}

void UCIWSServer::on_uci() {
    std::cout << "In method on_uci\n";
    server.broadcastMessage("uciok");
}

void UCIWSServer::on_ucinewgame(std::vector<std::string>& toks) {
    std::cout << "In method on_ucinewgame\n";
    if (b == nullptr) delete b;
    if (e == nullptr) delete e;
    e = new Engine();
    e->time_left = std::chrono::milliseconds(stoi(toks[2]));
    if (toks[1] == "board-7-3") {
        b = new Board(SEVEN_THREE);
    }
    else if (toks[1] == "board-8-4") {
        b = new Board(EIGHT_FOUR);
    }
    else if (toks[1] == "board-8-2") {
        b = new Board(EIGHT_TWO);
    }
    else {
        std::cout << "Received invalid board type from server\n";
    }
    server.broadcastMessage("newgameok");
}

void UCIWSServer::on_position(std::vector<std::string>& toks) {
    std::cout << "In method on_position\n";
    if (toks.size() > 3) {
        b->do_move_(str_to_move(toks[toks.size()-1]));
    }
}

void UCIWSServer::on_go(std::vector<std::string>& toks) {
    std::cout << "In method on_go\n";
    // launch a thread to find the best move
    e->time_left = std::chrono::milliseconds(stoi(toks[1]));
    this->game_thread = std::thread([this]() {
        e->find_best_move(*b);
        server.broadcastMessage("bestmove " + move_to_str(e->best_move));
        b->do_move_(e->best_move);
    });
    this->game_thread.join();
}

void UCIWSServer::on_quit() {
    std::cout << "In method on_quit\n";
}

