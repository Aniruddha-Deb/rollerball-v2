#pragma once

#include <vector>
#include <unordered_set>
#include <stack>
#include "constants.hpp"
#include "bdata.hpp"

struct Board {

    BoardData data;

    Board();
    Board(BoardType btype);
    Board(BoardData bdata);
    Board(const Board& source);

    std::unordered_set<U16> get_legal_moves() const;
    bool in_check() const;
    void do_move_(U16 move);

    std::unordered_set<U16> get_pseudolegal_moves() const;
    std::unordered_set<U16> get_pseudolegal_moves_for_piece(U8 piece_pos) const;
    void flip_player_();
    void do_move_without_flip_(U16 move);
    bool under_threat(U8 piece_pos) const;
    void undo_last_move_without_flip_(U16 move);
    std::unordered_set<U16> get_pseudolegal_moves_for_side(U8 color) const;
};
