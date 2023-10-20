#pragma once

#include <vector>
#include <unordered_set>
#include <stack>
#include "constants.hpp"

enum PlayerColor {
    WHITE=(1<<6),
    BLACK=(1<<7)
};

enum PieceType {
    EMPTY  = 0,
    PAWN   = (1<<1),
    ROOK   = (1<<2),
    KING   = (1<<3),
    BISHOP = (1<<4),
    KNIGHT = (1<<5)

};

enum Promotion {
    PAWN_BISHOP = (1<<6),
    PAWN_ROOK   = (1<<7)
};

enum BoardType {
    SEVEN_THREE = 1,
    EIGHT_FOUR = 2,
    EIGHT_TWO = 3 
};

struct BoardData {

    // DO NOT add any fields above this
    U8 w_rook_1   = DEAD;
    U8 w_rook_2   = DEAD;
    U8 w_king     = DEAD;
    U8 w_bishop   = DEAD;
    U8 w_knight_1 = DEAD;
    U8 w_knight_2 = DEAD;
    U8 w_pawn_1   = DEAD;
    U8 w_pawn_2   = DEAD;
    U8 w_pawn_3   = DEAD;
    U8 w_pawn_4   = DEAD;

    U8 b_rook_1   = DEAD;
    U8 b_rook_2   = DEAD;
    U8 b_king     = DEAD;
    U8 b_bishop   = DEAD;
    U8 b_knight_1 = DEAD;
    U8 b_knight_2 = DEAD;
    U8 b_pawn_1   = DEAD;
    U8 b_pawn_2   = DEAD;
    U8 b_pawn_3   = DEAD;
    U8 b_pawn_4   = DEAD;

    static const int n_pieces = 10;
    U8 board_0[64];
    U8 board_90[64];
    U8 board_180[64];
    U8 board_270[64];

    BoardType board_type = SEVEN_THREE;
    U8 *board_mask;
    PlayerColor player_to_play = WHITE;
    U8 last_killed_piece = 0;
    int last_killed_piece_idx = -1;

    U8 *transform_array[4];
    U8 *inverse_transform_array[4];

    U8 pawn_promo_squares[10];
    int n_pawn_promo_squares;

    BoardData();
    BoardData(BoardType board_type);
    BoardData(const BoardData& source);

    void set_pieces_on_board();
    void set_8_4_layout();
    void set_8_2_layout();
    void set_7_3_layout();
    void set_7x7_transforms();
    void set_8x8_transforms();

};
