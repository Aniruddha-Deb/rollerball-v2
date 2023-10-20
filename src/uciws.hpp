#pragma once

#include <csignal>
#include <string>
#include <thread>
#include <asio/io_service.hpp>

#include "server.hpp"
#include "board.hpp"
#include "engine.hpp"

class UCIWSServer {

    public:

    asio::io_service main_evt_loop;
    WebsocketServer server;
    
    std::thread server_thread;
    std::thread game_thread;
    std::atomic<bool> running;

    uint32_t port;
    std::string name;

    Board *b;
    Engine *e;

    UCIWSServer(std::string name, uint32_t port);

    void start();
    void stop();

    void handle_message(ClientConnection conn, const std::string& message);

    void on_uci();
    void on_isready();
    void on_ucinewgame(std::vector<std::string>& toks);
    void on_position(std::vector<std::string>& toks);
    void on_go(std::vector<std::string>& toks);
    void on_quit();
};
