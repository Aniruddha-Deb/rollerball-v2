#pragma once

#include <vector>
#include <unordered_set>
#include <stack>
#include "constants.hpp"
#include "board.hpp"

/**
* This function is used to convert move in U16 format to string format.
* @param move which is a parameter of type U16 representing a move.
* @return s which represents a move in string format.
*/
std::string move_to_str(U16 move);

/**
* This function is used to convert a move in string format to U16 format.
* @param move which is a parameter of type string representing a move.
* @return a Move type representing the move in U16 datatype. 
*/
U16 str_to_move(std::string move);

/**
 * This function is used to visualize the moves onto a string representation of
 * a board.
 * @param BoardData point b to the current board data and moves which are a list
 * of possible moves.
 * @return board_str which is a string representation of the board with possible
 * moves highlighted with 'X' marks.
 */
std::string show_moves(const BoardData *b, std::unordered_set<U16> &moves);

/**
 * This function is used to convert all board configurations (0, 90, 180, 270)
 * to string format for visualization.
 * @param Board object b representing the current state of the board.
 * @return board_str which is a string representation of all board
 * configurations.
 */
std::string all_boards_to_str(const Board &b);

/**
 * This function is used to convert a piece represented in byte format to
 * character representing the same.
 * @param piece which is a parameter of type byte representing a chess piece.
 * @return ch which is a character representing a chess piece(following the
 * naming convention: pawn - p, rook - r, bishop - b, knight - n and king - k).
 */
char piece_to_char(U8 piece);

/**
 * This function is used to convert the board data to string for visualization.
 * It can handle different board configurations (7_3, 8_4) and represents
 * different square configurations.
 * @param pointer b pointing to the BoardData object representing the current
 * state of the chess board.
 * @return board_str which is a string representation of the board.
 */
std::string board_to_str(const BoardData *b);

/**
* The function is used to convert a 7_3 board to string format for visualization.
* @param pointer b pointing to the byte representation of a 7_3 board.
* @return board_str which is a string representation of the 7_3 board. 
*/
std::string board_7_3_to_str(const U8 *b);

