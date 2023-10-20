#pragma once

#include "board.hpp"
#include <chrono>

class AbstractEngine {

    public:
    U16 best_move;
    std::chrono::milliseconds time_left;

    virtual void find_best_move(const Board& b) = 0;
};
