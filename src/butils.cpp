#include <string>
#include <iostream>
#include "board.hpp"
#include "butils.hpp"
#include "constants.hpp"
#include <cstring>

char piece_to_char(U8 piece) {
    char ch = '.';
    if      (piece & PAWN)   ch = 'p';
    else if (piece & ROOK)   ch = 'r';
    else if (piece & BISHOP) ch = 'b';
    else if (piece & KING)   ch = 'k';
    else if (piece & KNIGHT) ch = 'n';

    if (piece & WHITE) ch = ch - ('a'-'A');

    return ch;
}

std::string board_7_3_to_str(const U8 *b) {
    std::string board_str = ".......\n.......\n..   ..\n..   ..\n..   ..\n.......\n.......\n";

    for (int i=0; i<56; i++) {
        U8 piece = b[i];
        if (board_str[i] == '\n' || board_str[i] == ' ') continue;
        board_str[(48-(i/8)*8) + i%8] = piece_to_char(piece);
    }

    return board_str;
}

std::string board_to_str(const BoardData *b) {

    std::string board_str = "Could not convert board";
    if (b->board_type == SEVEN_THREE) {
        board_str = ".......\n.......\n..   ..\n..   ..\n..   ..\n.......\n.......\n";

        for (int i=0; i<56; i++) {
            U8 piece = b->board_0[i];
            if (board_str[i] == '\n' || board_str[i] == ' ') continue;
            board_str[(48-(i/8)*8) + i%8] = piece_to_char(piece);
        }
    }
    else {

        if (b->board_type == EIGHT_FOUR) {
            board_str = "........\n........\n..    ..\n..    ..\n..    ..\n..    ..\n........\n........\n";
        }
        else {
            board_str = "........\n........\n........\n...  ...\n...  ...\n........\n........\n........\n";
        }

        for (int i=0; i<72; i++) {
            if (board_str[i] == '\n' || board_str[i] == ' ') continue;
            int idx = i - i/9;
            U8 piece = b->board_0[idx];
            board_str[(63-(idx/8)*9) + idx%8] = piece_to_char(piece);
        }
    }

    return board_str;
}

std::string show_moves(const BoardData *board, std::unordered_set<U16>& moves) {

    std::string board_str = board_to_str(board);

    for (auto m : moves) {
        U8 p1 = getp1(m);
        if (board->board_type == SEVEN_THREE) board_str[(48-(p1/8)*8) + p1%8] = 'X';
        else board_str[(63-(p1/8)*9) + p1%8] = 'X';
    }

    return board_str;
}

std::string player_to_play_to_str(const Board& b) {
    if (b.data.player_to_play == WHITE) {
        return "WHITE";
    }
    else if (b.data.player_to_play == BLACK) {
        return "BLACK";
    }
    else {
        return "UNKNOWN";
    }
}

std::string all_boards_to_str(const Board& b) {

    std::string board_str(256, ' ');
    std::string board_mask = ".......\n.......\n..   ..\n..   ..\n..   ..\n.......\n.......\n";

    const U8 (*boards)[64] = &(b.data.board_0);

    for (int b=0; b<4; b++) {
        for (int i=0; i<56; i++) {
            if (board_mask[i] == '\n' || board_mask[i] == ' ') continue;
            board_str[(224-(i/8)*32) + b*8 + i%8] = piece_to_char(boards[b][i]);
        }
    }

    for (int i=31; i<256; i+=32) {
        board_str[i] = '\n';
    }

    return board_str.substr(32);
}

std::string move_to_str(U16 move) {

    std::string s = "a1a1";
    s[0] += getx(getp0(move));
    s[1] += gety(getp0(move));
    s[2] += getx(getp1(move));
    s[3] += gety(getp1(move));
    if (getpromo(move) & PAWN_BISHOP) {
        s += "b";
    }
    else if (getpromo(move) & PAWN_ROOK) {
        s += "r";
    }

    if (move == 0) s = "0000"; // null move

    return s;
}

U16 str_to_move(std::string move) {
    
    U8 x0 = move[0] - 'a';
    U8 y0 = move[1] - '1';
    U8 x1 = move[2] - 'a';
    U8 y1 = move[3] - '1';
    U8 promo = 0;
    if (move.size() > 4) {
        if (move[4] == 'r') promo = PAWN_ROOK;
        else promo = PAWN_BISHOP;
    }

    return move_promo(pos(x0,y0), pos(x1,y1), promo);
}
