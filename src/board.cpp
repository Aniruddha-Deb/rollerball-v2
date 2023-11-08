#include <string>
#include <iostream>
#include "board.hpp"
#include "butils.hpp"
#include "constants.hpp"
#include <cstring>

std::unordered_set<U16> transform_moves(const std::unordered_set<U16>& moves, const U8 *transform) {

    std::unordered_set<U16> rot_moves;

    for (U16 move : moves) {
        rot_moves.insert(move_promo(transform[getp0(move)], transform[getp1(move)], getpromo(move)));
    }

    return rot_moves;
}

std::unordered_set<U16> construct_rook_moves(const U8 p0, const U8 *board, const U8 *bmask) {

    PlayerColor color = color(board[p0]);
    PlayerColor oppcolor = oppcolor(board[p0]);
    std::unordered_set<U16> rook_moves;

    // right - move one square 
    if (inboard(bmask, getx(p0)+1, gety(p0)) && 
        !occupied(board, p0+pos(1,0), color)) rook_moves.insert(move(p0, p0+pos(1,0)));

    // bottom - move one square 
    if (inboard(bmask, getx(p0), gety(p0)-1) && 
        !occupied(board, p0-pos(0,1), color)) rook_moves.insert(move(p0, p0-pos(0,1)));

    // top - move multiple if left end (forward), move one if right end
    if (inboard(bmask, getx(p0), gety(p0)+1)) {
        if (getx(p0) >= 4 && !occupied(board, p0+pos(0,1), color)) {
            // right end 
            rook_moves.insert(move(p0, p0+pos(0,1)));
        }
        else {
            for (int y=1; inboard(bmask, getx(p0), gety(p0)+y); y++) {
                U8 tgt_pos = p0+pos(0,y);
                if (occupied(board, tgt_pos, color)) break;
                
                rook_moves.insert(move(p0, tgt_pos));
                if (occupied(board, tgt_pos, oppcolor)) break;
            }
        }
    }

    // left
    bool blocked = false;
    for (int x=1; inboard(bmask, getx(p0)-x, gety(p0)); x++) {
        U8 tgt_pos = p0-pos(x,0);
        if (occupied(board, tgt_pos, color)) { blocked = true; break; }
        
        rook_moves.insert(move(p0, tgt_pos));
        if (occupied(board, tgt_pos, oppcolor)) { blocked = true; break; }
    }

    // reflect if on outer ring and not blocked yet
    if (!blocked && gety(p0) == 0) {
        for (int y=1; inboard(bmask, 0, y); y++) {
            U8 tgt_pos = pos(0,y);
            if (occupied(board, tgt_pos, color)) break;
            
            rook_moves.insert(move(p0, tgt_pos));
            if (occupied(board, tgt_pos, oppcolor)) break;
        }
    }

    return rook_moves;
}

std::unordered_set<U16> construct_bishop_moves(const U8 p0, const U8 *board, const U8 *bmask) {

    PlayerColor color = color(board[p0]);
    PlayerColor oppcolor = oppcolor(board[p0]);
    std::unordered_set<U16> bishop_moves;

    // top right - move one square 
    if (inboard(bmask, getx(p0)+1, gety(p0)+1) && 
        !occupied(board, p0+pos(1,1), color)) bishop_moves.insert(move(p0, p0+pos(1,1)));

    // bottom right - move one square 
    if (inboard(bmask, getx(p0)+1, gety(p0)-1) && 
        !occupied(board, p0+pos(1,0)-pos(0,1), color)) 
        bishop_moves.insert(move(p0, p0+pos(1,0)-pos(0,1)));

    // top left - move till reflection, then reflect
    bool blocked = false;
    U8 tgt_pos = DEAD;
    for (int s=1; inboard(bmask, getx(p0)-s, gety(p0)+s); s++) {
        tgt_pos = p0-pos(s,0)+pos(0,s);
        if (occupied(board, tgt_pos, color)) { blocked = true; break; }
        
        bishop_moves.insert(move(p0, tgt_pos));
        if (occupied(board, tgt_pos, oppcolor)) { blocked = true; break; }
    }

    // reflect on left / top edge
    if (!blocked) {
        // std::cout << "Not blocked, reflecting\n";
        U8 p1 = tgt_pos;
        if (getx(tgt_pos) == 0) {
            // left edge 
            for (int s=1; inboard(bmask, getx(p1)+s, gety(p1)+s); s++) {
                U8 tgt_pos = p1+pos(s,s);
                if (occupied(board, tgt_pos, color)) { blocked = true; break; }
                
                bishop_moves.insert(move(p0, tgt_pos));
                if (occupied(board, tgt_pos, oppcolor)) { blocked = true; break; }
            }
        }
        else {
            // top edge
            for (int s=1; inboard(bmask, getx(p1)-s, gety(p1)-s); s++) {
                U8 tgt_pos = p1-pos(s,s);
                if (occupied(board, tgt_pos, color)) { blocked = true; break; }
                
                bishop_moves.insert(move(p0, tgt_pos));
                if (occupied(board, tgt_pos, oppcolor)) { blocked = true; break; }
            }
        }
    }

    // bottom left - move till reflection, then reflect
    blocked = false;
    tgt_pos = DEAD;
    for (int s=1; inboard(bmask, getx(p0)-s, gety(p0)-s); s++) {
        tgt_pos = p0-pos(s,s);
        if (occupied(board, tgt_pos, color)) { blocked = true; break; }
        
        bishop_moves.insert(move(p0, tgt_pos));
        if (occupied(board, tgt_pos, oppcolor)) { blocked = true; break; }
    }

    // reflect on bottom edge
    if (!blocked) {
        U8 p1 = tgt_pos;
        for (int s=1; inboard(bmask, getx(p1)-s, gety(p1)+s); s++) {
            tgt_pos = p1-pos(s,0)+pos(0,s);
            if (occupied(board, tgt_pos, color)) { blocked = true; break; }
            
            bishop_moves.insert(move(p0, tgt_pos));
            if (occupied(board, tgt_pos, oppcolor)) { blocked = true; break; }
        }
    }

    return bishop_moves;
}

std::unordered_set<U16> construct_knight_moves(const U8 p0, const U8 *board, const U8 *bmask) {

    PlayerColor color = color(board[p0]);
    std::unordered_set<U16> knight_moves;

    // similar to king moves
    int x_incrs[8] = {1, 2,  2,  1, -1, -2, -2, -1};
    int y_incrs[8] = {2, 1, -1, -2, -2, -1,  1,  2};

    for (int i=0; i<8; i++) {
        if (!inboard(bmask, getx(p0)+x_incrs[i], gety(p0)+y_incrs[i])) continue;
        U8 p1 = pos(getx(p0)+x_incrs[i], gety(p0)+y_incrs[i]);
        if (!occupied(board, p1, color)) knight_moves.insert(move(p0, p1));
    }

    return knight_moves;
}

bool can_promote(U8 pos, U8 *promo, int n_promo) {
    for (int i=0; i<n_promo; i++) {
        if (promo[i] == pos) return true;
    }
    return false;
}

std::unordered_set<U16> construct_pawn_moves(const U8 p0, const U8 *board, const U8 *bmask,
        U8 *promo, int n_promo, bool promote) {
    
    PlayerColor color = color(board[p0]);
    std::unordered_set<U16> pawn_moves;

    for (int y = gety(p0)-1; y <= gety(p0) + 1; y++) {
        if (!inboard(bmask, getx(p0)-1, y)) continue;
        U8 p1 = pos(getx(p0)-1, y);
        if (!occupied(board, p1, color)) {
            if (promote && can_promote(p1, promo, n_promo)) {
                pawn_moves.insert(move_promo(p0, p1, PAWN_ROOK));
                pawn_moves.insert(move_promo(p0, p1, PAWN_BISHOP));
            }
            else {
                pawn_moves.insert(move(p0, p1));
            }
        }
    }

    return pawn_moves;
}

std::unordered_set<U16> construct_king_moves(const U8 p0, const U8 *board, const U8 *bmask) {

    PlayerColor color = color(board[p0]);
    std::unordered_set<U16> king_moves;

    int x_incrs[8] = {1, 1,  1, 0,  0, -1, -1, -1};
    int y_incrs[8] = {1, 0, -1, 1, -1,  1,  0, -1};

    for (int i=0; i<8; i++) {
        if (!inboard(bmask, getx(p0)+x_incrs[i], gety(p0)+y_incrs[i])) continue;
        U8 p1 = pos(getx(p0)+x_incrs[i], gety(p0)+y_incrs[i]);
        if (!occupied(board, p1, color)) king_moves.insert(move(p0, p1));
    }

    return king_moves;
}

std::unordered_set<U16> Board::get_pseudolegal_moves_for_piece(U8 piece_pos) const {

    std::unordered_set<U16> moves;
    U8 piece_id = this->data.board_0[piece_pos];
    int board_idx = data.board_mask[piece_pos] - 2;
    const U8 *transform_arr = this->data.transform_array[board_idx];
    const U8 *inv_transform_arr = this->data.inverse_transform_array[board_idx];

    const U8 *board = this->data.board_0;
    if (board_idx == 1) board = this->data.board_270;
    if (board_idx == 2) board = this->data.board_180;
    if (board_idx == 3) board = this->data.board_90;

    if (piece_id & PAWN) {
        moves = construct_pawn_moves(inv_transform_arr[piece_pos], board, this->data.board_mask,
                (U8*)this->data.pawn_promo_squares, this->data.n_pawn_promo_squares,
                (board_idx==2 && (piece_id & WHITE)) || (board_idx==0 && (piece_id & BLACK)));
    }
    else if (piece_id & ROOK) {
        moves = construct_rook_moves(inv_transform_arr[piece_pos], board, this->data.board_mask);
    }
    else if (piece_id & BISHOP) {
        moves = construct_bishop_moves(inv_transform_arr[piece_pos], board, this->data.board_mask);
    }
    else if (piece_id & KING) {
        moves = construct_king_moves(inv_transform_arr[piece_pos], board, this->data.board_mask);
    }
    else if (piece_id & KNIGHT) {
        moves = construct_knight_moves(inv_transform_arr[piece_pos], board, this->data.board_mask);
    }

    moves = transform_moves(moves, transform_arr);
    return moves;
}

Board::Board(): data{SEVEN_THREE} {}

Board::Board(BoardType btype): data{btype} {}

Board::Board(BoardData bdata) {
    this->data = bdata; // copy constructor
}

Board::Board(const Board& source) {
    this->data = source.data; // copy constructor
}

bool Board::under_threat(U8 piece_pos) const {

    auto pseudolegal_moves = this->get_pseudolegal_moves_for_side(this->data.player_to_play ^ (WHITE | BLACK));

    for (auto move : pseudolegal_moves) {
        // std::cout << move_to_str(move) << " ";
        if (getp1(move) == piece_pos) {
            // std::cout << "<- causes check\n";
            return true;
        }
    }
    // std::cout << std::endl;

    return false;
}

bool Board::in_check() const {

    auto king_pos = this->data.w_king;
    if (this->data.player_to_play == BLACK) {
        king_pos = this->data.b_king;
    }

    return under_threat(king_pos);
}

std::unordered_set<U16> Board::get_pseudolegal_moves() const {
    return get_pseudolegal_moves_for_side(this->data.player_to_play);
}

std::unordered_set<U16> Board::get_pseudolegal_moves_for_side(U8 color) const {

    std::unordered_set<U16> pseudolegal_moves;

    int si = (color>>7) * 10;

    U8 *pieces = (U8*)(&this->data);
    for (int i=0; i<this->data.n_pieces; i++) {
        U8 piece = pieces[si+i];
        if (piece == DEAD) continue;
        auto piece_moves = this->get_pseudolegal_moves_for_piece(piece);
        pseudolegal_moves.insert(piece_moves.begin(), piece_moves.end());
    }

    return pseudolegal_moves;
}

// legal move generation:
// Get all pseudolegal moves
// for each pseudolegal move for our color:
//     if doing this move will leave the king in threat from opponent's pieces
//         don't add the move to legal moves
//     else
//         add to legal moves
std::unordered_set<U16> Board::get_legal_moves() const {

    Board c(*this);
    auto pseudolegal_moves = c.get_pseudolegal_moves();
    std::unordered_set<U16> legal_moves;

    for (auto move : pseudolegal_moves) {
        c.do_move_without_flip_(move);

        if (!c.in_check()) {
            legal_moves.insert(move);
        }

        c.undo_last_move_without_flip_(move);
    }

    return legal_moves;
}

void Board::do_move_(U16 move) {
    do_move_without_flip_(move);
    flip_player_();
}

void Board::flip_player_() {
    this->data.player_to_play = (PlayerColor)(this->data.player_to_play ^ (WHITE | BLACK));
}

void Board::do_move_without_flip_(U16 move) {

    U8 p0 = getp0(move);
    U8 p1 = getp1(move);
    U8 promo = getpromo(move);

    U8 piecetype = this->data.board_0[p0];
    this->data.last_killed_piece = 0;
    this->data.last_killed_piece_idx = -1;

    // scan and get piece from coord
    U8 *pieces = (U8*)this;
    for (int i=0; i<2*this->data.n_pieces; i++) {
        if (pieces[i] == p1) {
            pieces[i] = DEAD;
            this->data.last_killed_piece = this->data.board_0[p1];
            this->data.last_killed_piece_idx = i;
        }
        if (pieces[i] == p0) {
            pieces[i] = p1;
        }
    }

    if (promo == PAWN_ROOK) {
        piecetype = (piecetype & (WHITE | BLACK)) | ROOK;
    }
    else if (promo == PAWN_BISHOP) {
        piecetype = (piecetype & (WHITE | BLACK)) | BISHOP;
    }

    this->data.board_0  [this->data.transform_array[0][p1]] = piecetype;
    this->data.board_90 [this->data.transform_array[1][p1]] = piecetype;
    this->data.board_180[this->data.transform_array[2][p1]] = piecetype;
    this->data.board_270[this->data.transform_array[3][p1]] = piecetype;

    this->data.board_0  [this->data.transform_array[0][p0]] = 0;
    this->data.board_90 [this->data.transform_array[1][p0]] = 0;
    this->data.board_180[this->data.transform_array[2][p0]] = 0;
    this->data.board_270[this->data.transform_array[3][p0]] = 0;

}

void Board::undo_last_move_without_flip_(U16 move) {

    U8 p0 = getp0(move);
    U8 p1 = getp1(move);
    U8 promo = getpromo(move);

    U8 piecetype = this->data.board_0[p1];
    U8 deadpiece = this->data.last_killed_piece;
    this->data.last_killed_piece = 0;

    // scan and get piece from coord
    U8 *pieces = (U8*)(&(this->data));
    for (int i=0; i<2*this->data.n_pieces; i++) {
        if (pieces[i] == p1) {
            pieces[i] = p0;
            break;
        }
    }
    if (this->data.last_killed_piece_idx >= 0) {
        pieces[this->data.last_killed_piece_idx] = p1;
        this->data.last_killed_piece_idx = -1;
    }

    if (promo == PAWN_ROOK) {
        piecetype = ((piecetype & (WHITE | BLACK)) ^ ROOK) | PAWN;
    }
    else if (promo == PAWN_BISHOP) {
        piecetype = ((piecetype & (WHITE | BLACK)) ^ BISHOP) | PAWN;
    }

    this->data.board_0  [this->data.transform_array[0][p1]] = deadpiece;
    this->data.board_90 [this->data.transform_array[1][p1]] = deadpiece;
    this->data.board_180[this->data.transform_array[2][p1]] = deadpiece;
    this->data.board_270[this->data.transform_array[3][p1]] = deadpiece;

    this->data.board_0  [this->data.transform_array[0][p0]] = piecetype;
    this->data.board_90 [this->data.transform_array[1][p0]] = piecetype;
    this->data.board_180[this->data.transform_array[2][p0]] = piecetype;
    this->data.board_270[this->data.transform_array[3][p0]] = piecetype;

}
