#pragma once

#include <vector>
#include <unordered_set>
#include <stack>
#include <map>
#include "constants.hpp"
#include "bdata.hpp"

/**
 * @brief Represents the chess board.
 *
 * This struct holds the data representing the state of the chess board. It
 * includes methods to retrieve legal and pseudolegal moves, check if the board
 * is in check, and perform moves on the board.
 */
struct Board {

  BoardData data; /* The data representing the state of the chess board. */

  /**
   * @brief Default constructor.
   *
   * Initializes the board with the standard starting position.
   */
  Board();

  /**
   * @brief Constructor with board type.
   *
   * Initializes the board with the provided board configuration.
   *
   * @param btype The type of board configuration to initialize with.
   */
  Board(BoardType btype);

  /**
   * @brief Constructor with board data.
   *
   * Initializes the board with the provided board data.
   *
   * @param bdata The board data to initialize with.
   */
  Board(BoardData bdata);

  /**
   * @brief Copy constructor.
   *
   * Creates a copy of the provided board.
   *
   * @param source The board object to copy.
   */
  Board(const Board &source);

  /**
   * @brief Get the legal moves for the current board state.
   *
   * This method returns the set of legal moves that can be made from the
   * current board state. A legal move is a pseudolegal move that does not
   * result in the current player's king being under threat from opponent's
   * pieces.
   *
   * @return The set of legal moves for the current board state.
   */
  std::unordered_set<U16> get_legal_moves() const;

  /**
   * @brief Check if the current player is in check.
   *
   * This method checks if the current player is in check, i.e., if the
   * player's king is under threat from opponent's pieces.
   *
   * @return True if the current player is in check, false otherwise.
   */
  bool in_check() const;

  /**
   * @brief Perform a move on the board.
   *
   * This method performs the specified move on the board, updating the board
   * state accordingly. After the move is performed, it changes the current
   * player to the opponent.
   *
   * @param move The move to be performed.
   */
  void do_move_(U16 move);

  /**
   * @brief Get the pseudolegal moves for the current board state.
   *
   * This method returns the set of pseudolegal moves that can be made from
   * the current board state. A pseudolegal move is a move that satisfies the
   * basic rules of the chess pieces on the board, without considering factors
   * such as checks or blocked pieces.
   *
   * @return The set of pseudolegal moves for the current board state.
   */
  std::unordered_set<U16> get_pseudolegal_moves() const;

  /**
   * @brief Get the pseudolegal moves for a specific piece on the board.
   *
   * This method returns the set of pseudolegal moves that can be made by the
   * specified piece on the current board state.
   *
   * @param piece_pos The position of the piece on the board.
   * @return The set of pseudolegal moves for the specified piece on the
   * current board state.
   */
  std::unordered_set<U16> get_pseudolegal_moves_for_piece(U8 piece_pos) const;

  /**
   * @brief Flip the current player.
   *
   * This method flips the current player, changing it from white to black or
   * from black to white.
   */
  void flip_player_();

  /**
   * @brief Perform a move on the board without flipping the current player.
   *
   * This method performs the specified move on the board, updating the board
   * state accordingly. It does not change the current player.
   *
   * @param move The move to be performed.
   */
  void do_move_without_flip_(U16 move);

  /**
   * @brief Check if a specific piece on the board is under threat.
   *
   * This method checks if the specified piece on the board is under threat
   * from opponent's pieces.
   *
   * @param piece_pos The position of the piece on the board.
   * @return True if the specified piece is under threat, false otherwise.
   */
  bool under_threat(U8 piece_pos) const;

  /**
   * @brief Undo the last move on the board without flipping the current
   * player.
   *
   * This method undoes the last move that was performed on the board,
   * restoring the previous board state. It does not change the current
   * player.
   *
   * @param move The move to be undone.
   */
  void undo_last_move_without_flip_(U16 move);

  /**
   * @brief Get the pseudolegal moves for a specific side (color) on the
   * board.
   *
   * This method returns the set of pseudolegal moves that can be made by the
   * specified side (color) on the current board state.
   *
   * @param color The color of the side for which to get the pseudolegal
   * moves.
   * @return The set of pseudolegal moves for the specified side on the
   * current board state.
   */
  std::unordered_set<U16> get_pseudolegal_moves_for_side(U8 color) const;
};
