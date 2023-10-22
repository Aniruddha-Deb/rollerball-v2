#pragma once

#include <vector>
#include <unordered_set>
#include <stack>
#include "constants.hpp"

/**
 * Declares PlayerColor enum with possible values: WHITE, BLACK.
 */
enum PlayerColor {
    WHITE=(1<<6),
    BLACK=(1<<7)
};

/**
 * Enumerates available chess pieces and EMPTY state for a spot on the board.
 */
enum PieceType {
    EMPTY  = 0,
    PAWN   = (1<<1),
    ROOK   = (1<<2),
    KING   = (1<<3),
    BISHOP = (1<<4),
    KNIGHT = (1<<5)
};

/**
 * Enumerates possible pawn promotion states for Pawn chess piece.
 */
enum Promotion {
    PAWN_BISHOP = (1<<6),
    PAWN_ROOK   = (1<<7)
};

/**
 * Enumerates possible board configurations by explicitly stating the 
 * difference between the boards.
 */
enum BoardType {
    SEVEN_THREE = 1,
    EIGHT_FOUR = 2,
    EIGHT_TWO = 3 
};

/**
 * The BoardData struct serves as an object-based representation of the current
 * board state and configuration. Provides member functions and instances that
 * will serve to represent the game's current status and progress.
 */
struct BoardData {

  // Declare variables that hold position of each piece. Default to DEAD.
  // DO NOT add any fields above this
  U8 w_rook_1 = DEAD;
  U8 w_rook_2 = DEAD;
  U8 w_king = DEAD;
  U8 w_bishop = DEAD;
  U8 w_knight_1 = DEAD;
  U8 w_knight_2 = DEAD;
  U8 w_pawn_1 = DEAD;
  U8 w_pawn_2 = DEAD;
  U8 w_pawn_3 = DEAD;
  U8 w_pawn_4 = DEAD;

  U8 b_rook_1 = DEAD;
  U8 b_rook_2 = DEAD;
  U8 b_king = DEAD;
  U8 b_bishop = DEAD;
  U8 b_knight_1 = DEAD;
  U8 b_knight_2 = DEAD;
  U8 b_pawn_1 = DEAD;
  U8 b_pawn_2 = DEAD;
  U8 b_pawn_3 = DEAD;
  U8 b_pawn_4 = DEAD;

  static const int n_pieces = 10;

  // Array that holds the board state from different orientations.
  U8 board_0[64];
  U8 board_90[64];
  U8 board_180[64];
  U8 board_270[64];

  // Variables that record the game status and configuration.
  BoardType board_type = SEVEN_THREE;
  U8 *board_mask;
  PlayerColor player_to_play = WHITE;
  U8 last_killed_piece = 0;
  int last_killed_piece_idx = -1;

  // Transformation arrays for 90, 180, and 270 degree rotations of the matrix.
  U8 *transform_array[4];
  U8 *inverse_transform_array[4];

  // Variables that represent pawn promotion squares
  U8 pawn_promo_squares[10];
  int n_pawn_promo_squares;

  /**
   * Default constructor - initializes an instance of the BoardData structure.
   */
  BoardData();

  /**
   * Constructor - accepts an enumerated Board type and initializes BoardData
   * instance based on it.
   * @param board_type - type of the board: SEVEN_THREE, EIGHT_TWO, EIGHT_FOUR.
   */
  BoardData(BoardType board_type);

  /**
   * Copy constructor - creates a deep copy of the source instance.
   * @param source - source BoardData instance from which to copy.
   */
  BoardData(const BoardData &source);

  /**
   * member function that sets pieces on the board based on their status(dead or
   * alive).
   */
  void set_pieces_on_board();

  /**
   * member function that sets the board layout for 8x4.
   */
  void set_8_4_layout();

  /**
   * member function that sets the board layout for 8x2.
   */
  void set_8_2_layout();

  /**
   * member function that sets the board layout for 7x3.
   */
  void set_7_3_layout();

  /**
   * member function that implements 7x7 transforms.
   * Sets the transformation and inverse transformation arrays to represent 7x7
   * board rotation.
   */
  void set_7x7_transforms();

  /**
   * member function that implements 8x8 transforms.
   * Sets the transformation and inverse transformation arrays to represent 8x8
   * board rotation.
   */
  void set_8x8_transforms();
};
