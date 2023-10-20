#pragma once

#include "engine_base.hpp"
#include <atomic>

class Engine : public AbstractEngine {

    // add extra items here. 
    // Note that your engine will always be instantiated with the default 
    // constructor.
    
    public:
    void find_best_move(const Board& b) override;

};
