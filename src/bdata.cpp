#include <string>
#include <algorithm>
#include <cstring>
#include "board.hpp"
#include "constants.hpp"

void rotate_board(U8 *src, U8 *tgt, const U8 *transform) {

    for (int i=0; i<64; i++) {
        tgt[transform[i]] = src[i];
    }
}

void BoardData::set_pieces_on_board() {

    if (this->b_rook_1   != DEAD) this->board_0[this->b_rook_1]   = BLACK | ROOK ;
    if (this->b_rook_2   != DEAD) this->board_0[this->b_rook_2]   = BLACK | ROOK ;
    if (this->b_king     != DEAD) this->board_0[this->b_king]     = BLACK | KING ;
    if (this->b_bishop   != DEAD) this->board_0[this->b_bishop]   = BLACK | BISHOP ;
    if (this->b_knight_1 != DEAD) this->board_0[this->b_knight_1] = BLACK | KNIGHT ;
    if (this->b_knight_2 != DEAD) this->board_0[this->b_knight_2] = BLACK | KNIGHT ;
    if (this->b_pawn_1   != DEAD) this->board_0[this->b_pawn_1]   = BLACK | PAWN ;
    if (this->b_pawn_2   != DEAD) this->board_0[this->b_pawn_2]   = BLACK | PAWN ;
    if (this->b_pawn_3   != DEAD) this->board_0[this->b_pawn_3]   = BLACK | PAWN ;
    if (this->b_pawn_4   != DEAD) this->board_0[this->b_pawn_4]   = BLACK | PAWN ;

    if (this->w_rook_1   != DEAD) this->board_0[this->w_rook_1]   = WHITE | ROOK ;
    if (this->w_rook_2   != DEAD) this->board_0[this->w_rook_2]   = WHITE | ROOK ;
    if (this->w_king     != DEAD) this->board_0[this->w_king]     = WHITE | KING ;
    if (this->w_bishop   != DEAD) this->board_0[this->w_bishop]   = WHITE | BISHOP ;
    if (this->w_knight_1 != DEAD) this->board_0[this->w_knight_1] = WHITE | KNIGHT ;
    if (this->w_knight_2 != DEAD) this->board_0[this->w_knight_2] = WHITE | KNIGHT ;
    if (this->w_pawn_1   != DEAD) this->board_0[this->w_pawn_1]   = WHITE | PAWN ;
    if (this->w_pawn_2   != DEAD) this->board_0[this->w_pawn_2]   = WHITE | PAWN ;
    if (this->w_pawn_3   != DEAD) this->board_0[this->w_pawn_3]   = WHITE | PAWN ;
    if (this->w_pawn_4   != DEAD) this->board_0[this->w_pawn_4]   = WHITE | PAWN ;

    rotate_board(this->board_0, this->board_90, this->transform_array[1]);
    rotate_board(this->board_0, this->board_180, this->transform_array[2]);
    rotate_board(this->board_0, this->board_270, this->transform_array[3]);
}

void BoardData::set_7x7_transforms() {
    this->transform_array[0]         = (U8*)id_7x7;
    this->transform_array[1]         = (U8*)cw_90_7x7;
    this->transform_array[2]         = (U8*)cw_180_7x7;
    this->transform_array[3]         = (U8*)acw_90_7x7;
    this->inverse_transform_array[0] = (U8*)id_7x7;
    this->inverse_transform_array[1] = (U8*)acw_90_7x7;
    this->inverse_transform_array[2] = (U8*)cw_180_7x7;
    this->inverse_transform_array[3] = (U8*)cw_90_7x7;
}

void BoardData::set_8x8_transforms() {
    this->transform_array[0]         = (U8*)id_8x8;
    this->transform_array[1]         = (U8*)cw_90_8x8;
    this->transform_array[2]         = (U8*)cw_180_8x8;
    this->transform_array[3]         = (U8*)acw_90_8x8;
    this->inverse_transform_array[0] = (U8*)id_8x8;
    this->inverse_transform_array[1] = (U8*)acw_90_8x8;
    this->inverse_transform_array[2] = (U8*)cw_180_8x8;
    this->inverse_transform_array[3] = (U8*)cw_90_8x8;
}

void BoardData::set_7_3_layout() {
    this->b_rook_1 = pos(2,5);
    this->b_rook_2 = pos(2,6);
    this->b_king   = pos(3,5);
    this->b_bishop = pos(3,6);
    this->b_pawn_1 = pos(4,5);
    this->b_pawn_2 = pos(4,6);

    this->w_rook_1 = pos(4,1);
    this->w_rook_2 = pos(4,0);
    this->w_king   = pos(3,1);
    this->w_bishop = pos(3,0);
    this->w_pawn_1 = pos(2,1);
    this->w_pawn_2 = pos(2,0);

    this->pawn_promo_squares[0] = pos(2,0);
    this->pawn_promo_squares[1] = pos(2,1);
    this->n_pawn_promo_squares = 2;
}

void BoardData::set_8_4_layout() {
    this->w_pawn_1 = pos(2,1);
    this->w_pawn_2 = pos(2,0);
    this->w_pawn_3 = pos(5,1);
    this->w_pawn_4 = pos(5,0);
    this->w_king   = pos(3,1);
    this->w_bishop = pos(3,0);
    this->w_rook_1 = pos(4,1);
    this->w_rook_2 = pos(4,0);

    this->b_pawn_1 = pos(2,6);
    this->b_pawn_2 = pos(2,7);
    this->b_king   = pos(4,6);
    this->b_bishop = pos(4,7);
    this->b_rook_1 = pos(3,6);
    this->b_rook_2 = pos(3,7);
    this->b_pawn_3 = pos(5,6);
    this->b_pawn_4 = pos(5,7);

    this->pawn_promo_squares[0] = pos(2,0);
    this->pawn_promo_squares[1] = pos(2,1);
    this->n_pawn_promo_squares = 2;
}

void BoardData::set_8_2_layout() {
    this->w_pawn_1   = pos(2,1);
    this->w_pawn_2   = pos(2,0);
    this->w_pawn_3   = pos(2,2);
    this->w_pawn_4   = pos(5,2);
    this->w_king     = pos(4,1);
    this->w_knight_1 = pos(3,1);
    this->w_knight_2 = pos(3,2);
    this->w_bishop   = pos(4,2);
    this->w_rook_1   = pos(5,1);
    this->w_rook_2   = pos(5,0);

    this->b_pawn_1   = pos(5,6);
    this->b_pawn_2   = pos(5,7);
    this->b_pawn_3   = pos(2,5);
    this->b_pawn_4   = pos(5,5);
    this->b_king     = pos(3,6);
    this->b_knight_1 = pos(4,6);
    this->b_knight_2 = pos(4,5);
    this->b_bishop   = pos(3,5);
    this->b_rook_1   = pos(2,6);
    this->b_rook_2   = pos(2,7);

    this->pawn_promo_squares[0] = pos(2,0);
    this->pawn_promo_squares[1] = pos(2,1);
    this->pawn_promo_squares[2] = pos(2,2);
    this->n_pawn_promo_squares = 3;
}

BoardData::BoardData(BoardType btype): 
    board_0{0}, 
    board_90{0}, 
    board_180{0}, 
    board_270{0}, 
    pawn_promo_squares{0} {

    this->board_type = btype;

    if (btype == SEVEN_THREE) {
        this->set_7_3_layout();
        this->set_7x7_transforms();
        this->board_mask = (U8*)board_7_3;
    }
    else {
        this->set_8x8_transforms();
        if (btype == EIGHT_FOUR) {
            this->set_8_4_layout();
            this->board_mask = (U8*)board_8_4;
        }
        else {
            this->set_8_2_layout();
            this->board_mask = (U8*)board_8_2;
        }
    }

    this->set_pieces_on_board();
}

BoardData::BoardData() {}

BoardData::BoardData(const BoardData& source) {

    this->b_rook_1   = source.b_rook_1   ;
    this->b_rook_2   = source.b_rook_2   ;
    this->b_king     = source.b_king     ;
    this->b_bishop   = source.b_bishop   ;
    this->b_knight_1 = source.b_knight_1 ;
    this->b_knight_2 = source.b_knight_2 ;
    this->b_pawn_1   = source.b_pawn_1   ;
    this->b_pawn_2   = source.b_pawn_2   ;
    this->b_pawn_3   = source.b_pawn_3   ;
    this->b_pawn_4   = source.b_pawn_4   ;
                                         ;
    this->w_rook_1   = source.w_rook_1   ;
    this->w_rook_2   = source.w_rook_2   ;
    this->w_king     = source.w_king     ;
    this->w_bishop   = source.w_bishop   ;
    this->w_knight_1 = source.w_knight_1 ;
    this->w_knight_2 = source.w_knight_2 ;
    this->w_pawn_1   = source.w_pawn_1   ;
    this->w_pawn_2   = source.w_pawn_2   ;
    this->w_pawn_3   = source.w_pawn_3   ;
    this->w_pawn_4   = source.w_pawn_4   ;

    memcpy(this->board_0  , source.board_0  , 64);
    memcpy(this->board_90 , source.board_90 , 64);
    memcpy(this->board_180, source.board_180, 64);
    memcpy(this->board_270, source.board_270, 64);

    this->board_type = source.board_type;
    this->board_mask = source.board_mask;
    this->player_to_play = source.player_to_play;
    this->last_killed_piece = source.last_killed_piece;
    this->last_killed_piece_idx = source.last_killed_piece_idx;

    memcpy(this->transform_array, source.transform_array, 4*sizeof(U8*));
    memcpy(this->inverse_transform_array, source.inverse_transform_array, 4*sizeof(U8*));

    memcpy(this->pawn_promo_squares, source.pawn_promo_squares, 10);
    this->n_pawn_promo_squares = source.n_pawn_promo_squares;
}
